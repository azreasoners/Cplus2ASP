#pragma once

#include <string>
#include <ostream>

#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"

#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/DomainType.h"
#include "bcplus/symbols/Symbol.h"

namespace bcplus {
namespace symbols {

class Resolver;

class VariableSymbol : public Symbol {

private:
	/*************************************************************************************/
	/* Private Members */
	/*************************************************************************************/
	/// The variable's sort
	babb::utils::ref_ptr<const SortSymbol> _sort;

public:
	/// Basic constructor
	/// @param base The name of the variable
	/// @param sort The sort the variable ranges over
	VariableSymbol(ReferencedString const* base, SortSymbol const* sort);

	/// Loads the variable from the property tree node
	/// @param node The node to load the symbol from
	/// @param err An error strem to write to (or NULL)
	/// Sets the symbol's good flag if it was successful
	VariableSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err = NULL);

	/// Destructor stub
	virtual ~VariableSymbol();

	/// Get the variable's sort
	SortSymbol const* sort() const									{ return _sort; }

	virtual DomainType::type domainType() const;
	virtual bool operator==(Symbol const& other) const;
	virtual void save(boost::property_tree::ptree& node) const;
	virtual void outputDefinition(std::ostream& out) const;


};

}}
