
#include "bcplus/DomainType.h"
#include "bcplus/symbols/Resolver.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/VariableSymbol.h"
#include "bcplus/symbols/SortSymbol.h"

namespace bcplus {
namespace symbols{


VariableSymbol::VariableSymbol(ReferencedString const* base, SortSymbol const* sort)
	: Symbol(Symbol::Type::VARIABLE, base, 0), _sort(sort) {
	// Intentionally left blank
}

VariableSymbol::VariableSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err)
	: Symbol(Symbol::Type::VARIABLE, node, err) {

	if (good() && arity()) {
		good(false);
		if (err) *err << "ERROR: Variable \"" << *base() << "\" cannot be declared with non-zero arity." << std::endl;
	}
	
	std::string sortname = node.get("<xmlattr>.sort", "");
	if (sortname  == "") {
		good(false);
		if (err) *err << "ERROR: Expected a 'sort' attribute in the declaration of variable \"" << *base() << "\"." << std::endl;
	} else {

		SortSymbol const* s = (SortSymbol const*)resolver->resolve(Symbol::Type::SORT, sortname);
		if (!s) {
			good(false);

			if (err) {
				// see if the symbol is declared elsewhere
				if (resolver->resolve(~Symbol::Type::SORT, sortname)) {
					*err << "ERROR: Variable \"" << *base() << "\" has invalid 'sort' attribute. \"" << sortname << "\" is not a sort." << std::endl;
				} else {
					*err << "ERROR: Variable \"" << *base() << "\" has invlaid 'sort' attribute. \"" << sortname << "\" has not been declared." << std::endl;
				}
			}
		}

		_sort = s;
	}
}

VariableSymbol::~VariableSymbol() {
	// Intentionally left blank
}

DomainType::type VariableSymbol::domainType() const {
	return sort()->domainType();
}

bool VariableSymbol::operator==(Symbol const& other) const {
	if (!Symbol::operator==(other)) return false;
	VariableSymbol const& o = (VariableSymbol const&)other;
	
	// we draw from the same set of symbols so we can use pointer comparison
	if (sort() != o.sort()) return false;
	return true;

}

void VariableSymbol::save(boost::property_tree::ptree& node) const {
	Symbol::save(node);
	node.put("<xmlattr>.sort", *(sort()->base()));
}

void VariableSymbol::outputDefinition(std::ostream& out) const {
	out << *base();
}

}}

