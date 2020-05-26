#pragma once

#include <ostream>

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/SimpleStatement.h"

namespace bcplus {
namespace statements {
namespace detail {

template <Statement::Type::type t, typename value_t, typename valuearg_t>
SimpleStatement<t, value_t, valuearg_t>::SimpleStatement(valuearg_t value, Location const& begin, Location const& end)
	: Statement(t, begin, end), _value(value) { 
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename value_t, typename valuearg_t>
SimpleStatement<t, value_t, valuearg_t>::~SimpleStatement() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename value_t, typename valuearg_t>
Statement* SimpleStatement<t, value_t, valuearg_t>::copy() const {
	return new SimpleStatement<t, value_t, valuearg_t>(value(), beginLoc(), endLoc());
}


}}}

