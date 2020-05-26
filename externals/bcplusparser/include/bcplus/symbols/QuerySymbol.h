#pragma once

#include <string>
#include <ostream>

#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Symbol.h"

namespace bcplus {
namespace symbols {

class Resolver;

class QuerySymbol : public Symbol {

private:
	/*************************************************************************************/
	/* Private Members */
	/*************************************************************************************/

	/// The minimum maximum step value, or a negative integer for undefined.
	int _minmax;

	/// The maximum maximum step value, or a negative integer for undefined.
	int _maxmax;

public:
	/// Basic constructor
	/// @param base The name of the query
	/// @param minmax The minimum maximum step value, or a negative integer for undefined.
	/// @param maxmax The maximum maximum step value, or a negative integer for undefined.
	QuerySymbol(ReferencedString const* base, int minmax, int maxmax);

	/// Loads the query from the property tree node
	/// @param node The node to load the symbol from
	/// @param err An error strem to write to (or NULL)
	/// Sets the symbol's good flag if it was successful
	QuerySymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err = NULL);

	/// Destructor stub
	virtual ~QuerySymbol();

	/// Get the query's minimum maximum step value (or a negative integer if undefined)
	inline int minmax() const										{ return _minmax; }

	/// Get the query's maximum maximum step value (or a negative integer if undefined)
	inline int maxmax() const										{ return _maxmax; }

	// inherited
	virtual DomainType::type domainType() const;
	virtual bool operator==(Symbol const& other) const;
	virtual void save(boost::property_tree::ptree& node) const;
	virtual void outputDefinition(std::ostream& out) const;


};

}}
