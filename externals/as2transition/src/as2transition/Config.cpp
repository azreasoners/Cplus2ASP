
#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "as2transition/Config.h"

namespace as2transition {


Config::Config(int showmask, PredFormat::type format, ReferencedString const* noneAlias,
	bool stripSanitization, bool stripPrefix, bool separateExternal, bool separateActions,
	bool onePredPerLine)
	: _showmask(showmask), _format(format), _stripSanitization(stripSanitization),
	  _stripPrefix(stripPrefix), _noneAlias(noneAlias), _separateExternal(separateExternal),
	  _separateActions(separateActions), _solver(Solver::UNKNOWN) {
	// intentionally left blank
}

Config::Config(Config const& other) 
	: _showmask(other._showmask), _format(other._format), _stripSanitization(other._stripSanitization),
	  _stripPrefix(other._stripPrefix), _noneAlias(other._noneAlias), _separateExternal(other._separateExternal),
	  _separateActions(other._separateActions), _solver(other._solver) {
	// Intentionally left blank
}



Config::~Config() {
	// intentionally left blank
}



}
