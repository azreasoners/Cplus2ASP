#pragma once

//#include <type_traits>

namespace babb  {
namespace utils {

class Referenced;
template <typename T> class weak_ptr;
template <typename T> class unsafe_ptr;

/// Referenced counted pointer class.
/// @param T The type which it's referencing. Should ONLY be descendant of Reference.
template <typename T>
class ref_ptr {
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
	/* Constructors */
	/*******************************************************************/

	/// Constructor
	inline ref_ptr(T* ptr = NULL) {
		_ptr = ptr;
		if (ptr) ptr->_ref();
	}

	/// Copy constructor
	inline ref_ptr(ref_ptr<T> const& other) {
		_ptr = const_cast<T*>(other.get());
		if (_ptr) _ptr->_ref();
	}


	/// Destructor
	inline ~ref_ptr() {
		if (_ptr && !_ptr->_unref()) {
			delete _ptr;
		}
	}

	/*******************************************************************/
	/* Functions */
	/*******************************************************************/
	inline T* get() const 				{ return _ptr; }
	inline bool valid() const 			{ return _ptr != NULL; }

	// Implicit conversion
	inline operator T*() { return _ptr; }
	inline operator T const*() const { return _ptr; }

	/// Release the memory location from this pointer without deallocating it
	inline T* release() {
		if (!_ptr) return NULL;
		T* p = _ptr;
		_ptr->_unref();
		_ptr = NULL;
		return p;
	}


	/*******************************************************************/
	/* Operators */
	/*******************************************************************/
	/// Assignment
	inline ref_ptr<T>& operator=(T* other) {
		if (_ptr == other) return *this;

		//  Make sure to remove the reference from the current pointer.
		if (_ptr && !_ptr->_unref()) {
			delete _ptr;
		}

		_ptr = other;
		if (_ptr) _ptr->_ref();
		return *this;
	}

	inline ref_ptr<T>& operator=(ref_ptr<T> const& other) {
		return *this = other.get();
	}
	
	inline ref_ptr<T>& operator=(weak_ptr<T> const& other) {
		return *this = other.get();
	}

	inline ref_ptr<T>& operator=(unsafe_ptr<T> const& other) {
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
