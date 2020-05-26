
#include "bcplus/symbols/SortSymbol.h"
#include "bcplus/symbols/NumberRangeSymbol.h"

#include "RangeData.h"



namespace cplus2asp {
namespace cplus2asp_bin {

namespace sy = bcplus::symbols;

RangeData::RangeData(sy::NumberRangeSymbol const* range)
	: _range(range), _translated(false) {
	/* Intentionally left blank */
}

RangeData::~RangeData() {
	/* Intentionally left blank */
}



}}

