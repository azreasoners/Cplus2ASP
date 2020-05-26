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


/// A scanner source which draws directly from a pre-defined in-memory buffer
class ScannerBufferSource : public ScannerRawSource {

private:
	/***********************************************************/
	/* Private Members */
	/***********************************************************/


	/// Buffer information
	char* _buffer;
	size_t _buffer_sz;

	/// File position
	babb::utils::ref_ptr<const ReferencedPath> _file;
	size_t _line;
	char const* _newline;
	
	/// The initial column location provided during initialization
	size_t _first_col;

	/// Whether we're tracking position or not
	bool _track;

public:
	/***********************************************************/
	/* Constructors */
	/***********************************************************/
	/// @param config The system wide configuration information
	/// @param buffer The null-terminated buffer to read from.
	/// @param loc The location that should be displayed in output messages partaining to this source
	/// @param track_position Whether the position relative to the starting position should be tracked.
	ScannerBufferSource(Configuration const* config, char const* buffer, Location const& loc = Location(NULL, 0, 0), bool track_position = false);

	/// Destructor
	virtual ~ScannerBufferSource();

	/***********************************************************/
	/* Public Functions */
	/***********************************************************/

	/// The file provided as the location
	inline ReferencedPath const* file() const					{ return _file; }

	/// The current line number (or the provided line number if we're not tracking position)
	inline size_t line() const									{ return _line; }

	/// Gets the current column (or the provided column if we're not tracking position)
	inline size_t col() const									{ return (_track ? (size_t)(token() - _newline + 1) : _first_col); }

	// inherited stuffs
	virtual void newline();
	virtual Location loc() const;
	virtual Status::type status() const;
	virtual void close();
	virtual void fill(size_t n);

};

}}}

