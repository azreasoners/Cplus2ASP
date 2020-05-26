#pragma once

#include <string>
#include <ostream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

namespace bcplus {

/// A simple location structure
class Location {
private:
	babb::utils::ref_ptr<const ReferencedPath > _file;
	size_t _line;
	size_t _col;

public:

	/// Default constructor
	Location();

	/// Full constructor.
	Location(ReferencedPath const* file, size_t line, size_t col);

	/// Destructor
	virtual ~Location();

	/// Accessors
	inline ReferencedPath const* file() const								{ return _file; }
	inline size_t line() const												{ return _line; }
	inline size_t col() const												{ return _col; }

	/// Mutators
	inline void file(ReferencedPath const* f)								{ _file = f; }
	inline void line(size_t l)												{ _line = l; }
	inline void col(size_t c)												{ _col = c; }
	
	/// Generate the name of the file or use a placeholder if it's null
	std::string filename() const;

	/// Output the location to the provided stream
	/// @param out The stream to write to.
	void output(std::ostream& out) const;	

	/// Helper function for output to generate a string
	inline std::string str() const											{ std::stringstream str; output(str); return str.str(); }

};




}

inline std::ostream& operator<<(std::ostream& out, bcplus::Location const& loc) {
	loc.output(out);
	return out;
}

