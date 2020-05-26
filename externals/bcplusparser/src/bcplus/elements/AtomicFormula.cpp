
#include "bcplus/DomainType.h"
#include "bcplus/elements/formulas.h"
#include "bcplus/elements/terms.h"
#include "bcplus/elements/AtomicFormula.h"

namespace bcplus {
namespace elements {

AtomicFormula::AtomicFormula(Constant const* c, Term const* v, Location const& begin, Location const& end, bool p)
	: Formula(Formula::Type::ATOMIC, newConstSet(c, v), newVarSet(c, v), c->cmask() | v->cmask(), begin, end, p), _c(c), _v(v) {
	/* Intentionally left blank */
}

AtomicFormula::~AtomicFormula() {
	/* Intentionally left blank */
}

Element* AtomicFormula::copy() const {
	return new AtomicFormula((Constant const*)c()->copy(), (Term*)v()->copy(), beginLoc(), endLoc(), parens());
}

void AtomicFormula::output(std::ostream& out) const {
	if (parens()) out << "(";
	c()->output(out);
	out << "=";
	v()->output(out);
	if (parens()) out << ")";
}

DomainType::type AtomicFormula::domainType() const {
	return DomainType::NO_DOMAIN;
}

}}


