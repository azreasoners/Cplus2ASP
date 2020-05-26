
#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"



namespace bcplus {

Location::Location() 
	: _line(0), _col(0) {
	// intentionally left blank
}

Location::Location(ReferencedPath const* file, size_t line, size_t col)
	: _file(file), _line(line), _col(col) {
	// intentionally left blank
}

Location::~Location() {
	// intentionally left blank
}

std::string Location::filename() const {
	return (_file ? _file->native() : std::string("<NULL>"));
}

void Location::output(std::ostream& out) const {
	out << "[" << filename() << ":" << line() << ":" << col() << "]";
}

}

