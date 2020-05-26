#pragma once

#include <ostream>
#include <string>

#include "babb/utils/memory.h"

#include "bcplus/DomainType.h"
#include "bcplus/Location.h"
#include "bcplus/elements/Element.h"

namespace bcplus {
namespace elements {
namespace detail {

/// template class for nullary elements such as "true" and "false"
/// @param BaseType The base class for this template
/// @param type The type flag to indicate the specialization of the type
/// @param Op The type that should be accepted for operator definitions.
/// @param toString A functor to call to get the post-subelement string corresponding to the operation
/// @param dt A functor to call to determine the domain type of this element.
template <typename BaseType, int type, typename Op, typename toString, typename dt>
class NullaryElement : public BaseType
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

public:
    /****************************************************************************/
    /* Constructor / Destructor */
    /****************************************************************************/
    /// Full constructor
    /// @param op The operator being applied to the subformulas
	/// @param sub The subformula
    /// @param begin The beginning location of this element
    /// @param end The ending location of this element
    /// @param parens Whether the element is surrounded by parentheses
    NullaryElement(typename Op::type const& op, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

    /// Destructor stub
    virtual ~NullaryElement();

    /****************************************************************************/
    /* Public Methods */
    /****************************************************************************/

	/// The operator for the formula
	inline typename Op::type const& op() const	{ return _op; }

	// inherited from Element
    virtual Element* copy() const;
    virtual void output(std::ostream& out) const;
	virtual DomainType::type domainType() const;
};

}}}


#include "bcplus/elements/detail/NullaryElement.impl.h"

