
#include "bcplus/symbols/SortSymbol.h"

#include "SortData.h"



namespace cplus2asp {
namespace cplus2asp_bin {

namespace sy = bcplus::symbols;

SortData::SortData(sy::SortSymbol const* sort)
	: _sort(sort), _translated(false) {
	/* Intentionally left blank */
}

SortData::~SortData() {
	/* Intentionally left blank */
}


void SortData::addTranslatedSubSort(sy::SortSymbol const* s) {

	if (_translatedSubSorts.insert(s).second) {

		// add our sort to their translated super sorts
		//s->metadata<SortData>()->addTranslatedSuperSort(sort());
		((SortData*)(s->metadata()))->addTranslatedSuperSort(sort());
	}
}

void SortData::addTranslatedSuperSort(sy::SortSymbol const* s) {

	if (_translatedSuperSorts.insert(s).second) {

		// add our sort to their translated sub sorts
		//s->metadata<SortData>()->addTranslatedSubSort(sort());
		((SortData*)(s->metadata()))->addTranslatedSubSort(sort());
	}
}


}}

