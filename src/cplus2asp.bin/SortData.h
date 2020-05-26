#pragma once

#include "babb/utils/memory.h"
#include "babb/utils/utils.h"
#include "memwrappers.h"

#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/VariableSymbol.h"

namespace cplus2asp {
namespace cplus2asp_bin {

/// Metadata container for sorts
class SortData : public babb::utils::Referenced
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
	babb::utils::weak_ptr<const bcplus::symbols::SortSymbol> _sort;

	/// Whether the sort declaration has already been translated.
	bool _translated;

	/// A list of the sub sorts which have already been translated
	SortSet _translatedSubSorts;

	/// A list of the super sorts which have
	SortSet _translatedSuperSorts;


public:
	/***************************************************************************/
	/* Constructors / Destructors */
	/***************************************************************************/

	/// Basic constructor
	/// @param symbol The constant this data is attached to
	SortData(bcplus::symbols::SortSymbol const* symbol);

	/// Destructor stub
	virtual ~SortData();

	/***************************************************************************/
	/* Public Members */
	/***************************************************************************/

	/// Get the sort this data is attached to
	inline bcplus::symbols::SortSymbol const* sort() const						{ return _sort; }

	/// get/set whether the sort declaration has been translated.
	inline bool translated() const												{ return _translated; }
	inline void translated(bool t) 												{ _translated = t; }

	/// Determine whether the provided subsort/supersort relation has already been translated
	inline bool translatedSubSort(bcplus::symbols::SortSymbol const* s) const		{ return _translatedSubSorts.count(s) != 0; }
	inline bool translatedSuperSort(bcplus::symbols::SortSymbol const* s) const		{ return _translatedSuperSorts.count(s) != 0; }

	/// Registers the provided sort as being a translated sub/super sort relation.
	/// These also update the provided sort's metadata
	void addTranslatedSubSort(bcplus::symbols::SortSymbol const* s);
	void addTranslatedSuperSort(bcplus::symbols::SortSymbol const* s);

};

}}

