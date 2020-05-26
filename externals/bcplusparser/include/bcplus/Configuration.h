#pragma once
#include <string>
#include <iostream>

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif
#include <boost/filesystem.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"

namespace bcplus {




/// Container for verbosity levels
struct Verb {

	enum type {
		ERROR = 0,					/// Error Messages
		WARN = 1,					/// warning messages
		STD = 2,					/// Standard Messages
		OP = 3,						/// Operation-level messages
		DETAIL = 4,					/// Detailed debug Messages

		// extremely verbose debug messages
		TRACE_PARSER = 10,			/// verbose parser trace messages
		TRACE_SYMTAB = 11,			/// verbose symbol table trace messages
		TRACE_MACRO_PARSER = 12,	/// verbose macro parser trace messages
		TRACE_SCANNER = 13			/// verbose scanner tracing messages


	};
};



class Configuration : public babb::utils::Referenced
{
public:
	/***************************************************************************/
	/* Constants */
	/***************************************************************************/

	/// Default verbosity level
	static const size_t DEFAULT_VERB_LEVEL;
	
	/***************************************************************************/
	/* Public types */
	/***************************************************************************/
	struct Status {
		enum type {
			STAT_OK,		///< Everything is a-ok.
			STAT_BAD_ARG,	///< The user provided an option we don't recognize.
			STAT_HELP,		///< The user needs help.
			STAT_VERSION,	///< The user wants the application version.
			STAT_NEXT_ARG	///< The next argument is needed in order to complete parsing the option.
		};
	};

	/// Container for option enumeration information
	struct Option {
			enum type {
				SYMTAB_INPUT,		///< Input file to initialize symbol table from.
				SYMTAB_OUTPUT,		///< Output file to save symbol table to.
				VERBOSITY,			///< Specify the application verbosity level.
				PARSE_TRACE,		///< Whether we should display lemon parser trace information.
				HELP,				///< Show help dialog.
				VERSION,			///< Show the application version
				MACRO_DEF,			///< A macro definition
				BAD					///< Unknown option.
		};
	};

	typedef std::pair<babb::utils::ref_ptr<const ReferencedString>, 
		babb::utils::ref_ptr<const ReferencedString> > MacroDefinition;
	typedef ReferencedList<MacroDefinition>::type MacroList;
	typedef MacroList::iterator iterator;
	typedef MacroList::const_iterator const_iterator;

private:
	/***************************************************************************/
	/* Private Static Members */
	/***************************************************************************/
	

	/***************************************************************************/
	/* Private Members */
	/***************************************************************************/
	babb::utils::ref_ptr<const ReferencedString> _version;		///< The version of this application.
	babb::utils::ref_ptr<const ReferencedString> _bin_name;		///< The name of the executable.

	babb::utils::ref_ptr<const ReferencedPath> _symtab_in;		///< Input file (if any) for the symbol table.
	babb::utils::ref_ptr<const ReferencedPath> _symtab_out;		///< Output file (if any) for the symbol table.
	size_t _verb;												///< The user defined verbosity level.	
	bool _parse_trace;											///< Whether we should display parser trace information.

	MacroList _macros;

public:
	/***************************************************************************/
	/* Constructors */
	/***************************************************************************/
	/// Default constructor
	/// Initializes all parameters to defaults.
	/// @param bin The name of the binary
	/// @param version The system version.
	Configuration(char const* bin = NULL, char const* version = NULL);

	/// Destructor
	virtual ~Configuration();

	/***************************************************************************/
	/* Public Methods */
	/***************************************************************************/
	
	/// Get the application version
	inline ReferencedString const* version() const				{ return _version; }

	/// Get the name of this executable file
	inline ReferencedString const* binaryName() const			{ return _bin_name; }

	/// Get/set the input file for the symbol table
	inline ReferencedPath const* symtabInput() const			{ return _symtab_in; }
	inline void symtabInput(ReferencedPath const* input)		{ _symtab_in = input; }

	/// Get/set the output file for the symbol table
	inline ReferencedPath const* symtabOutput() const			{ return _symtab_out; }
	inline void symtabOutput(ReferencedPath const* output)		{ _symtab_out = output; }
	
	/// Get/set the application verbosity level
	inline size_t verbosity() const								{ return _verb; }
	inline void verbosity(size_t verb)							{ _verb = verb; }

	/// Determine whether we should display parser trace information
	inline bool parseTrace() const								{ return _parse_trace; }
	inline void parseTrace(bool trace)							{ _parse_trace = trace; }

	/// iterate over the list of command line macros
	inline const_iterator begin() const							{ return _macros.begin(); }
	inline iterator begin()										{ return _macros.begin(); }
	inline const_iterator end() const							{ return _macros.end(); }
	inline iterator end()										{ return _macros.end(); }

	/// Add a macro definition
	inline void addMacro(ReferencedString const* name, ReferencedString const* def)
																{ _macros.push_back(MacroDefinition(name,def)); }
	inline void addMacro(MacroDefinition const& m)				{ _macros.push_back(m); }


	/// Get the output stream for the provided verbosity level
	/// This will return a null-sink, std::cerr, or std::cout.
	std::ostream& ostream(Verb::type v) const;

	/// Output a formatted error
	/// @param err The error message to output
	/// @param loc The locatio nthe error occurred (if any)
	void error(std::string const& err, Location const* loc = NULL) const;


	// --------------------------------------------------------------------------
	
	/// Loads configuration parameters from the provided arguments list.
	/// @param argc The number of arguments.
	/// @param argv An array of these arguments.
	/// @return The status of the load
	virtual int load(int argc, char const** argv);
	
	/// Verifies the integrity of the configuration parameters.
	/// @return True if the parameters appear to be correct. False otherwise.
	virtual bool good();

	// -------------------------------------------------------------------------

	/// Outputs the application version information.
	/// @param out The output stream to write to.
	/// @return out
	std::ostream& outputVersion(std::ostream& out) const;

	/// Outputs a help dialog to the provided output stream.
	/// @param out The output stream to write the help dialog to.
	/// @return out
	std::ostream& outputHelp(std::ostream& out) const;
	

protected:
	/***************************************************************************/
	/* Private Methods */
	/***************************************************************************/
	/// Parses the string option and performs the task specified by it
	/// @param opt The option string to parse.
	/// @param opt2 A second option which should be provided when STAT_NEXT_ARG is returned.
	/// @return A status code indicating whether or not it was a success.
	virtual int parseOption(char const* opt, char const* op2 = NULL);

	/// Outputs the options for this configuration
	/// FORMAT: (dash is a space)
	/// -----<category>:
	/// ----------<option>
	/// ----------<option2>
	/// ---------------<description>
	/// @param out The output stream to write to
	/// @return out.
	virtual std::ostream& outputOptions(std::ostream& out) const;

};

};
