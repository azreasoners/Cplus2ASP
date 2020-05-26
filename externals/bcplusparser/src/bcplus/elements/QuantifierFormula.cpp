
#include "babb/utils/memory.h"

#include "bcplus/Location.h"
#include "bcplus/DomainType.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/QuantifierFormula.h"

namespace u = babb::utils;

namespace bcplus {
namespace elements {

QuantifierFormula::QuantifierFormula(
	QuantifierList* quants,
	Formula* subformula,
	Location const& beginLoc,
	Location const& endLoc,
	bool parens) 
	: Formula(Formula::Type::QUANTIFIER, subformula->constants(), NULL, subformula->cmask(), beginLoc, endLoc, parens), _quants(quants), _sub(subformula)
{
	u::ref_ptr<VariableSet> vars = newVarSet(subformula);
	for (const_iterator it = begin(); it != end(); it++) {
		vars->erase(it->second->symbol());
	}
	freeVariables(vars);
}

QuantifierFormula::~QuantifierFormula() {
	/* Intentionally left blank */
}

Element* QuantifierFormula::copy() const {

	// Make a new quantifier list
	QuantifierList* l = new QuantifierList();
	for (const_iterator it = begin(); it != end(); it++) {
		l->push_back(Quantifier(it->first, (Variable*)it->second->copy()));
	}

	return new QuantifierFormula(l, (Formula*)subformula()->copy(), beginLoc(), endLoc(), parens());
}

void QuantifierFormula::output(std::ostream& out) const {
	if (parens()) out << "(";

	out << "[";
	for (const_iterator it = begin(); it != end(); it++) {
		outputQuantifier(*it, out);
	}
	out << "| ";
	subformula()->output(out);
	out << " ]";
	if (parens()) out << ")";
}

void QuantifierFormula::outputQuantifier(Quantifier const& q, std::ostream& out) const {
	switch (q.first) {
	case Operator::CONJ:				out << "/\\";	break;
	case Operator::DISJ:				out << "\\/";	break;
	default:							out << "<BAD_QUANTIFIER>"; break;
	}
	q.second->output(out);

}


DomainType::type QuantifierFormula::domainType() const {
	return DomainType::NO_DOMAIN;
}

}}


