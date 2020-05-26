#pragma once

#include <ostream>

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/VoidStatement.h"

namespace bcplus {
namespace statements {
namespace detail {

template <Statement::Type::type t>
VoidStatement<t>::VoidStatement(Location const& begin, Location const& end)
	: Statement(t, begin, end) {
	/* Intentionally left blank */
}

template <Statement::Type::type type>
VoidStatement<type>::~VoidStatement() {
	/* Intentionally left blank */
}

template <Statement::Type::type t>
Statement* VoidStatement<t>::copy() const {
	return new VoidStatement<t>(beginLoc(), endLoc());
}


}}}

