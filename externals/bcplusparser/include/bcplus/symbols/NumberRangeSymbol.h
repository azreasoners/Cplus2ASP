#pragma once
#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/elements/detail/ElementClass.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/Resolver.h"
#include "bcplus/DomainType.h"
#include "bcplus/elements/terms.h"

namespace bcplus {
namespace symbols {


class Resolver;

/// A simple class to track an integral token
class NumberRangeSymbol : public Symbol
{
private:
	/************************************************************/
	/* Private Members */
	/************************************************************/

	/// The token's minimum and maximum values
	babb::utils::ref_ptr<elements::Term> _min, _max;

public:
	/************************************************************/
	/* Constructors / Destructors */
	/************************************************************/
	NumberRangeSymbol(ReferencedString const* name, elements::Term* min, elements::Term* max);

	/// Loads the object from the property tree node
    /// @param node The node to load the symbol from
    /// @param err An error strem to write to (or NULL)
    /// Sets the symbol's good flag if it was successful
	/// NOTE: Not fully implemented.
    NumberRangeSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err = NULL);

	virtual ~NumberRangeSymbol();


	/************************************************************/
	/* Public Members */
	/************************************************************/

	/// Get the value of the token
	inline elements::Term const* min() const					{ return _min; }
	inline elements::Term const* max() const					{ return _max; }
	inline elements::Term* min()								{ return _min; }
	inline elements::Term* max()								{ return _max; }
	

	// inherited
	inline DomainType::type domainType() const		{ return DomainType::INTEGRAL; }
 	virtual void save(boost::property_tree::ptree& node) const;

};


}}
