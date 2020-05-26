#pragma once
#include "babb/utils/memory.h"
#include "bcplus/Location.h"

namespace bcplus {
namespace parser {

/// A simple class to track an integral token
class Number : public babb::utils::Referenced
{
private:
	/************************************************************/
	/* Private Members */
	/************************************************************/

	/// The token's value
	int _val;

	/// Beginning locaiton of the numeric token
	Location _begin;

	/// Ending location of the numeric token
	Location _end;

public:
	/************************************************************/
	/* Constructors / Destructors */
	/************************************************************/
	Number(int val, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0));

	virtual ~Number();


	/************************************************************/
	/* Public Members */
	/************************************************************/

	/// Get the value of the token
	inline int val() const						{ return _val; }
	

	/// Get/set the location of the token
	inline Location const& beginLoc() const		{ return _begin; }
	inline Location const& endLoc() const			{ return _end; }
	
	inline void beginLoc(Location const& v)		{ _begin = v; }
	inline void endLoc(Location const& v) 			{ _end = v; }


};


}}
