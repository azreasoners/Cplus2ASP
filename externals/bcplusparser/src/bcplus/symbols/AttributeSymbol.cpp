
#include <ostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Resolver.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/ConstantSymbol.h"
#include "bcplus/symbols/AttributeSymbol.h"



namespace bcplus {
namespace symbols {

AttributeSymbol::AttributeSymbol(ReferencedString const* base, SortSymbol const* sort, ConstantSymbol const* attribOf, SortList const* args) 
	: ConstantSymbol(ConstantSymbol::Type::ATTRIBUTE, base, sort, args) , _attribof(attribOf) {
	// intentionally left blank
}

AttributeSymbol::AttributeSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err)
	: ConstantSymbol(node, resolver, err) {

	try {
		std::string c_name = node.get<std::string>("<xmlattr>.attributeOf");
		ConstantSymbol const* c = (ConstantSymbol const*)resolver->resolve(Symbol::Type::CONSTANT, c_name);
		if (!c) {
			good(false);
			if (err) *err << "ERROR: An error occurred while scanning the definition of \"" << *name() << "\". \"" << c_name << "\" is not a valid action constant." << std::endl;
		} else {

			if (c->constType() != ConstantSymbol::Type::ABACTION && c->constType() != ConstantSymbol::Type::ACTION && c->constType() != ConstantSymbol::Type::EXOGENOUSACTION) {
				good(false);
				if (err) *err << "ERROR: An error occurred while scanning the definition of \"" << *name() << "\". \"" << c_name << "\" is not a valid target for an attribue. Expected an \"abAction\" or \"action\" type constant." << std::endl;
				
			} else {
				_attribof = c;
			}
		}
	} catch (boost::property_tree::ptree_error& e) {
		good(false);
		if (err) *err << "ERROR: An error occurred while scanning the definition of \"" << *name() << "\". Expected a 'sort' attribute declaring the constant's sort." << std::endl;
	}
}

AttributeSymbol::~AttributeSymbol() {
	// Intentionally left blank
}


bool AttributeSymbol::operator==(Symbol const& other) const {
	if (!ConstantSymbol::operator==(other)) return false;
	AttributeSymbol const& o = (AttributeSymbol const&)other;

	// ensure the parent constants are the same
	if (attribOf() != o.attribOf()) return false;
	return true;
}

void AttributeSymbol::save(boost::property_tree::ptree& node) const {
	ConstantSymbol::save(node);
	node.put("<xmlattr>.attributeOf", *_attribof->name());
}

}}

