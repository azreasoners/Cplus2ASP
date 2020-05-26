#include <ostream>

#include "bcplus/Location.h"
#include "bcplus/DomainType.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/detail/NullaryElement.h"

namespace bcplus {
namespace elements {
namespace detail {

template <typename BaseType, int t, typename Op, typename toString, typename dt>
NullaryElement<BaseType, t, Op, toString, dt>::NullaryElement(typename Op::type const& op, Location const& begin, Location const& end, bool parens) 
	: BaseType((typename BaseType::Type::type)t, BaseType::newConstSet(), BaseType::newVarSet(), 0, begin, end, parens), _op(op) {
	/* Intentionally left blank */
}



template <typename BaseType, int t, typename Op, typename toString, typename dt>
NullaryElement<BaseType, t, Op, toString, dt>::~NullaryElement() {
	/* Intentionally left blank */
}


template <typename BaseType, int t, typename Op, typename toString, typename dt>
Element* NullaryElement<BaseType, t, Op, toString, dt>::copy() const {
	return new NullaryElement<BaseType, t, Op, toString, dt>(op(), ((BaseType const*)this)->beginLoc(), ((BaseType const*)this)->endLoc(), ((BaseType const*)this)->parens());
}
template <typename BaseType, int t, typename Op, typename toString, typename dt>
void NullaryElement<BaseType, t, Op, toString, dt>::output(std::ostream& out) const {
	toString str;
	out << str(op());
}

template <typename BaseType, int t, typename Op, typename toString, typename dt>
DomainType::type NullaryElement<BaseType, t, Op, toString, dt>::domainType() const {
	dt tp;
	return tp(op());
}

}}}
