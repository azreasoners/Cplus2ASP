#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/BasicLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename head_t, typename ifbody_t, typename ifcons_t, typename after_t, typename unless_t, typename where_t>
BasicLawForm<t, head_t, ifbody_t, ifcons_t, after_t, unless_t, where_t>::BasicLawForm(head_t const* head, ifbody_t const* ifbody, ifcons_t const* ifcons,
	after_t const* after, unless_t const* unless, where_t const* where,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _head(head), _ifbody(ifbody), _ifcons(ifcons), _after(after), _unless(unless), _where(where) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename ifbody_t, typename ifcons_t, typename after_t, typename unless_t, typename where_t>
BasicLawForm<t, head_t, ifbody_t, ifcons_t, after_t, unless_t, where_t>::~BasicLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename ifbody_t, typename ifcons_t, typename after_t, typename unless_t, typename where_t>
Statement* BasicLawForm<t, head_t, ifbody_t, ifcons_t, after_t, unless_t, where_t>::copy() const {
	return new BasicLawForm<t, head_t, ifbody_t, ifcons_t, after_t, unless_t, where_t>(head(), ifbody(), ifcons(), after(), unless(), where(), beginLoc(), endLoc());
}

}}}

