	// Intentionally left blank
#pragma once
#include <list>
#include <iostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "as2transition/Predicate.h"
#include "as2transition/parser/Scanner.h"


namespace as2transition {
namespace parser {

/// A parser which allows us to scan a list of predicates
class PredicateParser : public babb::utils::Referenced {
public:
	/***********************************************************************************/
	/* Types */
	/***********************************************************************************/
	/// Container for status codes
	struct Status {
		enum type {
			READY,			///< Everything is fine and dandy
			FINISHED,		///< Done parsing
			SYNTAX_ERR,		///< A syntax error has occurred
		};
	};


	typedef std::list<babb::utils::ref_ptr<Predicate> > PredicateList;
	typedef PredicateList::iterator iterator;
	typedef PredicateList::const_iterator const_iterator; 


private:
	/***********************************************************************************/
	/* Members */
	/***********************************************************************************/

	/// The scanner object we're reading from
	babb::utils::ref_ptr<Scanner> _scanner;

	/// The lemon internal parser object
	void* _parser;

	/// Storage for the predicates that have been parsed
	PredicateList _predicates;	

	/// Our current parse status
	Status::type _stat;

	/// Last token read in
	std::string _last_token;

public:
	/***********************************************************************************/
	/* Constructors */
	/***********************************************************************************/
	/// @param input The input stream to read from
	/// @param stopnewline Whether to stop reading the stream at a newline
	PredicateParser(std::istream& input, bool stopnewline);

	/// Destructor
	virtual ~PredicateParser();

	/***********************************************************************************/
	/* Public Functions */
	/***********************************************************************************/

	/// Attempts to parser the predicate list
	/// @return A status code indicating whether it was successful
	Status::type parse();

	/// Get the number of parsed predicates
	inline size_t size() const								{ return _predicates.size(); }

	/// Iterate through the list of parsed predicates
	inline iterator begin()									{ return _predicates.begin(); }
	inline const_iterator begin() const						{ return _predicates.begin(); }
	inline iterator end()									{ return _predicates.end(); }
	inline const_iterator end() const						{ return _predicates.end(); }


	/***********************************************************************************/
	/* Stuff Thou Shall not Call */
	/***********************************************************************************/

	/// INTERNAL FUNCTION
	/// Adds a freshly parsed predicate to the list
	/// @param pred The predicate to add
	inline void _add(Predicate* pred)						{ 
		_predicates.push_back(pred); 
	}

	/// INTERNAL FUNCTION
	/// Registers a parse error.
	/// @param err The error to register
	void _parse_error(std::string const& err);

};


}}

