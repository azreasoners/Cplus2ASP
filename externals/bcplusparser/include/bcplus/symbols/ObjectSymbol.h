#pragma once

#include <string>
#include <ostream>

#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/detail/BaseSymbol.h"

namespace bcplus {
namespace symbols {

class Resolver;

class ObjectSymbol : public detail::BaseSymbol {

private:
public:
	/*************************************************************************************/
	/* Public Types */
	/*************************************************************************************/

private:
	/*************************************************************************************/
	/* Private Members */
	/*************************************************************************************/

	/// The domain type
	DomainType::type _dt;

	/// The integer represented by this symbol, if any.
	int _int;
	/// The boolean represented by this symbol, if any
	bool _bool;

public:

	/// Basic constructor
	/// @param base The name of this object
	/// @param args The sorts for each of the arguments for this object.
	ObjectSymbol(ReferencedString const* base, SortList const* args = NULL);

	/// Loads the object from the property tree node
	/// @param node The node to load the symbol from
	/// @param err An error strem to write to (or NULL)
	/// Sets the symbol's good flag if it was successful
	ObjectSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err = NULL);

	/// Destructor stub
	virtual ~ObjectSymbol();

	/// Gets the integer represented by this object, if any.
	inline int const* integer() const						{ return ((_dt == DomainType::INTEGRAL) ? &_int : NULL); }

	/// Gets the boolean value represented by this object, if any.
	inline bool const* boolean() const						{ return ((_dt == DomainType::BOOLEAN) ? &_bool : NULL); }

	// inherited
	inline DomainType::type domainType() const				{ return _dt; }

private:

	/// Determines the domain type of this object symbol and initializes the appropriate object members.
	void initDomainType();

};

}}
