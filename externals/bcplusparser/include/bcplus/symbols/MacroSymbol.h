#pragma once

#include <string>
#include <ostream>

#include <boost/property_tree/ptree.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/DomainType.h"
#include "bcplus/elements/Element.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/parser/Token.h"

namespace bcplus {
namespace symbols {

class MacroSymbol : public Symbol {
public:
	/****************************************************************************/
	/* Public Types */
	/****************************************************************************/
	typedef ReferencedVector<babb::utils::ref_ptr<const ReferencedString> >::type ArgumentList;
	typedef ArgumentList::iterator iterator;
	typedef ArgumentList::const_iterator const_iterator;

	typedef ReferencedVector<babb::utils::ref_ptr<const elements::Element> >::type ElementList;
	typedef ReferencedVector<babb::utils::ref_ptr<const parser::Token> >::type TokenList;

private:
	/****************************************************************************/
	/* Private Members */
	/****************************************************************************/
	/// List of expansion arguments
	babb::utils::ref_ptr<const ArgumentList> _args;
	
	/// The text to expand
	babb::utils::ref_ptr<const ReferencedString> _text;

	/// Mapping from sorted argument ordering to physical argument ordering.
	size_t* _arg_map;

public:
	/****************************************************************************/
	/* Constructors / Destructors */
	/****************************************************************************/
	/// Initializes the macro symbol
	/// @param base The base name for the macro.
	/// @param text The text that will be used as an expansion template.
	/// @param args formal arguments for the macro to be expanded (or NULL for no arguments).
	MacroSymbol(ReferencedString const* base, ReferencedString const* text, ArgumentList* args = NULL);

	/// Attempts to load the symbol from the property tree node.
	/// @param node The node to load the symbol from
	/// @param err An error stream to write to or NULL.
	/// Sets the symbol's good flag if it was successful.
	MacroSymbol(boost::property_tree::ptree const& node, std::ostream* err);

	/// Destructor
	virtual ~MacroSymbol();

	/****************************************************************************/
	/* Public Functions */
	/****************************************************************************/


	/// Iterate over the formal arguments
	inline const_iterator begin() const							{ return _args->begin(); }
	inline const_iterator end() const							{ return _args->end(); }


	/// Get the macro definition text
	inline ReferencedString const* text() const					{ return _text; }

	/// Expand the macro given the provided arguments list
	std::string expand(ArgumentList const* args = NULL) const;
	std::string expand(ElementList const* args) const;
	std::string expand(TokenList const* args) const;

	// inherited
 	virtual void save(boost::property_tree::ptree& node) const;
	virtual void outputDefinition(std::ostream& out) const;
	DomainType::type domainType() const;

private:

	/// Compiles the argument mapping from expansion order to real order.
	/// This allows us to expand elements in a way that we don't accidentally
	/// expand #xy with the definition of #x.
	void compileArgMap();

};

}}
