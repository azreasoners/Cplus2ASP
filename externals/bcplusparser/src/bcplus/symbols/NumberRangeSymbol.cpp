#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"


#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/Resolver.h"
#include "bcplus/symbols/NumberRangeSymbol.h"
#include "bcplus/elements/terms.h"


namespace bcplus {
namespace symbols {

NumberRangeSymbol::NumberRangeSymbol(ReferencedString const* name, elements::Term* min, elements::Term* max)
	: Symbol(Symbol::Type::RANGE, name, 0), _min(min), _max(max) {
	/* Intentionally Left Blank */
}

NumberRangeSymbol::NumberRangeSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err)
	: Symbol(Symbol::Type::RANGE, node, err) {
	// TODO: Load terms
}


NumberRangeSymbol::~NumberRangeSymbol() {
	/* Intentionally Left Blank */
}


void NumberRangeSymbol::save(boost::property_tree::ptree& node) const {
	/// TODO: Save terms
	Symbol::save(node);
}


}}

