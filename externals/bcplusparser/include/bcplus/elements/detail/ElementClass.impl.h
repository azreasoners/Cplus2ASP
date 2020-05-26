
#include "bcplus/Location.h"
#include "bcplus/elements/detail/ElementClass.h"

namespace bcplus {
namespace elements  {
namespace detail {

template<int t, typename Subtypes>
ElementClass<t, Subtypes>::ElementClass(typename Type::type subtype, ConstantSet const* constants, VariableSet const* variables, int cmask, Location const& begin, Location const& end, bool parens)
	: Element((Element::Type::type)t, constants, variables, cmask, begin, end, parens), _subtype(subtype) {
	/* Intentionally left blank */
}

template<int type, typename Subtypes>
ElementClass<type, Subtypes>::~ElementClass() {
	/* Intentionally left blank */
}

}}}


