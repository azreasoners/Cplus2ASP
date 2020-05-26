#pragma once

#include <string>
#include <iostream>

#include "babb/utils/memory.h"

namespace as2transition  {
namespace parser {


class Scanner : public babb::utils::Referenced {
public:
	/***********************************************************/
	/* Public Types */
	/***********************************************************/

private:

	/***********************************************************/
	/* Private Types */
	/***********************************************************/
	/// Condition type for the scanner
	/// NOTE: Order matters.
	enum YYCONDTYPE {
		yycNORMAL,						///< Standard condition 
		yycDBL_STRING,					///< Inside a doubel quoted string
		yycSGL_STRING					///< Inside a single quoted string
	};

	/***********************************************************/
	/* Private Members */
	/***********************************************************/

	/// Input source
	std::istream& _source;

	/// Whether we should stop at newline
	bool _stopnewline;

	/// Scanner buffer information
	char* _buffer;
	size_t _buff_sz;

	
	/// Scanner state information
	YYCONDTYPE _cond;
	char const* _cursor;
	char const* _limit;
	char const* _marker;
	char const* _token;
	bool _newline;

public:
	/***********************************************************/
	/* Constructors */
	/***********************************************************/
	/// @param source The input source for the scanner
	/// @param stopnewline Whether we should stop at a newline
	Scanner(std::istream& source, bool stopnewline);

	/// Destructor
	virtual ~Scanner();

	/***********************************************************/
	/* Public Functions */
	/***********************************************************/

	/// Reads a token from the input stream.
	/// @return The type of token that was read
	/// @param[out] token The token that was read in.
	int readToken(ReferencedString*& token);

	/// Determines if the scanner has reached the end of its input
	inline bool eof()			{ return (_cursor ? !*_cursor : _source.eof()); }

	/// Determines if the scanner is in an error state
	inline bool good()			{ return _source.good(); }

private:

	/// Internal call to fill the Scanner's buffer 
	void fill(size_t sz);


};

}}

