#pragma once

#include <ostream>
#include <string>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/DomainType.h"

#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/VariableSymbol.h"
#include "bcplus/elements/formulas.h"
#include "bcplus/elements/terms.h"

namespace bcplus {
namespace elements {

/**
 * @brief A cardinality formula 'min{v1,v2,...| F}max'
 */
class CardinalityFormula : public Formula
{

public:
    /****************************************************************************/
    /* Public Types */
    /****************************************************************************/

	typedef ReferencedList<babb::utils::ref_ptr<const symbols::VariableSymbol> >::type VariableList;

	typedef VariableList::iterator iterator;
	typedef VariableList::const_iterator const_iterator;


private:
    /****************************************************************************/
    /* Private Members */
    /****************************************************************************/

	/// min/max limits. (if they exist)
	babb::utils::ref_ptr<Term> _min;
	babb::utils::ref_ptr<Term> _max;

    /// The sort bindings for local variables
	babb::utils::ref_ptr<VariableList> _vars;
	

	/// The formula we're testing
	babb::utils::ref_ptr<Formula> _formula;


public:
    /****************************************************************************/
    /* Constructor / Destructor */
    /****************************************************************************/
    /// Full constructor
	/// @param vars the local variables
	/// @param formula The formula being operated on.
	/// @param min The minimum limit (or NULL).
	/// @param max The maximum limit (or NULL).
    /// @param begin The beginning location of this element
    /// @param end The ending location of this element
    /// @param parens Whether the element is surrounded by parentheses
    CardinalityFormula(VariableList* vars, Formula* formula, Term* min = NULL, Term* max = NULL, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

    /// Destructor stub
    virtual ~CardinalityFormula();

    /****************************************************************************/
    /* Public Methods */
    /****************************************************************************/

	/// Iterate over the local bindings
	inline iterator begin()					{ return _vars->begin(); }
	inline const_iterator begin() const		{ return _vars->begin(); }
	inline iterator end()					{ return _vars->end(); }
	inline const_iterator end() const		{ return _vars->end(); }

	/// Get the atomic formula.
	inline Formula* formula()				{ return _formula; }
	inline Formula const* formula() const	{ return _formula; }

	// Get the minimum limit (or NULL)
	inline Term* min()						{ return _min; }
	inline Term const* min() const			{ return _min; }

	// Get the maximum limit (or NULL)
	inline Term* max() 						{ return _max; }
	inline Term const* max() const			{ return _max; }


	// inherited
    virtual Element* copy() const;
    virtual void output(std::ostream& out) const;
	virtual DomainType::type domainType() const;

};

}}

