#include <ostream>
#include <boost/foreach.hpp>

#include "babb/utils/memory.h"

#include "bcplus/Location.h"
#include "bcplus/DomainType.h"
#include "bcplus/symbols/VariableSymbol.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/formulas.h"
#include "bcplus/elements/terms.h"
#include "bcplus/elements/CardinalityFormula.h"

namespace u = babb::utils;

namespace bcplus {
namespace elements {


CardinalityFormula::CardinalityFormula(VariableList* vars, Formula* formula, Term* min, Term* max, Location const& begin, Location const& end, bool parens)
	: Formula(Formula::Type::CARDINALITY, 
			insertConstants(newConstSet(formula, min), max), 
			NULL,
			formula->cmask() | (min ? min->cmask() : 0) | (max ? max->cmask() : 0), 
			begin, end, parens), 
		_min(min), _max(max), _vars(vars), _formula(formula) {
	if (!vars) _vars = new VariableList();

	u::ref_ptr<VariableSet> v = newVarSet(formula);

	BOOST_FOREACH(symbols::VariableSymbol const* var, *this) {
		v->erase(var);
	}

	if (min) v->insert(min->beginFreeVariables(), min->endFreeVariables());
	if (max) v->insert(max->beginFreeVariables(), max->endFreeVariables());
	freeVariables(v);
}

CardinalityFormula::~CardinalityFormula() {
	/* Intentionally left blank */
}

Element* CardinalityFormula::copy() const {
	// copy the binding list
	VariableList* l = new VariableList();
	BOOST_FOREACH(symbols::VariableSymbol const* var, *this) {
		l->push_back(var);
	}

	return new CardinalityFormula(l, (Formula*)formula()->copy(), (Term*)min()->copy(), (Term*)max()->copy(), beginLoc(), endLoc(), parens());
}

void CardinalityFormula::output(std::ostream& out) const {
	if (min()) out << *min();
	out << "{";
	bool first = true;
	BOOST_FOREACH(symbols::VariableSymbol const* v, *this) {
		if (!first) out << ", ";
		first = false;
		out << *v->base();
	}
	out << " | " << *formula() << "}";
	if (max()) out << *max(); 

}


DomainType::type CardinalityFormula::domainType() const {
	return DomainType::NO_DOMAIN;
}


}}




