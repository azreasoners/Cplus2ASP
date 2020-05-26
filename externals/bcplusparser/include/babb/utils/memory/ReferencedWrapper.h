#pragma once
#include <string>
#include <list>
#include <vector>

#include "babb/utils/memory/Referenced.h"

namespace babb  {
namespace utils {

/// A base wrapper class for creating reference counted classes from non-reference counted ones.
template <typename T>
class ReferencedWrapper : public Referenced, public T
{
	using T::operator=;

public:
	ReferencedWrapper();

	template <typename T2>
	ReferencedWrapper(T2 const& other);

	virtual ~ReferencedWrapper();


	ReferencedWrapper<T>& operator=(ReferencedWrapper<T> const& other);
//	ReferencedWrapper<T>& operator=(T const& other);


};

template <typename T>
ReferencedWrapper<T>::ReferencedWrapper() {
	// Intentionally left blank
}

template <typename T>
template <typename T2>
ReferencedWrapper<T>::ReferencedWrapper(T2 const& other) {
	*this = other;
}


template <typename T>
ReferencedWrapper<T>::~ReferencedWrapper() {
	// Intentionally left blank
}

template <typename T>
ReferencedWrapper<T>& ReferencedWrapper<T>::operator=(ReferencedWrapper<T> const& other) {
	*((T*)(this)) = *((T const*)(&other));
	return *this;
}

/*
template <typename T>
ReferencedWrapper<T>& ReferencedWrapper<T>::operator=(T const& other) {
	*((T*)(this)) = other;
	return *this;
} */

/// Reference counted string
typedef ReferencedWrapper<std::string> ReferencedString;

}}


