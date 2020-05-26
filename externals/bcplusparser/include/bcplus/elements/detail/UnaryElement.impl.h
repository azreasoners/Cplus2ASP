
#include "bcplus/elements/detail/UnaryElement.h"

namespace bcplus {
namespace elements {
namespace detail {

template <typename BaseType, int type, typename Op, typename Sub, typename preOpString, typename postOpString, typename dt>
UnaryElement<BaseType,type,Op,Sub,preOpString,postOpString,dt>::UnaryElement(typename Op::type const& op, Sub const* subformula, Location const& begin, Location const& end, bool parens)
	: BaseType((typename BaseType::Type::type)type, subformula->constants(), subformula->freeVariables(), subformula->cmask(), begin, end, parens), _op(op), _sub(subformula) {
	/* Intentionally left blank */
}

template <typename BaseType, int type, typename Op, typename Sub, typename preOpString, typename postOpString, typename dt>
UnaryElement<BaseType,type,Op,Sub,preOpString,postOpString,dt>::~UnaryElement() {
	/* Intentionally left blank */
}

template <typename BaseType, int type, typename Op, typename Sub, typename preOpString, typename postOpString, typename dt>
Element* UnaryElement<BaseType,type,Op,Sub,preOpString,postOpString,dt>::copy() const {
	return new UnaryElement<BaseType,type,Op,Sub,preOpString,postOpString,dt>(op(), (Sub const*)sub()->copy(), ((BaseType*)this)->beginLoc(), ((BaseType*)this)->endLoc(), ((BaseType*)this)->parens());
}

template <typename BaseType, int type, typename Op, typename Sub, typename preOpString, typename postOpString, typename dt>
void UnaryElement<BaseType,type,Op,Sub,preOpString,postOpString,dt>::output(std::ostream& out) const {
	preOpString pre;
	postOpString post;

	if (((BaseType*)this)->parens()) out << "(";	
	out << pre(op());
	sub()->output(out);
	out << post(op());
	if (((BaseType*)this)->parens()) out << ")";
}

template <typename BaseType, int type, typename Op, typename Sub, typename preOpString, typename postOpString, typename dt>
DomainType::type UnaryElement<BaseType,type,Op,Sub,preOpString,postOpString,dt>::domainType() const {
	dt dtype;
	return dtype(op());
}


}}}


