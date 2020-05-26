

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "as2transition/Config.h"
#include "as2transition/PredElement.h"


namespace as2transition {

namespace u = babb::utils;


PredElement::PredElement(Type::type type, ReferencedString const* base, ElementList* args)
	: _base(base), _args(args), _type(type) {
	if (!_args) _args = new ElementList();

}

PredElement::PredElement(Type::type type, ReferencedString const* base, PredElement* arg)
	: _base(base), _type(type) {
	_args = new ElementList();
	_args->push_back(arg);

}

PredElement::~PredElement() { }

PredElement* PredElement::format(Config const* config) const {

	u::ref_ptr<const ReferencedString> newbase = base();

	if (config->stripPrefix()) {
		// remove o_
		if (base()->find("o_") == 0) {
			newbase = new ReferencedString(base()->substr(2));

		// remove c_x_x_
		} else if (base()->find("c_") == 0) {
			size_t loc1 = base()->find("_", 2);
			if (loc1 != std::string::npos) {
				size_t loc2 = base()->find("_", loc1+1);
				if (loc2 != std::string::npos) {
					newbase = new ReferencedString(base()->substr(loc2+1));
				} 
			} 
		
		// remove e_x_x_
		} else if (base()->find("e_") == 0) {
			size_t loc1 = base()->find("_", 2);
			if (loc1 != std::string::npos) {
				size_t loc2 = base()->find("_", loc1+1);
				if (loc2 != std::string::npos) {
					newbase = new ReferencedString(base()->substr(loc2+1));
				} 
			} 
		}

	}

	if (config->stripSanitization()) {
		// remove "saniConst_"
		if (newbase->find("saniConst_") == 0) {
			newbase = new ReferencedString(newbase->substr(strlen("saniConst_")));
		}

		// remove "saniObj_"
		else if (newbase->find("saniObj_") == 0) {
			newbase = new ReferencedString(newbase->substr(strlen("saniObj_")));
		}
	}

	if (config->noneAlias() && *newbase == *config->noneAlias()) {
		newbase = new ReferencedString("none");
	}

	// recurse...
	u::ref_ptr<ElementList> newargs = new ElementList();
	for (const_iterator it = begin(); it != end(); it++) {
		newargs->push_back((*it)->format(config));
	}


	return new PredElement(type(), newbase, newargs);

}



bool PredElement::operator<(PredElement const& other) const {
	if (*base() != *other.base()) return *base() < *other.base();
	if (arity() != other.arity()) return arity() < other.arity();

	for (size_t i = 0; i < arity(); i++) {
		if (*get(i) != *other.get(i)) return *get(i) < *other.get(i);
	}
	return false;
}

bool PredElement::operator>(PredElement const& other) const {
	if (*base() != *other.base()) return *base() > *other.base();
	if (arity() != other.arity()) return arity() > other.arity();

	for (size_t i = 0; i < arity(); i++) {
		if (*get(i) != *other.get(i)) return *get(i) > *other.get(i);
	}
	return false;
}



bool PredElement::operator==(PredElement const& other) const {
	if (*base() != *other.base()) return false;
	if (arity() != other.arity()) return false;

	for (size_t i = 0; i < arity(); i++) {
		if (*get(i) != *other.get(i)) return false;
	}
	return true;
}


bool PredElement::operator!=(PredElement const& other) const {
	if (*base() != *other.base()) return true;
	if (arity() != other.arity()) return true;

	for (size_t i = 0; i < arity(); i++) {
		if (*get(i) != *other.get(i)) return true;
	}
	return false;
}



std::ostream& operator<<(std::ostream& out, PredElement const& elem) {

	if (*elem.base() == "=" && elem.arity() == 2) {
		// special case... if the element is =(c,v) then use infix notation...
		out << *elem.get(0);
		out << *elem.base();
		out << *elem.get(1);
	} else if (*elem.base() == "~" && elem.arity() == 1) {
		// special case... if the elemen is ~(c) then omit the parens...
		out << *elem.base() << *elem.get(0);
	} else if (*elem.base() == "-" && elem.arity() == 1) {
		// special case... if the elemen is ~(c) then omit the parens...
		out << *elem.base() << *elem.get(0);
	} else {
		
		out << *elem.base();
		if (elem.arity()) {
			out << "(";
			bool first = true;
			for (PredElement::const_iterator it = elem.begin(); it != elem.end(); it++) {
				if (first) first = false;	
				else out << ", ";
				out << **it;
			}
			out << ")";
		}
	}
	return out;
}



};
