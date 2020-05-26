#pragma once

#include "bcplus/elements/Element.h"
#include "bcplus/elements/terms.h"
#include "bcplus/elements/formulas.h"

namespace cplus2asp {
namespace cplus2asp_bin {

/// A base class for recursing over formulas to identify properties
/// @param t The return type of the predicate
template <typename t>
class FormulaPredicate
{
private:
	/*****************************************************************************/
	/* Private Members */
	/*****************************************************************************/



public:

	/// Traverse over the provided element hierarchy performining some calculation.
	/// @param init The initial value for the 
	/// @return The return value for the calculation
	t apply(bcplus::elements::Element const* node);


private:

	/// Internal traversal calls
	bool traverse(bcplus::elements::Element const* node, t& ret);
	bool traverse(bcplus::elements::BinaryFormula const* node, t& ret);
	bool traverse(bcplus::elements::ComparisonFormula const* node, t& ret);
	bool traverse(bcplus::elements::QuantifierFormula const* node, t& ret);
	bool traverse(bcplus::elements::CardinalityFormula const* node, t& ret);
	bool traverse(bcplus::elements::AtomicFormula const* node, t& ret);
	bool traverse(bcplus::elements::UnaryFormula const* node, t& ret);
	bool traverse(bcplus::elements::NullaryFormula const* node, t& ret);

	bool traverse(bcplus::elements::BinaryTerm const* node, t& ret);
	bool traverse(bcplus::elements::UnaryTerm const* node, t& ret);
	bool traverse(bcplus::elements::LuaTerm const* node, t& ret);
	bool traverse(bcplus::elements::PyTerm const* node, t& ret);
	bool traverse(bcplus::elements::AnonymousVariable const* node, t& ret);
	bool traverse(bcplus::elements::Object const* node, t& ret);
	bool traverse(bcplus::elements::Variable const* node, t& ret);
	bool traverse(bcplus::elements::Constant const* node, t& ret);
	bool traverse(bcplus::elements::NullaryTerm const* node, t& ret);

protected:

	/// Initialize the return value to the default value.
	virtual void _init(t& ret);

	/// Call used to perform any processing on the provided node
	/// @param node The node to process
	/// @param[inout] ret The current return value for the predicate to be (possibly) modified.
	/// @return true if the return value is final and the process should be halted.
	virtual bool _apply(bcplus::elements::BinaryFormula const* node, t& ret);
	virtual bool _apply(bcplus::elements::ComparisonFormula const* node, t& ret);
	virtual bool _apply(bcplus::elements::QuantifierFormula const* node, t& ret);
	virtual bool _apply(bcplus::elements::CardinalityFormula const* node, t& ret);
	virtual bool _apply(bcplus::elements::AtomicFormula const* node, t& ret);
	virtual bool _apply(bcplus::elements::UnaryFormula const* node, t& ret);
	virtual bool _apply(bcplus::elements::NullaryFormula const* node, t& ret);

	virtual bool _apply(bcplus::elements::BinaryTerm const* node, t& ret);
	virtual bool _apply(bcplus::elements::UnaryTerm const* node, t& ret);
	virtual bool _apply(bcplus::elements::LuaTerm const* node, t& ret);
	virtual bool _apply(bcplus::elements::PyTerm const* node, t& ret);
	virtual bool _apply(bcplus::elements::AnonymousVariable const* node, t& ret);
	virtual bool _apply(bcplus::elements::Object const* node, t& ret);
	virtual bool _apply(bcplus::elements::Variable const* node, t& ret);
	virtual bool _apply(bcplus::elements::Constant const* node, t& ret);
	virtual bool _apply(bcplus::elements::NullaryTerm const* node, t& ret);

};

}}
