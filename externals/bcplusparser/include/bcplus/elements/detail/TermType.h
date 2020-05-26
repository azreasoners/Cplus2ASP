#pragma once

namespace bcplus {
namespace elements {

namespace detail {

/// Container for the types of terms that we can have.
struct TermType {
	enum type {
		BINARY,
		UNARY,
		OBJECT,
		VARIABLE,
		CONSTANT,
		LUA,
		PYTHON,
		ANON_VAR,
		NULLARY,
		BINDING
	};
};
}}}
