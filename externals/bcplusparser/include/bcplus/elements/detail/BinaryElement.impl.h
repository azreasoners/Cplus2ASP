
#include "bcplus/elements/detail/BinaryElement.h"

namespace bcplus {
namespace elements {
namespace detail {

template <typename BaseType, int type, typename Op, typename LHS, typename RHS, typename opString, typename dt>
BinaryElement<BaseType, type, Op, LHS, RHS, opString, dt>::BinaryElement(typename Op::type const& op, LHS const* left, RHS const* right, Location const& begin, Location const& end, bool parens)
	: BaseType(
		(typename BaseType::Type::type)type, 
		BaseType::newConstSet(left, right), 
		BaseType::newVarSet(left, right), 
		((LHS const*)left)->cmask() | ((RHS const*)right)->cmask(), 
		begin, end, parens), 
		_op(op), _left(left), _right(right) {
	/* Intentionally left blank */
}

template <typename BaseType, int type, typename Op, typename LHS, typename RHS, typename opString, typename dt>
BinaryElement<BaseType, type, Op, LHS, RHS, opString, dt>::~BinaryElement() {
	/* Intentionally left blank */
}

template <typename BaseType, int type, typename Op, typename LHS, typename RHS, typename opString, typename dt>
Element* BinaryElement<BaseType, type, Op, LHS, RHS, opString, dt>::copy() const {
	BaseType const* bt = (BaseType const*)this;

	return new BinaryElement(op(), (LHS const*)left()->copy(), (RHS const*)right()->copy(), bt->beginLoc(), bt->endLoc(), bt->parens());
}

template <typename BaseType, int type, typename Op, typename LHS, typename RHS, typename opString, typename dt>
void BinaryElement<BaseType, type, Op, LHS, RHS, opString, dt>::output(std::ostream& out) const {
	if (((BaseType*)this)->parens()) out << "(";
	left()->output(out);
	opString cstr;
	out << cstr(op());
	right()->output(out);
	if (((BaseType*)this)->parens()) out << ")";
}

template <typename BaseType, int type, typename Op, typename LHS, typename RHS, typename opString, typename dt>
DomainType::type BinaryElement<BaseType, type, Op, LHS, RHS, opString, dt>::domainType() const {
	dt dtype;
	return dtype(op());
}

}}}


