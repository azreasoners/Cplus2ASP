#pragma once
#include <list>

#include <boost/utility.hpp>

namespace babb {
namespace utils {

template <typename T> class ref_ptr;
template <typename T> class weak_ptr;
template <typename T> class unsafe_ptr;


/// Simple baseclass which stores a reference counter for the object and automatically deallocates it.
/// Inspired by OpenSceneGraph
class Referenced : public boost::noncopyable {

	template <typename T> friend class ref_ptr;
	template <typename T> friend class weak_ptr;
	template <typename T> friend class unsafe_ptr;

private:
	/***********************************************************/
	/* Private Members */
	/***********************************************************/
	
	/// The validity flag for this reference
	bool* _valid;

	/// Reference count.
	mutable size_t _refs;

public:
	/***********************************************************/
	/* Constructors */
	/***********************************************************/
	inline Referenced()
		: _refs(0)
	{ 
		static std::list<bool> validList;
		validList.push_back(true);
		_valid = &validList.back();
	}


protected:
	inline virtual ~Referenced() {
		*_valid = false;
	}


private:
	/***********************************************************/
	/* Private Functions */
	/***********************************************************/

	inline size_t _ref() const { return ++_refs; }
	inline size_t _unref() const { return --_refs; }

};

}}
