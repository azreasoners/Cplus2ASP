#include "babb/utils/memory.h"
#include "bcplus/Location.h"
#include "bcplus/elements/terms.h"


namespace bcplus {
namespace parser {
namespace detail {

/// A simple class to track an integral token
class NumberRangeEval : public babb::utils::Referenced
{
private:
	/************************************************************/
	/* Private Members */
	/************************************************************/

	/// The token's minimum and maximum values
	int _min, _max;

	/// Beginning locaiton of the numeric token
	Location _begin;

	/// Ending location of the numeric token
	Location _end;

public:
	/************************************************************/
	/* Constructors / Destructors */
	/************************************************************/
	NumberRangeEval(int min, int max, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0));

	virtual ~NumberRangeEval();


	/************************************************************/
	/* Public Members */
	/************************************************************/

	/// Get the value of the token
	inline int min()				{ return _min; }
	inline int max()				{ return _max; }
	

	/// Get/set the location of the token
	inline Location const& beginLoc() const		{ return _begin; }
	inline Location const& endLoc() const		{ return _end; }
	
	inline void beginLoc(Location const& v)		{ _begin = v; }
	inline void endLoc(Location const& v) 		{ _end = v; }


};


}}}
