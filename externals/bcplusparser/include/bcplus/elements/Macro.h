#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/symbols/MacroSymbol.h"

#include "bcplus/Location.h"
#include "bcplus/elements/Element.h"
#include "bcplus/elements/detail/IdentifierElement.h"
#include "bcplus/elements/terms.h"
#include "bcplus/parser/Token.h"

namespace bcplus {
namespace elements {
/**
 * @brief An arbitrary macro instance 'm(...)'.
 */
class Macro : public detail::IdentifierElement<Element, Element::Type::MACRO, symbols::MacroSymbol, parser::Token>
{
public:

	/************************************************************************************************/
	/* Public Types */
	/************************************************************************************************/
	
private:
	/************************************************************************************************/
	/* Private Members */
	/************************************************************************************************/

	/// The expansion of this macro
	babb::utils::ref_ptr<const ReferencedString> _expansion;

public:
	/************************************************************************************************/
	/* Constructors / Destructors */
	/************************************************************************************************/
	/// Full constructor
    /// @param symbol The symbol entry in the symbol table.
    /// @param args The argument list for this macro.
    /// @param begin The beginning location of this macro
    /// @param end The ending location of this macro
    /// @param parens Whether the macro is surrounded by parentheses
	Macro(symbols::MacroSymbol const* symbol, ArgumentList const* args, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0), bool parens = false);

	/// Stub destructor
	virtual ~Macro();

	/************************************************************************************************/
	/* Public Functions */
	/************************************************************************************************/

	/// Expand this macro into its final form.
	inline ReferencedString const* expand() const								{ return _expansion; }

};

}}
