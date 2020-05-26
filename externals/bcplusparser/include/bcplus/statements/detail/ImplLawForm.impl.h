#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/ImplLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename head_t, typename body_t, typename where_t>
ImplLawForm<t, head_t, body_t, where_t>::ImplLawForm(head_t const* head, body_t const* body, where_t const* where,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _head(head), _body(body), _where(where) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename body_t, typename where_t>
ImplLawForm<t, head_t, body_t, where_t>::~ImplLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename body_t, typename where_t>
Statement* ImplLawForm<t, head_t, body_t, where_t>::copy() const {
	return new ImplLawForm<t, head_t, body_t, where_t>(head(), body(), where(), beginLoc(), endLoc());
}

}}}

