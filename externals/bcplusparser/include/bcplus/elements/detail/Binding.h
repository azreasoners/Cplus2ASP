#pragma once

#include <ostream>
#include <string>

#include "babb/utils/memory.h"

#include "bcplus/DomainType.h"
#include "bcplus/elements/Element.h"
#include "bcplus/Location.h"

namespace bcplus {
namespace elements {

namespace detail {

/**
 * @brief A temporal binding for an element of the form i:F
 */
template <typename BaseType, int type, typename ST, typename T>
class Binding : public BaseType
{
private:
    /****************************************************************************/
    /* Private Members */
    /****************************************************************************/

    /// The constant
    babb::utils::ref_ptr<ST> _step;

	/// The value
	babb::utils::ref_ptr<T> _element;

public:
    /****************************************************************************/
    /* Constructor / Destructor */
    /****************************************************************************/
    /// Full constructor
    /// @param step The step to bind the element to
	/// @param element The element to bind
	/// @param begin The beginning location of this element
    /// @param end The ending location of this element
    /// @param parens Whether the element is surrounded by parentheses
    Binding(ST* step, T* element, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

    /// Destructor stub
    virtual ~Binding();

    /****************************************************************************/
    /* Public Methods */
    /****************************************************************************/

	/// The step
	inline ST* step()							{ return _step; }
	inline ST const* step() const				{ return _step; }

	/// The value
	inline T* element()						{ return _element; }
	inline T const* element() const			{ return _element; }

	// inherited
    virtual Element* copy() const;
    virtual void output(std::ostream& out) const;
	virtual DomainType::type domainType() const;
private:

};

}}}

#include "bcplus/elements/detail/Binding.impl.h"

