#pragma once

#include <ostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// template base for statements which don't have any content
/// @param type The type of statement this is
template <Statement::Type::type type>
class VoidStatement : public Statement
{
public:
	/// Basic Constructor
	/// @param begin The beginning location of this statement.
	/// @param end The ending location of this tatement.
	VoidStatement(Location const& begin, Location const& end);

	/// Stub destructor
	~VoidStatement();

	// inherited stuffs
	virtual Statement* copy() const;

};

}}}

#include "bcplus/statements/detail/VoidStatement.impl.h"

