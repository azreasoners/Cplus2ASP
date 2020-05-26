#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/SimpleLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename head_t, typename where_t>
SimpleLawForm<t, head_t, where_t>::SimpleLawForm(head_t const* head, where_t const* where,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _head(head), _where(where) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename where_t>
SimpleLawForm<t, head_t, where_t>::~SimpleLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename where_t>
Statement* SimpleLawForm<t, head_t, where_t>::copy() const {
	return new SimpleLawForm<t, head_t, where_t>(head(), where(), beginLoc(), endLoc());
}

}}}

