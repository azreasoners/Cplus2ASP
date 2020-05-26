#pragma once

#include "babb/utils/memory.h"

#include "bcplus/Location.h"
#include "bcplus/DomainType.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/detail/AnonymousElement.h"
namespace u = babb::utils;

namespace bcplus {
namespace elements {
namespace detail {

template <typename BaseType, int type>
AnonymousElement_bare<BaseType, type>::AnonymousElement_bare(ReferencedString const* base, Location const& begin, Location const& end, bool parens) 
	: BaseType((typename BaseType::Type::type)type, BaseType::newConstSet(), BaseType::newVarSet(), 0, begin, end, parens), _base(base) {
	/* Intentionally left blank */
}

template <typename BaseType, int type>
AnonymousElement_bare<BaseType, type>::~AnonymousElement_bare() {
	/* Intentionally left blank */
}

template <typename BaseType, int type>
Element* AnonymousElement_bare<BaseType, type>::copy() const {
	return new AnonymousElement_bare<BaseType, type>(base(), ((BaseType const*)this)->beginLoc(), ((BaseType const*)this)->endLoc(), ((BaseType const*)this)->parens());
}

template <typename BaseType, int type>
void AnonymousElement_bare<BaseType, type>::output(std::ostream& out) const {
	out << *base();
}

template <typename BaseType, int type>
DomainType::type AnonymousElement_bare<BaseType, type>::domainType() const {
	return DomainType::UNKNOWN;
}

template <typename BaseType, int type, typename ArgType>
AnonymousElement<BaseType, type, ArgType>::AnonymousElement(ReferencedString const* base, ArgumentList const* args, Location const& begin, Location const& end, bool parens) 
	: AnonymousElement_bare<BaseType, type>(base, begin, end, parens), _args(args) {
	if (!args) _args = new ArgumentList();
}

template <typename BaseType, int type, typename ArgType>
AnonymousElement<BaseType, type, ArgType>::~AnonymousElement() {
	/* Intentionally left blank */
}

template <typename BaseType, int type, typename ArgType>
Element* AnonymousElement<BaseType, type, ArgType>::copy() const {

	// Create a new list
	u::ref_ptr<ArgumentList> l = new ArgumentList();
	for (const_iterator it = begin(); it != end(); it++) {
		l->push_back((ArgType*)(*it)->copy());
	}

	BaseType const* bt = (BaseType const*)this;

	return new AnonymousElement<BaseType, type, ArgType>(AnonymousElement_bare<BaseType, type>::base(), l, bt->beginLoc(), bt->endLoc(), bt->parens());
}

template <typename BaseType, int type, typename ArgType>
void AnonymousElement<BaseType, type, ArgType>::output(std::ostream& out) const {
	AnonymousElement_bare<BaseType, type>::output(out);
	if (arity() > 0) {
		out << "(";
		for (const_iterator it = begin(); it != end(); ) {
			(*it)->output(out);
			it++;
			if (it != end()) {
				out << ", ";
			}
		}
		out << ")";
	}
}	



}}}


