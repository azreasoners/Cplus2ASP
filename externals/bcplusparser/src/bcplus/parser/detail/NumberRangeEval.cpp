#include "babb/utils/memory.h"
#include "bcplus/parser/detail/NumberRangeEval.h"

namespace bcplus {
namespace parser {
namespace detail {

NumberRangeEval::NumberRangeEval(int min, int max, Location const& begin, Location const& end)
	: _min(min), _max(max), _begin(begin), _end(end) {
	/* Intentionally Left Blank */
}

NumberRangeEval::~NumberRangeEval() {
	/* Intentionally Left Blank */
}

}}}

