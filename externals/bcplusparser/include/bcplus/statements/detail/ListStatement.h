#pragma once

#include <ostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// template base for definition statements
/// @param type The type of statement this is
/// @param T the symbol type being declared
template <Statement::Type::type type, typename T>
class ListStatement : public Statement
{
public:
	/*******************************************************************************/
	/* Public Types */
	/*******************************************************************************/
	typedef T Element;
	typedef typename ReferencedList<babb::utils::ref_ptr<T> >::type ElementList;
	typedef typename ElementList::iterator iterator;
	typedef typename ElementList::const_iterator const_iterator;

private:
	babb::utils::ref_ptr<const ElementList> _elements;

public:
	/// Basic Constructor
	/// @param elements The list of elements in this statement.
	/// @param begin The beginning location of this statement.
	/// @param end The ending location of this tatement.
	ListStatement(ElementList const* elements, Location const& begin, Location const& end);

	/// Stub destructor
	~ListStatement();


	/// Get the number of macros defined by this statement
	inline size_t size() const						{ return _elements->size(); }

	/// Iterate over the defined macros
	const_iterator begin() const					{ return _elements->begin(); }
	iterator begin()								{ return _elements->begin(); }
	const_iterator end() const						{ return _elements->end(); }
	iterator end()									{ return _elements->end(); }

	// inherited stuffs
	virtual Statement* copy() const;


};

}}}

#include "bcplus/statements/detail/ListStatement.impl.h"

