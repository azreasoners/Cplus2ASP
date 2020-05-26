#pragma once

//#include <type_traits>

namespace babb  {
namespace utils {

class Referenced;

template <typename T> class ref_ptr;
template <typename T> class weak_ptr;

/// A lighter version of ref_ptr that can be used in unions (no constructor/destructor)
/// This pointer assumes that a value is assigned when useage begins and NULL is assigned before usage ends.
/// @param T The type which it's referencing. Should ONLY be descendant of Reference.
template <typename T>
class unsafe_ptr {
	/// Ensure that T is a descendant of Referenced.
	/// This only works with the C++11 standard.
//	static_assert(std::is_base_of<Referenced, T>::value,
//		"T must be a descendant of Referenced");


private:
	/*******************************************************************/
	/* Members */
	/*******************************************************************/

	/// The real pointer.
	T* _ptr;

public:

	/*******************************************************************/
	/* Functions */
	/*******************************************************************/
	inline T* get() const 				{ return _ptr; }
	inline bool valid() const 			{ return _ptr != NULL; }

	// Implicit conversion
	inline operator T*() const { return _ptr; }

	/// Release the memory location from this pointer without deallocating it
	inline T* release() {
		if (!_ptr) return NULL;
		_ptr->_unref();
		_ptr = NULL;
	}


	/*******************************************************************/
	/* Operators */
	/*******************************************************************/
	/// Assignment
	inline unsafe_ptr<T>& operator=(T* other) {
		if (_ptr == other) return *this;

		//  Make sure to remove the reference from the current pointer.
		if (_ptr && !_ptr->_unref()) {
			delete _ptr;
		}

		_ptr = other;
		if (_ptr) _ptr->_ref();
		return *this;
	}

	inline unsafe_ptr<T>& operator=(ref_ptr<T> const& other) {
		return *this = other.get();
	}
	
	inline unsafe_ptr<T>& operator=(weak_ptr<T> const& other) {
		return *this = other.get();
	}
	
	inline unsafe_ptr<T>& operator=(unsafe_ptr<T> const& other) {
		return *this = other.get();
	}

	/*
	inline bool operator==(ref_ptr<T> const& other) {
		return _ptr == other._ptr;
	} */

	/// Dereference
	inline T& operator*() const {
		return *_ptr;
	}

	inline T* operator->() const {
		return _ptr;
	}

};

}}
