#pragma once

/// Common wrapper definitions


#include<list>
#include<vector>
#include<set>
#include<map>
#include<string>

#ifndef NO_BOOST
#include<boost/filesystem/path.hpp>
#endif

#include "babb/utils/memory.h"


typedef babb::utils::ReferencedWrapper<std::string> ReferencedString;

#ifndef NO_BOOST
typedef babb::utils::ReferencedWrapper<boost::filesystem::path> ReferencedPath;
#endif

template <typename T>
struct ReferencedList
{
	typedef babb::utils::ReferencedWrapper<std::list<T> > type;
};


template <typename T>
struct ReferencedVector
{
	typedef babb::utils::ReferencedWrapper<std::vector<T> > type;
};


template <typename T>
struct ReferencedSet
{
	typedef babb::utils::ReferencedWrapper<std::set<T> > type;
};

template <typename T1, typename T2>
struct ReferencedMap
{
	typedef babb::utils::ReferencedWrapper<std::map<T1, T2> > type;
};
