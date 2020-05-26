#include "babb/utils/memory.h"
#include "bcplus/parser/detail/NumberRange.h"
#include "bcplus/elements/terms.h"


namespace bcplus {
namespace parser {
namespace detail {

NumberRange::NumberRange(elements::Term* min, elements::Term* max, Location const& begin, Location const& end)
	: _min(min), _max(max), _begin(begin), _end(end) {
	/* Intentionally Left Blank */
}

NumberRange::~NumberRange() {
	/* Intentionally Left Blank */
}

}}}

