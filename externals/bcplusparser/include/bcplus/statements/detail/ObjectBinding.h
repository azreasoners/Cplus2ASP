#pragma once
#include <ostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/symbols/ObjectSymbol.h"
#include "bcplus/symbols/SortSymbol.h"

namespace bcplus {
namespace statements {
namespace detail {

/// Binding of objects to a sort (o1, o2, o3 :: sort)
class ObjectBinding : public babb::utils::Referenced {
public:
	/***********************************************************************/
	/* Public Types */
	/***********************************************************************/
	typedef ReferencedList<babb::utils::ref_ptr<const symbols::Symbol> >::type ObjectList;
	typedef ObjectList::iterator iterator;
	typedef ObjectList::const_iterator const_iterator;
	

private:
	/***********************************************************************/
	/* Private Members */
	/***********************************************************************/

	/// The objects being bound to the sort
	babb::utils::ref_ptr<ObjectList> _objects;
	
	/// The sort the objects are being added to
	babb::utils::ref_ptr<symbols::SortSymbol> _sort;

public:
	/***********************************************************************/
	/* Constructors / Destructors */
	/***********************************************************************/
	/// Initializes the binding
	/// @param sort The sort that the objects will be added to.
	/// @param objects The list of objects to be added (or NULL to create an empty list).
	ObjectBinding(symbols::SortSymbol* sort, ObjectList* objects = NULL);

	/// Destructor stub
	virtual ~ObjectBinding();


	/// Get the sort
	inline symbols::SortSymbol* sort() 							{ return _sort; }
	inline symbols::SortSymbol const* sort() const				{ return _sort; }

	/// Get the number of objects being bound
	inline size_t size() const									{ return _objects->size(); }

	/// Iterate over the object being bound
	iterator begin() 											{ return _objects->begin(); }
	const_iterator begin() const								{ return _objects->begin(); }
	iterator end() 												{ return _objects->end(); }
	const_iterator end() const									{ return _objects->end(); }

	/// Add an object to the binding
	inline void push_back(symbols::Symbol const* obj)		{ _objects->push_back(obj); }
	inline void push_front(symbols::Symbol const* obj)	{ _objects->push_front(obj); }

};

}}}

