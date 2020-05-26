
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>

#include "babb/utils/memory.h"

#include "bcplus/symbols/Resolver.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/detail/BaseSymbol.h"


namespace u = babb::utils;

namespace bcplus {
namespace symbols{
namespace detail {

BaseSymbol::BaseSymbol(Symbol::Type::type type, ReferencedString const* base, SortList const* args)
	: Symbol(type, base, (args ? args->size() : 0)) {
	if (args) _args = args;
	else _args = new SortList();

}

BaseSymbol::BaseSymbol(Symbol::Type::type type, boost::property_tree::ptree const& node, Resolver const* resolver, std::ostream* err)
	: Symbol(type, node, err) {

	u::ref_ptr<SortList> l = new SortList();

	// Get all of the sorts for each of the arguments
	for (size_t i = 0; i < arity(); i++) {
		try {
			std::string sort_name = node.get<std::string>("<xmlattr>.arg" + boost::lexical_cast<std::string>(i));
			SortSymbol const* sort = (SortSymbol const*)resolver->resolve(Symbol::Type::SORT, sort_name);
			if (!sort) {
				good(false);
				if (err) *err << "ERROR: An error occurred scanning the definition of \"" << *name() << "\". \"" << sort_name << "\" is not a declared sort." << std::endl;
			} else {
				l->push_back(sort);
			}
			
		} catch (boost::property_tree::ptree_error& e) {
			good(false);
			if (err) *err << "ERROR: An error occurred scanning the definition of \"" << *name() << "\". The sort for argument " << i << " is not properly declared. Please ensure there is an 'arg" << i << "' attribute attached to the declaration." << std::endl;
		}
	}

	_args = l;
}

BaseSymbol::~BaseSymbol() {
	// Intentionally left blank
}

bool BaseSymbol::operator==(Symbol const& other) const {
	if (!Symbol::operator==(other)) return false;
	BaseSymbol const& o = (BaseSymbol const&)other;

	// check argument sorts
	// we can use pointer comparison as we draw from the same set of symbols
	const_iterator oit = o.begin();
	for (const_iterator it = begin(); it != end(); it++) {
		if (oit == o.end()) return false;
		if (*it != *oit) return false;
		oit++;
	}
	if (oit != o.end()) return false;
	return true;
}

void BaseSymbol::save(boost::property_tree::ptree& node) const {
	Symbol::save(node);
	size_t arg = 0;
	BOOST_FOREACH(SortSymbol const* sort, *_args) {
		node.put("<xmlattr>.arg" + boost::lexical_cast<std::string>(arg++), *(sort->base()));
	}
}

void BaseSymbol::outputDefinition(std::ostream& out) const {
	out << *base();
	if (arity()) {
		out << "(";
		for (const_iterator it = begin(); it != end(); ) {

			out << *(*it)->base();

			it++;
			if (it != end()) {
				out << ", ";
			}
		}
		out << ")";
	}
}


}}}
