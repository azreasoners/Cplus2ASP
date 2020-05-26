#pragma once

#include <boost/type_traits.hpp>
#include <ostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {
namespace detail {

/// template base for statements which have a single payload value
/// @param type The type of statement this is
/// @param value_t the type for the payload
/// @param valuearg_t the type to accept during initialization that will be used to initialize the value
template <Statement::Type::type type, typename value_t, typename valuearg_t >
class SimpleStatement : public Statement
{
public:

	typedef valuearg_t value_type;

private:

	/// The value of this statement
	value_t _value;

public:
	/// Basic Constructor
	/// @param begin The beginning location of this statement.
	/// @param end The ending location of this tatement.
	SimpleStatement(value_type value, Location const& begin, Location const& end);

	/// Stub destructor
	~SimpleStatement();

	/// Get the statement value
	inline typename boost::remove_const<value_type>::type const value() const 			{ return _value; }
	inline value_type value() 															{ return _value; }



	// inherited stuffs
	virtual Statement* copy() const;

};

}}}

#include "bcplus/statements/detail/SimpleStatement.impl.h"

