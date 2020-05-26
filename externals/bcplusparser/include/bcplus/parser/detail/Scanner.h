#pragma once

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "babb/utils/memory.h"

#include "bcplus/Location.h"
#include "bcplus/parser/Token.h"
#include "bcplus/parser/detail/ScannerSource.h"
#include "bcplus/parser/detail/ScannerTokenSource.h"
#include "bcplus/parser/detail/ScannerRawSource.h"

namespace bcplus  {
namespace parser  {
namespace detail {


class Scanner : public babb::utils::Referenced {
public:
	/***********************************************************/
	/* Public Types */
	/***********************************************************/
	/// A list of tokens.
	typedef ScannerTokenSource::TokenList TokenList;


private:

	/***********************************************************/
	/* Private Types */
	/***********************************************************/
	/// Condition type for the scanner
	/// NOTE: Order matters.
	enum YYCONDTYPE {
		yycNORMAL,						///< Standard condition 
		yycDBL_STRING,					///< Inside a doubel quoted string
		yycSGL_STRING,					///< Inside a single quoted string
		yycASP_GR,						///< Gringo style ASP block
		yycASP_CP,						///< CCalc style ASP block
		yycLUA_GR,						///< Gringo style LUA block
		yycLUA_CP,						///< CCalc style LUA block
		yycPYTHON_GR,					///< Gringo style python block
		yycPYTHON_CP,					///< CCalc style python block
		yycF2LP_GR,						///< Gringo style F2LP block
		yycF2LP_CP,						///< CCalc style F2LP block
		yycPRE_ARG,						///< Intermediate state after we parsed an identifier and are about to parse "("
		yycARG,							///< Inside an unparsed argument
		yycMACRO,						///< Macro definition string (anything following "->" and preceding ".").
		yycBLK_COMMENT,					///< Inside a block comment
		yycCOMMENT						///< Inside a line comment
	};

	/***********************************************************/
	/* Private Members */
	/***********************************************************/

	/// Input sources
	std::list<babb::utils::ref_ptr<ScannerSource> > _sources;

	/// Scanner State
	YYCONDTYPE _cond;

	/// System-wide configuration information
	babb::utils::ref_ptr<const Configuration> _config;

	/// Whether we're scanning a macro definition
	bool _in_macro;

public:
	/***********************************************************/
	/* Constructors */
	/***********************************************************/
	/// @param config System wide configuration information
	Scanner(Configuration const* config);

	/// Destructor
	virtual ~Scanner();

	/***********************************************************/
	/* Public Functions */
	/***********************************************************/

	/// Attempts to open the provided file and inject it into the front of the stream being read by the scanner.
	/// @param file The file to read from.
	/// @param squelch Whether to silence all errors resulting from this call.
	/// @return True if successful, false otherwise.
	bool push_front(boost::filesystem::path const& file, bool squelch = false);

	/// Attempts to open the provided file and append it to the end of the stream being read by the scanner.
	/// @param file The file to read from.
	/// @param squelch Whether to silence all errors resulting from this call.
	/// @return True if successful, false otherwise.
	bool push_back(boost::filesystem::path const& file, bool squelch = false);

	/// Injects the provided null-terminated buffer into the front of the stream being read by the scanner.
	/// @param buffer The null-terminated buffer to read from.
	/// @param loc The location to display in messages partaining to this buffer.
	/// @param track_position Whether the position should be tracked within this buffer (starting from {@ref loc})
	void push_front(char const* buffer, Location const& loc = Location(NULL, 0, 0), bool track_position = false);
	
	/// Appends the provided null-terminated buffer to the end of the stream being read by the scanner.
	/// @param buffer The null-terminated buffer to read from.
	/// @param loc The location to display in messages partaining to this buffer.
	/// @param track_position Whether the position should be tracked within this buffer (starting from {@ref loc})
	void push_back(char const* buffer, Location const& loc = Location(NULL, 0, 0), bool track_position = false);

	/// Injects the provided token into the from of the stream being read by the scanner
	/// @param token The token to inject
	void push_front(Token* token);
	
	/// Appends the provided token to the end of the stream being read by the scanner
	/// @param token The token to inject
	void push_back(Token* token);

	/// Injects the provided tokens into the from of the stream being read by the scanner
	/// @param tokens The tokens to inject
	void push_front(TokenList* tokens);

	/// Appends the provided token to the end of the stream being read by the scanner
	/// @param tokens The tokens to inject
	void push_back(TokenList* tokens);

	/// Gets the number of input streams associated with this scanner
	inline size_t size() const									{ return _sources.size(); }

	/// Reads a token from the currently open file.
	/// @return The token that was read in.
	Token* readToken();

	/// Gets the current location of the scanner
	inline Location loc() const									{ return (_sources.size() ? _sources.front()->loc() : Location(NULL, 0, 0)); }

	/// Closes all streams open by the scanner and resets its state
	void close();

private:

	/// Tokenizes the input from the provided raw source
	/// @param source The source input to tokenize
	Token* tokenize(ScannerRawSource* source);
	
	/// Internal tokenize helper for tokenizing something that isn't a comment.
	/// @param source The source input to tokenize
	Token* tokenize_2(ScannerRawSource* source);
	
	/// Internal tokenize helper for tokenizing something that isn't a comment or keyword.
	/// @param source The source input to tokenize
	Token* tokenize_3(ScannerRawSource* source);

};

}}}

