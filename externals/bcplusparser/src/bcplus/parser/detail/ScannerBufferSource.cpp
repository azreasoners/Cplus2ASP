#include <cstring>

#include "bcplus/parser/detail/ScannerRawSource.h"
#include "bcplus/parser/detail/ScannerBufferSource.h"

namespace fs = boost::filesystem;

namespace bcplus {
namespace parser {
namespace detail {

ScannerBufferSource::ScannerBufferSource(Configuration const* config, char const* buffer, Location const& loc, bool track_position)
	: ScannerRawSource(config), _file(loc.file()), _line(loc.line()), _first_col(loc.col()), _track(track_position) {
	
	// figure out the size of the buffer
	_buffer_sz = strlen(buffer) + 1;
	_buffer = new char[_buffer_sz];
	memcpy(_buffer, buffer, _buffer_sz);

	cursor() = _buffer;
	token() = _buffer;
	marker() = _buffer;
	ctxmarker() = _buffer;
	limit() = _buffer + _buffer_sz;
	_newline = _buffer - loc.col();
}

ScannerBufferSource::~ScannerBufferSource() {
	delete[] _buffer;
}

void ScannerBufferSource::newline() {
	if (_track) {
		_line++;
		_newline = cursor();
	}
}

Location ScannerBufferSource::loc() const {
	return Location(file(), line(), col());
}

ScannerBufferSource::Status::type ScannerBufferSource::status() const {
	if (cursor() && !*cursor()) return Status::END_INPUT;
	else return Status::OK;
}



void ScannerBufferSource::close() {
	// intentionaly left blankd
}


void ScannerBufferSource::fill(size_t n) {


	char const* first = (token() < marker()) ? token() : marker();
	first = (first < ctxmarker() ? first : ctxmarker());

    size_t remaining = limit() - first;
    size_t req_sz = remaining + n;

    if (req_sz > _buffer_sz) {
        // oh noes. need more buffer
        size_t new_sz = req_sz*2;
        char* new_buf = new char[new_sz];
        memcpy(new_buf, first, remaining);
        delete[] _buffer;
        _buffer = new_buf;
        _buffer_sz = new_sz;
        limit() = new_buf + new_sz;
    } else {
        // we can keep the buffer and shift the contents
        memmove(_buffer, first, remaining);
    }

    size_t marker_offset = (size_t)(marker() - first);
    size_t ctxmarker_offset = (size_t)(ctxmarker() - first);
    size_t token_offset = (size_t)(token() - first);
    size_t cursor_offset = (size_t)(cursor() - first);
	size_t newline_offset = (size_t)(_newline - first);

    cursor() = _buffer + cursor_offset;
    marker() = _buffer + marker_offset;
    token() = _buffer + token_offset;
	ctxmarker() = _buffer + ctxmarker_offset;
	_newline = _buffer + newline_offset;

    // Fill the remainder of the buffer
    char* readpos = _buffer + remaining;
    int read_amount = _buffer_sz - remaining;
	memset(readpos,'\0', read_amount);

}
}}}

