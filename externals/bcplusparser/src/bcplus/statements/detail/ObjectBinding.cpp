#include <ostream>

#include <boost/foreach.hpp>

#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/statements/detail/ObjectBinding.h"

namespace bcplus {
namespace statements {
namespace detail {

ObjectBinding::ObjectBinding(symbols::SortSymbol* sort, ObjectList* objects)
	: _sort(sort) {
	if (objects) _objects = objects;
	else _objects = new ObjectList();
} 

ObjectBinding::~ObjectBinding() {
	// Intentionally left blank
}

}}}

