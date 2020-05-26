
#include <string>
#include <sstream>
#include <ostream>
#include <algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/DomainType.h"
#include "bcplus/elements/Element.h"
#include "bcplus/symbols/MacroSymbol.h"


namespace bcplus {
namespace symbols{

bool length_desc(ReferencedString const* left, ReferencedString const* right) {
	if (left->size() != right->size()) return left->size() > right->size();
	return left > right;
}

MacroSymbol::MacroSymbol(ReferencedString const* base, ReferencedString const* text, ArgumentList* args)
	: Symbol(Symbol::Type::MACRO, base, (args ? args->size() : 0)), _text(text) {
	if (args) _args = args;
	else _args = new ArgumentList();

	compileArgMap();
}

MacroSymbol::MacroSymbol(boost::property_tree::ptree const& node, std::ostream* err) 
	: Symbol(Symbol::Type::MACRO, node, err) {
	babb::utils::ref_ptr<ArgumentList> args = new ArgumentList();	

	// Get the text definition of the macro
	try {
		_text = new ReferencedString(node.get<std::string>("<xmlattr>.text"));

	} catch (boost::property_tree::ptree_error const& e) {
		good(false);
		if (err) *err << "ERROR: An error occurred scanning the definition of \"" << *name() << "\". Expected a 'text' attribute containing the macro expansion text." << std::endl;
	}	

	// Get all of the sorts for each of the arguments
	for (size_t i = 0; i < arity(); i++) {
		try {
			std::string arg = node.get<std::string>("<xmlattr>.arg" + boost::lexical_cast<std::string>(i));
			args->push_back(new ReferencedString(arg));		
	
		} catch (boost::property_tree::ptree_error& e) {
			good(false);
			if (err) *err << "ERROR: An error occurred scanning the definition of \"" << *name() << "\". The value for argument " << i << " is not properly declared. Please ensure there is an 'arg" << i << "' attribute attached to the declaration." << std::endl;
		}
	}

	_args = args;
	if (good()) compileArgMap();
}

MacroSymbol::~MacroSymbol() {
	delete[] _arg_map;
}

void MacroSymbol::save(boost::property_tree::ptree& node) const {
	Symbol::save(node);

	node.put("<xmlattr>.text", *_text);

	size_t arg = 0;
	BOOST_FOREACH(ReferencedString const* argval, *_args) {
		node.put("<xmlattr>.arg" + boost::lexical_cast<std::string>(arg++), *argval);
	}
}

void MacroSymbol::compileArgMap() {
	ArgumentList sorted;
	sorted = *_args;
	std::sort(sorted.begin(), sorted.end(), length_desc);

	// compile a map from sorted position to real position
	_arg_map = new size_t[_args->size()];
	
	for (size_t i = 0; i < arity(); i++) {
		for (size_t j = 0; j < arity(); j++) {
			if (sorted[i] == (*_args)[j]) {
				_arg_map[i] = j;
			}
		}
	}
}

std::string MacroSymbol::expand(ArgumentList const* args) const {
	if (!args && !arity()) return *_text;
	else if (!args) return "<MACRO_ERROR>";	
	if (args->size() != arity()) return "<MACRO_ERROR>";

	// start with macro definition
	std::string result = *_text;

	for (size_t i = 0; i < arity(); i++) {
		size_t j = _arg_map[i];
		boost::replace_all(result, *(*_args)[j], *(*args)[j]);
	}

	return result;
}

std::string MacroSymbol::expand(ElementList const* args) const {

	if (!args && !arity()) return *_text;
	else if (!args) return "<MACRO_ERROR>";
	if (args->size() != arity()) return "<MACRO_ERROR>";
	
	std::stringstream tmp;
	std::string result = *_text;

	for (size_t i = 0; i < arity(); i++) {
		size_t j = _arg_map[i];
		tmp.str("");
		tmp << *(*args)[j];
		boost::replace_all(result, *(*_args)[j], tmp.str());
	}

	return result;
}

std::string MacroSymbol::expand(TokenList const* args) const {

	if (!args && !arity()) return *_text;
	else if (!args) return "<MACRO_ERROR>";
	if (args->size() != arity()) return "<MACRO_ERROR>";
	
	std::string result = *_text;
	
	for (size_t i = 0; i < arity(); i++) {
		size_t j = _arg_map[i];
		boost::replace_all(result, *(*_args)[j], *((*args)[j])->str());
	}

	return result;
}

void MacroSymbol::outputDefinition(std::ostream& out) const {
	out << *base();
	if (arity()) {
		out << "(";
		for (const_iterator it = begin(); it != end(); ) {

			out << **it;

			it++;
			if (it != end()) {
				out << ", ";
			}
		}
		out << ")";
	}

	out << " -> " << *_text;
}


DomainType::type MacroSymbol::domainType() const {
	return DomainType::NO_DOMAIN;
}


}}


