#pragma once

#include <ostream>
#include <string>

#include "babb/utils/memory.h"


#include "bcplus/Location.h"
#include "bcplus/elements/Element.h"

namespace bcplus {
namespace elements {
namespace detail {

/// Base class for classes of elements
/// @param type The element type flag for this class
/// @param Subtypes A list of possible Subtypes
template<int type, typename Subtypes>
class ElementClass : public Element
{

public:
	/****************************************************************************/
	/* Public Subtypes */
	/****************************************************************************/

	/// Alias for the possible subtype
	typedef Subtypes Type;

private:
	/****************************************************************************/
	/* Private Members */
	/****************************************************************************/

	/// The subtype we're looking at
	typename Type::type _subtype;

public:
	/****************************************************************************/
	/* Constructor / Destructor */
	/****************************************************************************/
	/// Full constructor
	/// @param type The type of element this is
	/// @param constants A set of constants occuring within the element
	/// @param variables A set of variables occuring within the element
	/// @param cmask A mask of constant types occurring within the element
	/// @param begin The beginning location of this element
	/// @param end The ending location of this element
	/// @param parens Whether the element is surrounded by parentheses
	ElementClass(typename Type::type type, ConstantSet const* constants, VariableSet const* variables, int cmask, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

	/// Destructor stub
	virtual ~ElementClass();

	/****************************************************************************/
	/* Public Methods */
	/****************************************************************************/

	/// Get the formula type
	inline typename Type::type subType() const				{ return _subtype; }


	// inherited
	virtual Element* copy() const = 0;
	virtual void output(std::ostream& out) const = 0;
	virtual DomainType::type domainType() const = 0;
};

}}}

#include "bcplus/elements/detail/ElementClass.impl.h"



