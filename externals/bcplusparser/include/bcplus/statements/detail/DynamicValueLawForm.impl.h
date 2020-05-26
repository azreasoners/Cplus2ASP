#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/DynamicValueLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename body_t, typename head_t, typename value_t, typename ifbody_t, typename unless_t, typename where_t>
DynamicValueLawForm<t, body_t, head_t, value_t, ifbody_t,  unless_t, where_t>::DynamicValueLawForm(body_t const* body, head_t const* head, value_t const* value, ifbody_t const* ifbody, unless_t const* unless, where_t const* where,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _body(body), _head(head), _value(value), _ifbody(ifbody), _unless(unless), _where(where) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename head_t, typename value_t, typename ifbody_t, typename unless_t, typename where_t>
DynamicValueLawForm<t, body_t, head_t, value_t, ifbody_t,  unless_t, where_t>::~DynamicValueLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename head_t, typename value_t, typename ifbody_t, typename unless_t, typename where_t>
Statement* DynamicValueLawForm<t, body_t, head_t, value_t, ifbody_t,  unless_t, where_t>::copy() const {
	return new DynamicValueLawForm<t, body_t, head_t, value_t, ifbody_t,  unless_t, where_t>(body(), head(), value(), ifbody(), unless(), where(), beginLoc(), endLoc());
}

}}}

