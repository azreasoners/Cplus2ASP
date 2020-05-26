
#include "bcplus/Location.h"
#include "bcplus/symbols/MacroSymbol.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/Macro.h"
#include "bcplus/elements/detail/IdentifierElement.h"
#include "bcplus/parser/Token.h"

namespace bcplus {
namespace elements {

Macro::Macro(symbols::MacroSymbol const* symbol, ArgumentList const* a, Location const& begin, Location const& end, bool parens)
	: detail::IdentifierElement<Element, Element::Type::MACRO, symbols::MacroSymbol, parser::Token>(symbol, a, begin, end, parens) {
	_expansion = new ReferencedString(symbol->expand(args()));

}

   
Macro::~Macro() {
	/* Intentionally left blank */
}




}}


