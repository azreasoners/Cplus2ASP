#pragma once

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "babb/utils/memory.h"


#include "bcplus/Location.h"
#include "bcplus/parser/detail/ScannerRawSource.h"


namespace bcplus {
namespace parser {
namespace detail {


/// A simple class to track files being worked on by the scanner
class ScannerFileSource : public ScannerRawSource {

private:
	/***********************************************************/
	/* Private Members */
	/***********************************************************/

	/// Input file/stream
	babb::utils::ref_ptr<ReferencedPath> _file;
	boost::filesystem::fstream _input;

	/// Buffer information
	char* _buffer;
	size_t _buffer_sz;

	/// File position
	size_t _line;
	char const* _newline;

public:
	/***********************************************************/
	/* Constructors */
	/***********************************************************/
	/// @param config The system wide configuration information
	/// @param file The file to read from
	/// @param squelch Whether to silence all errors resulting from this call.
	ScannerFileSource(Configuration const* config, boost::filesystem::path const& file, bool squelch = false);

	/// Destructor
	virtual ~ScannerFileSource();

	/***********************************************************/
	/* Public Functions */
	/***********************************************************/

	/// Gets the current file being read from.
	inline ReferencedPath const* file() const					{ return _file; }

	/// Gets the current line number
	inline size_t line() const									{ return _line; }
	
	/// Gets the current column number
	inline size_t col() const									{ return (size_t)(token() - _newline + 1); }


	// inherited stuffs
	virtual void newline();
	virtual Location loc() const;
	virtual Status::type status() const;
	virtual void close();
	virtual void fill(size_t n);

};

}}}

