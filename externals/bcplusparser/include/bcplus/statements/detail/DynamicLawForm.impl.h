#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/DynamicLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename body_t, typename head_t, typename ifbody_t, typename unless_t, typename where_t>
DynamicLawForm<t, body_t, head_t, ifbody_t,  unless_t, where_t>::DynamicLawForm(body_t const* body, head_t const* head, ifbody_t const* ifbody, 
	unless_t const* unless, where_t const* where,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _body(body), _head(head), _ifbody(ifbody), _unless(unless), _where(where) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename head_t, typename ifbody_t, typename unless_t, typename where_t>
DynamicLawForm<t, body_t, head_t, ifbody_t,  unless_t, where_t>::~DynamicLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename body_t, typename head_t, typename ifbody_t, typename unless_t, typename where_t>
Statement* DynamicLawForm<t, body_t, head_t, ifbody_t,  unless_t, where_t>::copy() const {
	return new DynamicLawForm<t, body_t, head_t, ifbody_t,  unless_t, where_t>(body(), head(), ifbody(), unless(), where(), beginLoc(), endLoc());
}

}}}

