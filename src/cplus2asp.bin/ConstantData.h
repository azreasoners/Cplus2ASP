#pragma once

#include "babb/utils/memory.h"
#include "babb/utils/utils.h"
#include "memwrappers.h"

#include "bcplus/symbols/ConstantSymbol.h"
#include "bcplus/symbols/VariableSymbol.h"

namespace cplus2asp {
namespace cplus2asp_bin {

/// Metadata container for sorts
class ConstantData : public babb::utils::Referenced
{
public:
	/***************************************************************************/
	/* Public Types */
	/***************************************************************************/

private:
	/***************************************************************************/
	/* Private Members */
	/***************************************************************************/

	babb::utils::weak_ptr<const bcplus::symbols::ConstantSymbol> _constant;

	/// Whether the constant declaration has already been translated.
	bool _translated;

public:
	/***************************************************************************/
	/* Constructors / Destructors */
	/***************************************************************************/

	/// Basic constructor
	/// @param symbol The constant this data is attached to
	ConstantData(bcplus::symbols::ConstantSymbol const* symbol);

	/// Destructor stub
	virtual ~ConstantData();

	/***************************************************************************/
	/* Public Members */
	/***************************************************************************/

	/// get/set whether the constant declaration has been translated.
	inline bool translated() const												{ return _translated; }
	inline void translated(bool t) 												{ _translated = t; }

	/// Get the constant this data is bound to
	inline bcplus::symbols::ConstantSymbol const* constantSymbol()				{ return _constant; }

};

}}

