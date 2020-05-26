#pragma once

#include <ostream>
#include <string>

#include "babb/utils/memory.h"


#include "bcplus/Location.h"

namespace bcplus {
namespace elements {
namespace detail {

/// template class for binary elements such as binary formulas and terms
/// @param BaseType The base class for this template
/// @param type The type flag to indicate the specialization of the type
/// @param Op The type for the operators that are allowed
/// @param LHS The type for the left-hand side
/// @param RHS The type for the right-hand side
/// @param opString A functor to convert operators to cstrings
/// @param dt A functor to call to determine the domain type of the element
template <typename BaseType, int type, typename Op, typename LHS, typename RHS, typename opString, typename dt>
class BinaryElement : public BaseType
{

public:
public:
    /****************************************************************************/
    /* Public Types */
    /****************************************************************************/
	/// Operator type alias
	typedef Op Operator;


private:
    /****************************************************************************/
    /* Private Members */
    /****************************************************************************/

    /// The operator
    typename Op::type _op;

	/// The left subformula
	babb::utils::ref_ptr<const LHS> _left;

	/// The right subformula
	babb::utils::ref_ptr<const RHS> _right;


public:
    /****************************************************************************/
    /* Constructor / Destructor */
    /****************************************************************************/
    /// Full constructor
    /// @param op The operator being applied to the subformulas
	/// @param left The left subformula
	/// @param right The right subformula
    /// @param begin The beginning location of this element
    /// @param end The ending location of this element
    /// @param parens Whether the element is surrounded by parentheses
    BinaryElement(typename Op::type const& op, LHS const* left, RHS const* right, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

    /// Destructor stub
    virtual ~BinaryElement();

    /****************************************************************************/
    /* Public Methods */
    /****************************************************************************/

	/// The left-hand subformula
	inline LHS const* left() const					{ return _left; }	

	/// The operator for the formula
	inline typename Op::type const& op() const		{ return _op; }

	/// The right-hand subformula
	inline RHS const* right() const					{ return _right; }

	// inherited from Element
    virtual Element* copy() const;
    virtual void output(std::ostream& out) const;
	virtual DomainType::type domainType() const;
};

}}}


#include "bcplus/elements/detail/BinaryElement.impl.h"

