
#include <cstring>

#include "babb/utils/utils.h"

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/detail/BaseSymbol.h"
#include "bcplus/symbols/ObjectSymbol.h"


namespace u = babb::utils;


namespace bcplus {
namespace symbols {


ObjectSymbol::ObjectSymbol(ReferencedString const* b, SortList const* args) 
	: BaseSymbol(Symbol::Type::OBJECT, b, args) {
	initDomainType();
}

ObjectSymbol::ObjectSymbol(boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err)
	: BaseSymbol(Symbol::Type::OBJECT, node, resolver, err) {
	initDomainType();
}

ObjectSymbol::~ObjectSymbol() {
	// Intentionally left blank
}


void ObjectSymbol::initDomainType() {
	if (arity()) _dt = DomainType::OTHER;
	else {
		char const* cstr = base()->c_str();
		if (!strcmp(cstr, "true")) {
			_dt = DomainType::BOOLEAN;
			_bool = true;
		} else if (!strcmp(cstr, "false")) {
			_dt = DomainType::BOOLEAN;
			_bool = false;
		} else if (u::fromString(base()->c_str(), _int)) {
			_dt = DomainType::INTEGRAL;
		} else _dt = DomainType::OTHER;
	}

}


}}

