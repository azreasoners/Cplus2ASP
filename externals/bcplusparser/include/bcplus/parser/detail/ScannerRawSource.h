#pragma once

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "babb/utils/memory.h"

#include "bcplus/Configuration.h"
#include "bcplus/parser/detail/ScannerSource.h"
#include "bcplus/Location.h"


namespace bcplus {
namespace parser {
namespace detail {

/// A simple base class for working with various raw text sources in the scanner
class ScannerRawSource : public ScannerSource {
public:
	/***********************************************************/
	/* Public Types */
	/***********************************************************/

	struct Status {
			enum type {
				OK,
				FAIL,
				CLOSED,
				END_INPUT
			};
	};

private:
	
	/***********************************************************/
	/* Private Members */
	/***********************************************************/

	/// Configuration
	babb::utils::ref_ptr<const Configuration> _config;

	/// Scanner State
	char const* _cursor;
	char const* _limit;
	char const* _token;
	char const* _marker;
	char const* _ctxmarker;

public:
	/***********************************************************/
	/* Constructors */
	/***********************************************************/
	/// @param config The system wide configuration information
	ScannerRawSource(Configuration const* config);

	/// Destructor
	virtual ~ScannerRawSource();

	/***********************************************************/
	/* Public Functions */
	/***********************************************************/

	/// Gets the system configuration
	inline Configuration const* config() const	{ return _config; }

	/// Scanner state accessors / mutators

	inline char const*& cursor()				{ return _cursor; }
	inline char const* const& cursor() const	{ return _cursor; }
	inline void cursor(char const* c)			{ _cursor = c; }

	inline char const*& limit() 				{ return _limit; }
	inline char const* const& limit() const		{ return _limit; }
	inline void limit(char const* l)			{ _limit = l; }
	
	inline char const*& token() 				{ return _token; }
	inline char const* const& token() const		{ return _token; }
	inline void token(char const* t)			{ _token = t; }
	
	inline char const*& marker() 				{ return _marker; }
	inline char const* const& marker() const	{ return _marker; }
	inline void marker(char const* m)			{ _marker = m; }
	
	inline char const*& ctxmarker() 			{ return _ctxmarker; }
	inline char const* const& ctxmarker() const	{ return _ctxmarker; }
	inline void ctxmarker(char const* m)		{ _ctxmarker = m; }


	/// Determines if the source is ready to read from
	inline bool good() const					{ return status() == Status::OK; }
	inline bool eof() const						{ return status() == Status::END_INPUT; }

	/// Callback which indicates that the scanner found a newline and the location should be updated
	virtual void newline() = 0;

	/// Gets the current location of the source
	virtual Location loc() const = 0;
	
	/// Determines the status of the source
	virtual Status::type status() const = 0;

	/// Closes the source
	virtual void close() = 0; 
	
	/// Refills the buffer to the desired level
	/// @param n The minimum number of characters to fill.
	virtual void fill(size_t n) = 0;

};

}}}


