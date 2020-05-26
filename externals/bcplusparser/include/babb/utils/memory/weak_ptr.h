#pragma once

namespace babb  {
namespace utils {

class Referenced;
template <typename T> class ref_ptr;
template <typename T> class unsafe_ptr;


/// Weak pointer for referenced counted memory locations.
/// Automatically set to null when the memory location is deallocated.
/// @param T The type which it's referencing. Should ONLY be descendant of Reference.
template <typename T>
class weak_ptr {
	/// Ensure that T is a descendant of Referenced.
	/// This only works with the C++11 standard.
//	static_assert(std::is_base_of<Referenced, T>::value,
//		"T must be a descendant of Referenced");

private:

	static const bool FALSE;


	/*******************************************************************/
	/* Members */
	/*******************************************************************/

	/// Pointer validity flag
	bool const* _valid;

	/// The real pointer.
	T* _ptr;

public:
	/*******************************************************************/
	/* Constructors */
	/*******************************************************************/

	/// Constructor
	inline weak_ptr(T* ptr = NULL) {
		_ptr = ptr;
		if (ptr) 
			_valid = _ptr->_valid;
		else
			_valid = &FALSE;
	}

	/// Copy constructor
	inline weak_ptr(ref_ptr<T> const& other) {
		_ptr = const_cast<T*>(other.get());
		if (_ptr) 
			_valid = _ptr->_valid;
		else
			_valid = &FALSE;
	}


	/// Destructor
	inline ~weak_ptr() {
		// intentionally left blank
	}

	/*******************************************************************/
	/* Functions */
	/*******************************************************************/
	inline T* get() const				{ return ((*_valid) ? _ptr : NULL); }
	inline bool valid() const 			{ return get() != NULL; }

	// Implicit conversion
	inline operator T*() const 			{ return get(); }

	/*******************************************************************/
	/* Operators */
	/*******************************************************************/
	/// Assignment
	inline weak_ptr<T>& operator=(T* other) {
		if (_ptr == other) return *this;
		_ptr = other;
		if (_ptr) 
			_valid = _ptr->_valid;
		else
			_valid = &FALSE;

		return *this;
	}

	inline weak_ptr<T>& operator=(ref_ptr<T> const& other) {
		return *this = other.get();
	}
	
	inline weak_ptr<T>& operator=(weak_ptr<T> const& other) {
		return *this = other.get();
	}
	
	inline weak_ptr<T>& operator=(unsafe_ptr<T> const& other) {
		return *this = other.get();
	}

	/*
	inline bool operator==(ref_ptr<T> const& other) {
		return _ptr == other._ptr;
	} */

	/// Dereference
	inline T& operator*() const {
		return *get();
	}

	inline T* operator->() const {
		return get();
	}

private:

};

template <typename T>
const bool weak_ptr<T>::FALSE = false;

}}
