#pragma once

#include <list>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Configuration.h"
#include "bcplus/parser/Token.h"
#include "bcplus/parser/detail/ScannerSource.h"

namespace bcplus{
namespace parser{
namespace detail{


/// Scanner source encapsulates a stream of scanner tokens 
class ScannerTokenSource : public ScannerSource
{

public:
	/***********************************************************************/
	/* Public Types */
	/***********************************************************************/
	typedef ReferencedList<babb::utils::ref_ptr<Token> >::type TokenList;


	typedef TokenList::iterator iterator;
	typedef TokenList::const_iterator const_iterator;


private:
	/***********************************************************************/
	/* Private Types */
	/***********************************************************************/
	
	/// The token stream
	babb::utils::ref_ptr<TokenList> _stream;

public:

	/***********************************************************************/
	/* Constructors / Destructors */
	/***********************************************************************/
	/// Initalizes the token stream to the provided value.
	/// @param stream The token stream to use for this source.
	ScannerTokenSource(TokenList* stream);

	/// Initializes the token stream with one or no tokens
	/// @param token The token to add to the token stream (or NULL to create an empty stream)
	ScannerTokenSource(Token* token = NULL);

	/// Destructor stub
	virtual ~ScannerTokenSource();


	/***********************************************************************/
	/* Public Methods */
	/***********************************************************************/

	/// injects a token into the stream to be read next
	/// @param token the token to inject
	inline void push_front(Token* token)			{ _stream->push_front(token); }

	/// injects tokens into the stream to be read next
	/// @param tokens the tokens to inject
	void push_front(TokenList* tokens);

	/// Adds a token to the end of the stream to be read last
	/// @param token The token to inject
	inline void push_back(Token* token)				{ _stream->push_back(token); }
	
	/// Adds tokens to the end of the stream to be read last
	/// @param tokens The tokens to inject
	void push_back(TokenList* tokens);

	/// Determines how many tokens are in the stream
	/// @param the number of tokens in the stream
	inline size_t size() const							{ return _stream->size(); }

	/// Pops the next token in the stream and returns it
	/// @return The next token or NULL if the stream is empty
	Token* next();

	/// Peaks at the next token in the stream
	/// @return The next token or NULL if the stream is empty
	Token* peak() const;


	/// Iterates over the token stream
	inline const_iterator begin() const					{ return _stream->begin(); }
	inline iterator begin()								{ return _stream->begin(); }
	inline const_iterator end() const					{ return _stream->end(); }
	inline iterator end() 								{ return _stream->end(); }

	
	// inherited
	virtual Location loc() const;

};

}}}
