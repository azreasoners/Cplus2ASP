#include "bcplus/parser/detail/ScannerRawSource.h"


namespace bcplus {
namespace parser {
namespace detail {

ScannerRawSource::ScannerRawSource(Configuration const* config)
	: ScannerSource(ScannerSource::Type::RAW), _config(config), _cursor(NULL), _limit(NULL), 
	  _token(NULL), _marker(NULL), _ctxmarker(NULL) {
	// Intentionally left blank
}

ScannerRawSource::~ScannerRawSource() {
	// Intentionally left blank
}

}}}


