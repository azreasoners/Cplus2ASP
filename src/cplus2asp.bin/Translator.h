#pragma once
#include <iostream>
#include <map>

#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/SymbolTable.h"
#include "bcplus/Location.h"
#include "bcplus/languages/BCPlus.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/declarations.h"
#include "bcplus/parser/BCParser.h"
#include "bcplus/elements/terms.h"
#include "bcplus/elements/formulas.h"

#include "Configuration.h"
#include "types.h"
#include "Context.h"

namespace cplus2asp {
namespace cplus2asp_bin {

class Translator : public babb::utils::Referenced
{
public:
	/************************************************************************/
	/* Types */
	/************************************************************************/

	/// Simple initializer for metadata
	class SymbolMetadataInitializer : public bcplus::symbols::SymbolTable::SymbolMetadataInitializer
	{

		/// Call to initialize metadata for a new symbol being added to teh symbol table.
		virtual void initMetadata(bcplus::symbols::Symbol* sym) const;

	};



private:
	/************************************************************************/
	/* Types */
	/************************************************************************/

	/// A structure used to store metadata for variables created during translation...
	class varinfo_t {
	private:
		babb::utils::ref_ptr<const ReferencedString> _name;			///< The name of the variable
		int _laststmt;
	public:
		/// Basic constructor
		varinfo_t(ReferencedString const* name, int stmt = 0) 
			: _name(name), _laststmt(stmt) { }

		/// Mark that this variable was used this statement
		inline void mark(int stmt) { _laststmt = stmt; }

		/// Get the variable name
		inline ReferencedString const* name() const { return _name; }

		/// Determine whether this variable has been used this statement
		inline bool used(int stmt) const { return _laststmt >= stmt; }
	};


	typedef std::map<bcplus::symbols::SortSymbol const*, std::list<varinfo_t> > varmap_t;


	/************************************************************************/
	/* Members */
	/************************************************************************/
	/// System configuration information
	babb::utils::ref_ptr<const Configuration> _config;

	/// Program symbol table
	babb::utils::ref_ptr<bcplus::symbols::SymbolTable> _symtab;

	/// A map of all of our temporary variables organized by their sort...
	varmap_t _global_varmap;
	varmap_t _local_varmap;

	/// The current IPart we're working with
	IPart::type _ipart;

	/// incremented for each new constant created
	int _constcount;

	/// incremented for each new statement.
	int _stmtcount;
	
	/// Whether we should output concurrency prohibiting enforcing rules for boolean actions
	bool _noconcurrency;

	/// Whether we should output concurrency pohibiting enforcing rules for all actions
	bool _strong_noconcurrency;

	/// Whether we've instantiated a "computed" sort object
	bool _computed;

public:
	/************************************************************************/
	/* Constructors / Destructors */
	/************************************************************************/
	/// Create a new translator instance.
	/// @param config The system wide configuration information.
	/// @param symtab The program's symbol table.
	Translator(Configuration const* config, bcplus::symbols::SymbolTable* symtab);

	/// Stub destructor
	virtual ~Translator();


	/************************************************************************/
	/* Public Methods */
	/************************************************************************/

	/// Get the system wide configuration
	inline Configuration const* config() const					{ return _config; }

	/// Get the program's symbol table
	inline bcplus::symbols::SymbolTable const* symtab() const	{ return _symtab; }
	inline bcplus::symbols::SymbolTable* symtab() 				{ return _symtab; }

	/// Output the preamble to the translation
	void prologue();


	/// Output the epilogue to the translation
	void epilogue();


	/// Translate the provided statement, outputting the result to the configured output stream.
	/// @param stmt The statement to translate
	/// @return true if successful, false otherwise
	bool translate(bcplus::statements::Statement const* stmt);
		

private:
	/// Call to translate a normal formed causal law
	bool translateCausalLaw(bcplus::elements::Formula const* head, bcplus::elements::Formula const* ifbody, bcplus::elements::Formula const* ifcons, bcplus::elements::Formula const* after, bcplus::elements::AtomicFormula const* unless, bcplus::elements::Formula const* where, StatementList* preStmts, ClauseList* extraClauses, std::stringstream& tmpout);

	/// CAll to translate an incremental causal law
	bool translateIncrementalLaw(el::AtomicFormula const* body, el::Constant const* head, el::Term const* value, el::Formula const* ifbody, el::AtomicFormula const* unless, el::Formula const* where, bool positive, StatementList* preStmts, ClauseList* extraClauses, std::stringstream& tmpout);

	/// Internal translation calls
	bool translate(bcplus::symbols::SortSymbol const* sort, std::ostream& out);
	bool translate(bcplus::symbols::VariableSymbol const* sym, std::ostream& out);
	bool translate(bcplus::symbols::ConstantSymbol const* sym, Context* c, std::ostream& out, bool args, ClauseList const* setArgs = NULL);
	bool translate(bcplus::symbols::ObjectSymbol const* sym, Context* c, std::ostream& out, bool args);
	bool translate(bcplus::symbols::NumberRangeSymbol const* t, Context* c, std::ostream& out);

	bool translate(bcplus::elements::Formula const* f, Context* c, std::ostream& out);
	bool translate(bcplus::elements::AtomicFormula const* af, Context* c, std::ostream& out, bool negate = false);

	bool translate(bcplus::elements::Term const* t, Context* c, std::ostream& out);
	
	/// Translate equality into an atomic formula
	bool translate_eq(bcplus::elements::Constant const* constant, std::string const& value, Context* c, std::ostream& out);
	bool translate_eq(bcplus::symbols::ConstantSymbol const* constant, std::string const& value, Context* c, std::ostream& out, ClauseList const* setArgs = NULL);
	
	/// Translate contribution atom
	bool translate_contrib(bcplus::elements::AtomicFormula const* body, bcplus::elements::Constant const* head, bcplus::elements::Term const* value, bool positive, Context* c, std::ostream& out);

	/// Get a new variable for the provided sort...
	/// This variable is unique for the current statement we're translating but may be reused from earlier.
	/// @param sort The sor that the variable should range over.
	/// @param The context to (possibly) add the domain declaration to...
	/// @param global Whether to get a globally declared variable or not.
	ReferencedString const* newVar(bcplus::symbols::SortSymbol const* sort, Context* c, bool global = true);

	/// Marks the beginning of a new statement and adds the provided statement to the given context
	/// @param c The context to add the statement to
	/// @param stmt The statement to add
	/// @param ipart The ipart the statement belongs to
	void nextStmt(Context* c, std::string const& stmt, IPart::type ipart);


	/// Create a new unique constant name based on the provided base string
	std::string newConst(std::string const& descr);

	/// Ensure that the ipart matches the provided value
	void assertIPart(IPart::type ipart, std::string const* step = NULL);


	/// Helper function for translating a subformula and capturing generated clauses
	bool bindAndTranslate(bcplus::elements::Formula const* f, Context* c, std::ostream& out, bool parens = true);

	
	/// add a translation of the provided object-sort binding as a prereq statement to the provided context
	bool translateObjectDeclaration(bcplus::symbols::ObjectSymbol const* obj, bcplus::symbols::SortSymbol const* sort, Context* c);

	/// add a translation of the provided range-sort binding as a prereq statement to the provided context
	bool translateRangeDeclaration(bcplus::symbols::NumberRangeSymbol const* range, bcplus::symbols::SortSymbol const* sort, Context* c);

	/// add a translation of a sort declaration as a prereq statement to the provided context
	bool translateSortDeclaration(bcplus::symbols::SortSymbol const* sort, Context* c);
	
	/// add a translation of a constant declaration as a prereq statement to the provided context
	bool translateConstDeclaration(bcplus::symbols::ConstantSymbol const* constant, Context* c);
};

}}
