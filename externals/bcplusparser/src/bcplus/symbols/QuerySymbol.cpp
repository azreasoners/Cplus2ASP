
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Resolver.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/QuerySymbol.h"


namespace bcplus {
namespace symbols{


QuerySymbol::QuerySymbol(ReferencedString const* base, int minmax, int maxmax)
	: Symbol(Symbol::Type::QUERY, base, 0), _minmax(minmax), _maxmax(maxmax) {
	// Intentionally left blank
}

QuerySymbol::QuerySymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err)
	: Symbol(Symbol::Type::QUERY, node, err) {

	if (good() && arity()) {
		good(false);
		if (err) *err << "ERROR: Query \"" << *base() << "\" cannot be declared with non-zero arity." << std::endl;
	}
	
	int minmax = -1;
	int maxmax = -1;
	try {
		minmax = node.get("<xmlattr>.minmax", -1);
		maxmax = node.get("<xmlattr>.maxmax", -1);

	} catch (boost::property_tree::ptree_error const& e) {
		good(false);
		if (err) *err << "Error: Encountered a malformed query symbol declaration. Expected integers for attributes \"minmax\" and \"maxmax\"." << std::endl;
	}

	_minmax = minmax;
	_maxmax = maxmax;
}

QuerySymbol::~QuerySymbol() {
	// Intentionally left blank
}

DomainType::type QuerySymbol::domainType() const {
	return DomainType::NO_DOMAIN;
}

bool QuerySymbol::operator==(Symbol const& other) const {
	// The query symbol doesn't have all of the information.
	// Thus we assume these are not equal if they're not the same
	// object
	return &other != this;	
}

void QuerySymbol::save(boost::property_tree::ptree& node) const {
	Symbol::save(node);
	if (_minmax >= 0) node.put("<xmlattr>.minmax", _minmax);
	if (_maxmax >= 0) node.put("<xmlattr>.maxmax", _maxmax);
}

void QuerySymbol::outputDefinition(std::ostream& out) const {
	out << *base();
}

}}

