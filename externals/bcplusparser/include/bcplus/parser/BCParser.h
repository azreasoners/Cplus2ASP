	// Intentionally left blank
#pragma once
#include <list>
#include <set>
#include <map>
#include <iostream>

#include <boost/filesystem.hpp>

#include "babb/utils/memory.h"



#include "bcplus/Configuration.h"
#include "bcplus/languages/Language.h"
#include "bcplus/parser/detail/MacroParser.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/symbols/SymbolTable.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/terms.h"
#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/NumberRangeSymbol.h"

namespace bcplus {
namespace parser {

/// A parser which allows us to scan a BC+ program
class BCParser : public babb::utils::Referenced {
public:
	/***********************************************************************************/
	/* Types */
	/***********************************************************************************/
	/// Container for status codes
	struct Status {
		enum type {
			OK,				///< Everything is fine and dandy
			SYNTAX_ERR,		///< A syntax error has occurred
			IO_ERR,
			END_INPUT		///< The end of input has been reached by the parser. No more input may be accepted.
		};
	};

	/// The type returned by the parse function
	typedef std::pair<Status::type, babb::utils::ref_ptr<statements::Statement> > ParseType;

private:
	/***********************************************************************************/
	/* Members */
	/***********************************************************************************/

	/// System wide configuration information
	babb::utils::ref_ptr<const Configuration> _config;

	/// The scanner object we're reading from
	babb::utils::ref_ptr<detail::MacroParser> _scanner;

	/// The symbol table 
	babb::utils::ref_ptr<symbols::SymbolTable> _symtab;

	/// Information about the language we're parsing
	babb::utils::ref_ptr<const languages::Language> _lang;

	/// The lemon internal parser object
	void* _parser;

	/// Temporary storage for a statement that's been parsed
	babb::utils::ref_ptr<statements::Statement> _stmt;

	/// Our current parse status
	Status::type _stat;

	/// Whether we've just encountered a soft end of file
	bool _soft_eof;

	/// The last token we passed to lemon (or NULL)
	babb::utils::weak_ptr<const Token> _last_token;

	/// The number of dynamic ranges that we've encountered.
	size_t _rangeCount;


public:
	/***********************************************************************************/
	/* Constructors */
	/***********************************************************************************/
	/// @param config The system-wide configuration information 
	/// @param lang The language specification that we will be enforcing
	/// @param symtab The symbol table to use, or NULL to create one using the configuration options.
	BCParser(Configuration const* config, languages::Language const* lang, symbols::SymbolTable* symtab = NULL);

	/// Destructor
	virtual ~BCParser();

	/***********************************************************************************/
	/* Public Functions */
	/***********************************************************************************/

	/// Attempts to parse the next rule from the scanner.
	/// @return A status code and the parsed rule if the status is OK, or NULL otherwise.
	ParseType parse();

	/// Resets the parser clearing all previous input
	void reset();

    /// Attempts to open the provided file and append it to the end of the stream being read by the scanner.
    /// @param file The file to read from.
    /// @param squelch Whether to silence all errors resulting from this call.
    /// @return True if successful, false otherwise.
    bool push_back(boost::filesystem::path const& file, bool squelch = false);

    /// Appends the provided null-terminated buffer to the end of the stream being read by the scanner.
    /// @param buffer The null-terminated buffer to read from.
    /// @param loc The location to display in messages partaining to this buffer.
    void push_back(char const* buffer, Location const& loc = Location(NULL, 0, 0));

	/// Gets the symbol table for the parser
	inline symbols::SymbolTable* symtab()						{ return _symtab; }
	inline symbols::SymbolTable const* symtab() const			{ return _symtab; }

	/// Get the language specification being enforced.
	inline languages::Language const* lang() const				{ return _lang; }
 
	/// Get the configuration information
	inline Configuration const* config() const					{ return _config; }

	/***********************************************************************************/
	/* Stuff Thou Shall not Call */
	/***********************************************************************************/

	/// INTERNAL FUNCTION
	/// Reports the use of an unsupported feature.
	/// @param feature The unsupported feature that was used.
	/// @param loc The location of the error.
	void _feature_error(languages::Language::Feature::type feature, Location const* loc = NULL);

	/// INTERNAL FUNCTION
	/// Registers a parse error.
	/// @param err The error to register
	/// @param loc The location of the error.
	void _parse_error(std::string const& err, Location const* loc = NULL);

	void _handle_stmt(statements::Statement* stmt);	

	/// INTERNAL FUNCTION
	/// Creates a new sort containing a range provided by the min and max terms
	symbols::SortSymbol* _newRange(elements::Term* min, elements::Term* max);

	/// Creates a new symbol containing a range provided by the min and max terms
	symbols::NumberRangeSymbol* _newRangeSymbol(elements::Term* min, elements::Term* max);



private:
	/// Makes a comment statement if there are comments waiting in the macro parser's buffer
	/// @return The comment statement made from the comments, or NULL if there are no comments.
	statements::Statement* makeCommentStmt();

};


}}

