#pragma once

#include "bcplus/DomainType.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/detail/Binding.h"

namespace bcplus {
namespace elements {
namespace detail {


template <typename BaseType, int type, typename ST, typename T>
Binding<BaseType,type,ST,T>::Binding(ST* step, T* elem, Location const& begin, Location const& end, bool p)
	: BaseType((typename BaseType::Type::type)type, BaseType::newConstSet(step, elem), BaseType::newVarSet(step, elem), step->cmask() | elem->cmask(), begin, end, p), _step(step), _element(elem) {
	/* Intentionally left blank */
}

template <typename BaseType, int type, typename ST, typename T>
Binding<BaseType,type,ST,T>::~Binding() {
	/* Intentionally left blank */
}

template <typename BaseType, int type, typename ST, typename T>
Element* Binding<BaseType,type,ST,T>::copy() const {
	return new Binding((ST*)step()->copy(), (T*)element()->copy(), BaseType::beginLoc(), BaseType::endLoc(), BaseType::parens());
}

template <typename BaseType, int type, typename ST, typename T>
void Binding<BaseType,type,ST,T>::output(std::ostream& out) const {
	if (BaseType::parens()) out << "(";
	step()->output(out);
	out << ":";
	element()->output(out);
	if (BaseType::parens()) out << ")";
}

template <typename BaseType, int type, typename ST, typename T>
DomainType::type Binding<BaseType,type,ST,T>::domainType() const {
	return element()->domainType();
}

}}}


