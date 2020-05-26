#pragma once

#include "babb/utils/memory.h"
#include "babb/utils/utils.h"
#include "memwrappers.h"

#include "bcplus/symbols/NumberRangeSymbol.h"
#include "bcplus/symbols/SortSymbol.h"

namespace cplus2asp {
namespace cplus2asp_bin {

/// Metadata container for sorts
class RangeData : public babb::utils::Referenced
{
public:
	/***************************************************************************/
	/* Public Types */
	/***************************************************************************/

	/// A set of sorts
	typedef std::set<babb::utils::weak_ptr<const bcplus::symbols::SortSymbol> > SortSet;

	typedef SortSet::iterator iterator;
	typedef SortSet::const_iterator const_iterator;

private:
	/***************************************************************************/
	/* Private Members */
	/***************************************************************************/

	/// The sort this data is attached to
	babb::utils::weak_ptr<const bcplus::symbols::NumberRangeSymbol> _range;

	/// Whether the sort declaration has already been translated.
	bool _translated;

	/// A list of the sorts for which this range has already be translated within
	SortSet _translatedSorts;

public:
	/***************************************************************************/
	/* Constructors / Destructors */
	/***************************************************************************/

	/// Basic constructor
	/// @param symbol The range this data is attached to
	RangeData(bcplus::symbols::NumberRangeSymbol const* symbol);

	/// Destructor stub
	virtual ~RangeData();

	/***************************************************************************/
	/* Public Members */
	/***************************************************************************/

	/// Get the sort this data is attached to
	inline bcplus::symbols::NumberRangeSymbol const* range() const				{ return _range; }

	/// get/set whether the sort declaration has been translated.
	inline bool translated() const												{ return _translated; }
	inline void translated(bool t) 												{ _translated = t; }

	/// Determine whether the range has already been translated within the provided sort
	inline bool translatedSort(bcplus::symbols::SortSymbol const* s) const		{ return _translatedSorts.count(s) != 0; }

	/// Registers the provided sort as having been translated with this range
	void addTranslatedSort(bcplus::symbols::SortSymbol const* s)				{ _translatedSorts.insert(s); }

};

}}

