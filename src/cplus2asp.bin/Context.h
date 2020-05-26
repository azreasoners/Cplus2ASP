#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "types.h"

#include "bcplus/symbols/VariableSymbol.h"
#include "Configuration.h"

namespace cplus2asp {
namespace cplus2asp_bin {

/// A translation context container which gives information required during translation.
class Context : public babb::utils::Referenced {
public:
	/*********************************************************************************/
	/* Types */
	/*********************************************************************************/

	/// The various positions we can be working in
	struct Position {
		enum type {
			HEAD, 	///< Head of a rule
			BODY,	///< Body of a rule
			DECL,	///< Inside any type of declaration
			AGGR,	///< Inside an aggregate.
		};
	};


private:
	/*********************************************************************************/
	/* Members */
	/*********************************************************************************/

	/// System wide configuration
	babb::utils::ref_ptr<const Configuration> _config;

	/// The current position
	Position::type _pos;

	/// Whether we're in the scope of negation
	bool _neg;

	/// The timestamp that should be applied to the expression, if any.
	babb::utils::ref_ptr<const ReferencedString> _ts;

	/// A list to append extra clauses to that should be added to the end up the conjuctive subformula (or NULL if they should be ignored)
	babb::utils::ref_ptr<ClauseList> _extraClauses;

	/// A list to add prereq statements to (or NULL if they should be ignored)
	babb::utils::ref_ptr<StatementList> _preStmts;
	
	/// A list of newly created free variables that should be quantified over
	babb::utils::ref_ptr<ClauseList> _existsVars;

	/// A list of variable substitutions to make during the translation
	VariableSubstitutionList _subs;




	/// The active incremental part
	IPart::type _ipart;

public:

	
	/*********************************************************************************/
	/* Constructors / Destructors */
	/*********************************************************************************/

	/// Initialize a fresh context
	/// @param config The system wide configuration.
	/// @param pos The position that the context occurs within
	/// @param ipart The incremental part the translation is operating within.
	/// @param preStatements A list to append prerequisite statements to
	/// @param extraClauses A list to append extra clauses to
	/// @param extraClauses A list to append existentially quantified variables to
	/// @param neg Whether the context occurs within the scope of negation
	/// @param timestamp The timestamp the formula refers to (null defaults to the configuration's DEFAULT or ZERO time depending on ipart).
	Context(Configuration const* config, Position::type pos, IPart::type ipart, StatementList* preStatements = NULL, ClauseList* extraClauses = NULL, ClauseList* existsVars = NULL, bool neg = false, ReferencedString const* timestamp = NULL, VariableSubstitutionList const* subs = NULL);


	/// stub destructor
	~Context();



	/*********************************************************************************/
	/* Child context initializers */
	/*********************************************************************************/

	/// Change the position of the context
	Context* mkPos(Position::type pos, bool negated = false);

	/// Change the position and incremental part of the context
	Context* mkPos(Position::type pos, IPart::type ipart, bool negated = false);

	/// Change the timestamp of the context
	Context* mkTime(ReferencedString const* ts);

	/// Change the clause / variable bindings
	Context* mkBinds(ClauseList* extraClauses, ClauseList* existsList);

	/// Change the statement binding
	Context* mkBindStmts(StatementList* preStatements);


	/// Add variable substitutations to the substitution list.
	Context* mkVarSubstitutions(VariableSubstitutionList const& subs);
	


	/*********************************************************************************/
	/* Public Methods */
	/*********************************************************************************/

	/// Get the system configuration
	inline Configuration const* config()						{ return _config; }

	/// Get the position
	inline Position::type pos() const							{ return _pos; }

	/// Determine if the context is negated
	inline bool neg() const										{ return _neg; }

	/// Determine the timestamp that should be applied
	ReferencedString const* ts() const							{ return _ts; }

	/// Append a clause to the list
	inline void addClause(std::string const& clause)			{ if (_extraClauses) _extraClauses->push_back(clause); }
	
	/// Append a variable to the list of existentially quantified variables
	inline void addExists(std::string const& var)				{ if (_existsVars) _existsVars->push_back(var); }

	/// Append a statement to the list
	inline void addPreStmt(std::string const& stmt, IPart::type ipart)		{ if (_preStmts) _preStmts->push_back(TranslationStatement(stmt, ipart)); }



	/// Get the string which should substitute the provided variable, or NULL if no such string has been defined
	inline ReferencedString const* getVarSubstitution(bcplus::symbols::VariableSymbol const* var) const
																{ VariableSubstitutionList::const_iterator it = _subs.find(var); 
																  return (it != _subs.end() ? (it->second) : NULL); }
	/// Get the active ipart for the translation
	IPart::type ipart() const									{ return _ipart; }
};


}}
