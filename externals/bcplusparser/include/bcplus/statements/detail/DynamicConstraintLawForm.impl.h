#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/DynamicConstraintLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename body_t, typename ifbody_t, typename unless_t, typename where_t>
DynamicConstraintLawForm<t, body_t, ifbody_t, unless_t, where_t>::DynamicConstraintLawForm(body_t const* body, 
	ifbody_t const* ifbody, unless_t const* unless, where_t const* where,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _body(body), _ifbody(ifbody), _unless(unless), _where(where) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename ifbody_t, typename unless_t, typename where_t>
DynamicConstraintLawForm<t, body_t, ifbody_t, unless_t, where_t>::~DynamicConstraintLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename ifbody_t, typename unless_t, typename where_t>
Statement* DynamicConstraintLawForm<t, body_t, ifbody_t, unless_t, where_t>::copy() const {
	return new DynamicConstraintLawForm<t, body_t, ifbody_t, unless_t, where_t>(body(), ifbody(), unless(), where(), beginLoc(), endLoc());
}

}}}

