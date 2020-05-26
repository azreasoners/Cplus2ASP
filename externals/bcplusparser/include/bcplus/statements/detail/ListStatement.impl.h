#pragma once

#include <ostream>

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/ListStatement.h"

namespace bcplus {
namespace statements {
namespace detail {

template <Statement::Type::type t, typename T>
ListStatement<t, T>::ListStatement(ElementList const* elements, Location const& begin, Location const& end)
	: Statement(t, begin, end), _elements(elements) {
	/* Intentionally left blank */
}

template <Statement::Type::type type, typename T>
ListStatement<type, T>::~ListStatement() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename T>
Statement* ListStatement<t, T>::copy() const {
	return new ListStatement<t, T>(_elements, beginLoc(), endLoc());
}


}}}

