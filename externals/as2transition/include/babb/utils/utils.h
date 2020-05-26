#pragma once

#ifndef NO_BOOST
#include <boost/lexical_cast.hpp>
#else
#include <sstream>
#endif

namespace babb {
namespace utils {

/// String conversion
template <typename T>
inline bool fromString(char const* str, T& val) {
#ifndef NO_BOOST
	try {
		val = boost::lexical_cast<T>(str);
		return true;
	} catch (boost::bad_lexical_cast const&) {
		return false;
	}
#else
	std::stringstream tmp;
	tmp << str;
	tmp >> val;
	if (tmp.fail()) return false;
	else return true;
#endif
}

/// Dereference less-than comparator for using pointer-like objects in sets
template <typename T>
class ptr_less
{
public:
	bool operator()(T const& a, T const& b) const {
		return (*a) < (*b);
	}
};


}}
