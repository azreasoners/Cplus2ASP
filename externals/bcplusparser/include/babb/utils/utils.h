#pragma once

#include <boost/lexical_cast.hpp>

namespace babb {
namespace utils {

/// String conversion
template <typename T>
inline bool fromString(char const* str, T& val) {
	try {
		val = boost::lexical_cast<T>(str);
		return true;
	} catch (boost::bad_lexical_cast const&) {
		return false;
	}
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
