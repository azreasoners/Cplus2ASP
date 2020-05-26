
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "bcplus/parser/detail/ScannerRawSource.h"
#include "bcplus/parser/detail/ScannerFileSource.h"

namespace fs = boost::filesystem;

namespace bcplus {
namespace parser {
namespace detail {

ScannerFileSource::ScannerFileSource(Configuration const* c, fs::path const& file, bool squelch)
	: ScannerRawSource(c), _buffer(NULL), _buffer_sz(0), _line(1), _newline(NULL) {


	// Ensure the file exists
	if (!squelch && (!fs::exists(file) || !fs::is_regular_file(file))) {
		config()->ostream(Verb::ERROR) << "ERROR: Cannot load input from file \"" << file.native() << "\". The file does not exist." << std::endl;
	}

	_file = new ReferencedPath();

	// try to open the new file
	_input.open(file);
	if (!squelch && !_input.good()) {
		config()->ostream(Verb::ERROR) << "ERROR: An error occurred while opening \"" << file.native() <<"\"." << std::endl;
		_input.close();
		_input.clear();
	}	
	
	// save the detail
	*_file += file;

	fill(10);
}

ScannerFileSource::~ScannerFileSource() {
	if (_buffer) delete[] _buffer;
	if (_input.is_open()) _input.close();
}


void ScannerFileSource::newline() {
	_line++;
	_newline = cursor();
}

Location ScannerFileSource::loc() const {
	return Location(file(), line(), col());
}


void ScannerFileSource::close() {
	if (_input.is_open()) _input.close();
}

ScannerFileSource::Status::type ScannerFileSource::status() const {
	if (_input.good() || (_input.eof() && cursor() && *cursor())) return Status::OK;
	else if (_input.eof()) return Status::END_INPUT;
	else if (!_input.is_open()) return Status::CLOSED;
	else return Status::FAIL;
}


void ScannerFileSource::fill(size_t n) {
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
	ctxmarker() = _buffer + ctxmarker_offset;
	token() = _buffer + token_offset;
	_newline = _buffer + newline_offset;

	// Fill the remainder of the buffer
	char* readpos = _buffer + remaining;
	int read_amount = _buffer_sz - remaining;

	_input.read(readpos, read_amount);

	// if we hit the end fill with EOF
	if (_input.gcount() < read_amount) {
		readpos += _input.gcount();
		memset(readpos, '\0', read_amount - _input.gcount());
	}

	if (_input.fail() && !_input.eof()) {
		config()->ostream(Verb::ERROR) << "ERROR: An error occurred while reading from \"" << _file->native() << "\"." << std::endl;
	}
}
}}}

