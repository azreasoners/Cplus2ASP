
#include <boost/foreach.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/symbols/QuerySymbol.h"
#include "bcplus/elements/formulas.h"
#include "bcplus/elements/terms.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/QueryStatement.h"


namespace bcplus {
namespace statements {

QueryStatement::QueryStatement(symbols::QuerySymbol const* sym, FormulaList* formulas, Location const& begin, Location const& end)
	: Statement(Statement::Type::QUERY, begin, end), _sym(sym) {
	if (formulas) _formulas = formulas;
	else _formulas = new FormulaList();
	/* Intentionally left blank */
}

QueryStatement::~QueryStatement() {
	/* Intentionally left blank */
}

Statement* QueryStatement::copy() const {
	// copy the formula list
	babb::utils::ref_ptr<FormulaList> l = new FormulaList();
	BOOST_FOREACH(elements::Formula const* f, *this) {
		l->push_back(f);
	}

	return new QueryStatement(symbol(), l, beginLoc(), endLoc());
}


}}

