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
#include "bcplus/parser/detail/Scanner.h"
#include "bcplus/statements/declarations.h"
#include "bcplus/symbols/SymbolTable.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/Macro.h"

namespace bcplus {
namespace parser {
namespace detail {

/// A parser which allows us to scan a BC+ program
class MacroParser : public babb::utils::Referenced {
public:
	/***********************************************************************************/
	/* Types */
	/***********************************************************************************/
	struct Status {
		enum type {
			OK,
			ERR_IO,
			ERR_SYNTAX,
			END_INPUT
		};
	};


	/// The type returned by the parse function
	typedef ReferencedList<babb::utils::ref_ptr<Token> >::type TokenBuffer;
private:
	/***********************************************************************************/
	/* Members */
	/***********************************************************************************/

	/// System wide configuration information
	babb::utils::ref_ptr<const Configuration> _config;

	/// The scanner object we're reading from
	babb::utils::ref_ptr<detail::Scanner> _scanner;

	/// The symbol table 
	babb::utils::ref_ptr<symbols::SymbolTable> _symtab;

	/// Information about the language we're parsing
	babb::utils::ref_ptr<const languages::Language> _lang;

	/// The lemon internal parser object
	void* _parser;

	/// Temporary buffer for comments that have been parsed
	TokenBuffer _comments;	

	/// Temporary storage for tokens that have been parsed
	TokenBuffer _tokens;

	/// Our current parser status
	Status::type _stat;

	/// The last token we passed to lemon (or NULL)
	babb::utils::weak_ptr<const Token> _last_token;

public:
	/***********************************************************************************/
	/* Constructors */
	/***********************************************************************************/
	/// @param config The system-wide configuration information 
	/// @param lang The language specification that we will be enforcing
	/// @param symtab The symbol table to use, or NULL to create one using the configuration options.
	MacroParser(Configuration const* config, languages::Language const* lang, symbols::SymbolTable* symtab = NULL);

	/// Destructor
	virtual ~MacroParser();

	/***********************************************************************************/
	/* Public Functions */
	/***********************************************************************************/

	/// Attempts to parse the next non-comment token, resolving all macros and include statements in the process.
	/// @return The parsed token (if any).
	Token const* next();

	/// Determines if there is a comment token waiting, if there is it returns it
	Token const* comment();

	/// Resets the parser clearing all previous input
	void reset();


	/// Gets the symbol table for the parser
	inline symbols::SymbolTable* symtab() 						{ return _symtab; }
	inline symbols::SymbolTable const* symtab() const			{ return _symtab; }

	/// Get the language specification being enforced.
	inline languages::Language const* lang() const				{ return _lang; }
 
	/// Get the configuration information
	inline Configuration const* config() const					{ return _config; }

	/// Attempts to open the provided file and append it to the end of the stream being read by the scanner.
    /// @param file The file to read from.
    /// @param squelch Whether to silence all errors resulting from this call.
    /// @return True if successful, false otherwise.
    bool push_back(boost::filesystem::path const& file, bool squelch = false);

	/// Appends the provided null-terminated buffer to the end of the stream being read by the scanner.
    /// @param buffer The null-terminated buffer to read from.
    /// @param loc The location to display in messages partaining to this buffer.
	/// @param track Whether to track the position offset of each token within the buffer.
    void push_back(char const* buffer, Location const& loc = Location(NULL, 0, 0), bool track = false);

	/// Appends the provided token to the end of the stream being read by the scanner.
    /// @param token The token to append.
    void push_back(Token* token);

	/***********************************************************************************/
	/* Stuff Thou Shall not Call */
	/***********************************************************************************/

	/// INTERNAL FUNCTION
	/// Reports the use of an unsupported feature.
	/// @param feature The unsupported feature that was used.
	/// @param loc The location of the error
	void _feature_error(languages::Language::Feature::type feature, Location const* loc = NULL);

	/// INTERNAL FUNCTION
	/// Registers a parse error.
	/// @param err The error to register
	/// @param loc The location of the error
	void _parse_error(std::string const& err, Location const* loc = NULL);

	/// callback for the lemon parser to provide another token to the post-parse token stream.
	void _token(Token* token);

	/// callback for the lemon parser to handle macro expansion
	void _handle_macro(elements::Macro const* macro);

	/// callback for the lemon parser to handle file inclusion
	void _handle_include(statements::IncludeStatement const* incl);	

	/// callback for the lemon parser to handle macro definitions
	void _handle_macro_def(statements::MacroDeclaration* decl);

	/// Attempts to open the provided file and append it to the beginning of the input stream being read from.
	/// @param file The file to read from.
	/// @param squelch Whether to silence all errors resulting from this call.
	/// @return True if successful, false otherwise.
	bool _push_front(boost::filesystem::path const& file, bool squelch = false);

	/// Appends the provided null-terminated buffer to the beginning of the inptu stream being read from.
	/// @param buffer The null-terminated buffer to read from.
	/// @param loc The location to display in messages partaining to this buffer.
	/// @param track Whether to track the position offset of each token within the buffer.
	void _push_front(char const* buffer, Location const& loc = Location(NULL, 0, 0), bool track = false);

	/// Appends the provided token to beginning of the input stream being read from.
	/// @param token The token to append.
	void _push_front(Token* token);

private:

	/// Prepares the parser for an input to be injected at its current location
	/// @param popState whether the top of the parser's state stack should be popped in the process.
	void preInjectPrep(bool popState = false);


};


}}}


