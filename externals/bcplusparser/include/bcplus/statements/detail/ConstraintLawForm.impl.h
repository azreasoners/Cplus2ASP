#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/ConstraintLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename body_t, typename after_t, typename unless_t, typename where_t>
ConstraintLawForm<t, body_t, after_t, unless_t, where_t>::ConstraintLawForm(body_t const* body, 
	after_t const* after, unless_t const* unless, where_t const* where,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _body(body), _after(after), _unless(unless), _where(where) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename after_t, typename unless_t, typename where_t>
ConstraintLawForm<t, body_t, after_t, unless_t, where_t>::~ConstraintLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename after_t, typename unless_t, typename where_t>
Statement* ConstraintLawForm<t, body_t, after_t, unless_t, where_t>::copy() const {
	return new ConstraintLawForm<t, body_t, after_t, unless_t, where_t>(body(), after(), unless(), where(), beginLoc(), endLoc());
}

}}}

