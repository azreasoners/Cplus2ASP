#include "babb/utils/memory.h"
#include "bcplus/parser/Number.h"

namespace bcplus {
namespace parser {

Number::Number(int val, Location const& begin, Location const& end)
	: _val(val), _begin(begin), _end(end) {
	/* Intentionally Left Blank */
}

Number::~Number() {
	/* Intentionally Left Blank */
}

}}

