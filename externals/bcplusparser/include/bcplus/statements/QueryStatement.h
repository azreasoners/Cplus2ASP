#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/symbols/QuerySymbol.h"
#include "bcplus/elements/formulas.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {

/// A structure for handling queries
class QueryStatement : public Statement 
{
public:
	/************************************************************************************/
	/* Public Types */
	/************************************************************************************/
	typedef ReferencedList<babb::utils::ref_ptr<const elements::Formula> >::type FormulaList;

	typedef FormulaList::iterator iterator;
	typedef FormulaList::const_iterator const_iterator;

private:
	/************************************************************************************/
	/* Private Members */
	/************************************************************************************/
	/// The query's symbol
	babb::utils::ref_ptr<const symbols::QuerySymbol> _sym;

	/// The query's list of query formulas
	babb::utils::ref_ptr<FormulaList> _formulas;

public:
	/************************************************************************************/
	/* Constructors / Destructors */
	/************************************************************************************/
	/// Basic constructor
	/// @param symbol The symbol for this query
	/// @param formulas The list of formulas within the query (or NULL to create an empty query)
	/// @param begin The beginning location for this statement
	/// @param end The ending location for this statement
	QueryStatement(symbols::QuerySymbol const* symbol, FormulaList* formulas, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0));

	/// Destructor stub
	virtual ~QueryStatement();

	/// Get's the query's symbol
	inline symbols::QuerySymbol const* symbol() const						{ return _sym; }

	/// Gets the query label, if any
	inline ReferencedString const* label() const							{ return _sym->base(); }

	/// Gets the query maxstep range definition, if any (negative values mean undefined)
	inline int maxmax() const												{ return _sym->maxmax(); }
	inline int minmax() const												{ return _sym->minmax(); }

	/// Gets the number of formulas that should be included
	inline size_t size() const												{ return _formulas->size(); }

	/// Iterate over the formula list
	inline iterator begin()													{ return _formulas->begin(); }
	inline const_iterator begin() const										{ return _formulas->begin(); }
	inline iterator end()													{ return _formulas->end(); }
	inline const_iterator end() const										{ return _formulas->end(); }

	/// Adds a formula to the query
	inline void push_back(elements::Formula const* formula) 				{ _formulas->push_back(formula); }


	/// Perform a dep copy of this statement
	virtual Statement* copy() const;


};


}}


