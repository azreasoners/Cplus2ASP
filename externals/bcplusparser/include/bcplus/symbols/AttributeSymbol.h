#pragma once

#include <string>
#include <ostream>

#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/detail/BaseSymbol.h"

namespace bcplus {
namespace symbols {

class SortSymbol;
class Resolver;

class AttributeSymbol : public ConstantSymbol {
public:

private:
	/*************************************************************************************/
	/* Private Members */
	/*************************************************************************************/
	/// The constant this constant is an attribute of
	babb::utils::weak_ptr<const ConstantSymbol> _attribof;

public:
	/// Basic constructor
	/// @param base The name of this object
	/// @param sort The sort that this constant ranges over
	/// @param args The sorts for each of the arguments for this object.
	/// @param attribOf The constant this constant is an attribute of (or NULL)
	AttributeSymbol(ReferencedString const* base, SortSymbol const* sort, ConstantSymbol const* attribOf, SortList const* args = NULL);

	/// Loads the object from the property tree node
	/// @param node The node to load the symbol from
	/// @param err An error strem to write to (or NULL)
	/// Sets the symbol's good flag if it was successful
	AttributeSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err = NULL);

	/// Destructor stub
	virtual ~AttributeSymbol();

	/// Get the constant this symbol is an attribute of (if any)
	ConstantSymbol const* attribOf() const				{ return _attribof; }

	// inherited
	virtual bool operator==(Symbol const& other) const;
	virtual void save(boost::property_tree::ptree& node) const;

};

}}
