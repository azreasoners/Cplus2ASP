#pragma once

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/detail/TemporalLawForm.h"

namespace bcplus {
namespace statements {
namespace detail {


template <Statement::Type::type t, typename head_t, typename at_t>
TemporalLawForm<t, head_t, at_t>::TemporalLawForm(head_t const* head, at_t const* at,
	Location const& begin, Location const& end)
	: Statement(t, begin, end), _head(head), _at(at) {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename at_t>
TemporalLawForm<t, head_t, at_t>::~TemporalLawForm() {
	/* Intentionally left blank */
}

template <Statement::Type::type t, typename head_t, typename at_t>
Statement* TemporalLawForm<t, head_t, at_t>::copy() const {
	return new TemporalLawForm<t, head_t, at_t>(head(), at(), beginLoc(), endLoc());
}

}}}

