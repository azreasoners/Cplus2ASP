#pragma once
#include <boost/filesystem.hpp>


#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Configuration.h"

namespace cplus2asp {
namespace cplus2asp_bin {


typedef bcplus::Verb Verb;

#define DEBUG


class Configuration : public bcplus::Configuration
{
public:
	/********************************************************************************/
	/* Public Types */
	/********************************************************************************/

	struct Status {
		enum type {
			STAT_OK = bcplus::Configuration::Status::STAT_OK,
			STAT_BAD_ARG = bcplus::Configuration::Status::STAT_BAD_ARG,
			STAT_HELP = bcplus::Configuration::Status::STAT_HELP,
			STAT_VERSION = bcplus::Configuration::Status::STAT_VERSION,
			STAT_NEXT_ARG = bcplus::Configuration::Status::STAT_NEXT_ARG,
			STAT_FILE_NOT_FOUND = 100
		};
	};

	struct Option {
		enum type {
			INPUT_MODE,				///< Input language mode
			OUTPUT_MODE,			///< output mode
			OUTPUT_FILE,			///< output file (if any)
			NO_DOMAIN_ENFORCE,		///< Indicates that we shouldn't produce domain-enforcing rules for non-additive constants
			NO_ADD_DOMAIN_ENFORCE,	///< Indicates that we shouldn't produce domain-enforcing rules for additive constants
			STATIC,					///< Whether to perform a static (non-incremental) translation.
			HELP,					///< Show the help dialog.
			VERSION,				///< Show the version dialog.
			BAD						///< Unknown option.
		};
	};

	struct Input {
		enum type {
			CPLUS,
			BCPLUS,
			BC,
			MVPF
		};
	};

	struct Output {
		enum type {
			STATIC,
			INCREMENTAL,
			INCREMENTAL_NO_DECL	///< Incremental without #base and #cumulative declarations
		};

	};


	/// The different time step classes
	struct TS {
		enum type {
			ZERO,			///< "0"
			STATIC,			///< fluents in static laws
			DYNAMIC,		///< fluents in dynamic laws
			ACTION,			///< actions / fluents of previous step
			MAXSTEP			///< The constants representing the maximum step
		};
	};

	
	typedef ReferencedList<babb::utils::ref_ptr<const ReferencedPath> >::type FileList;
	typedef FileList::iterator iterator;
	typedef FileList::const_iterator const_iterator;
	typedef FileList::reverse_iterator reverse_iterator;
	typedef FileList::const_reverse_iterator const_reverse_iterator;

	/********************************************************************************/
	/* Public Static Members */
	/********************************************************************************/


	/// Various timestamps
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_zero;				///< "0"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_static_incr;			///< "t"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_static_static;		///< "ST"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_dynamic_incr;		///< "t"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_dynamic_static;		///< "AT"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_action_incr;			///< "t-1"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_action_static;		///< "AT-1"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_maxstep_incr;		///< "t"
	static const babb::utils::ref_ptr<const ReferencedString> s_ts_maxstep_static;		///< "maxstep"

private:


	/********************************************************************************/
	/* Private Members */
	/********************************************************************************/
	/// The file to output to, if any
	babb::utils::ref_ptr<const ReferencedPath> _output_file;
	std::ostream* _ostream;

	/// The input language we're accepting
	Input::type _input_lang;

	/// The output language we're targeting
	Output::type _output_lang;

	/// The list of files to read from
	FileList _input_files;

	/// Whether we should enforce that constant's shouldn't take values outside their domain
	bool _enforceDomain;

	/// Whether we should enforce that additive constant's shouldn't take values outside their domains.
	bool _enforceAdditiveDomain;


public:
	/********************************************************************************/
	/* Constructors / Destructors */
	/********************************************************************************/

	/// Basic Constructor
	/// Initializes all parameters to default
	/// @param bin The name of the binary
	/// @param version The system version
	Configuration (char const* bin, char const* version);
	
	/// Destructor stub
	virtual ~Configuration();

	/********************************************************************************/
	/* Public Methods */
	/********************************************************************************/

	/// Clear the current output file (defaulting to std::cout)
	void clearOutputFile();

	/// Get/set the output file (and associated stream), if any
	inline ReferencedPath const* outputFile() const				{ return _output_file; }
	bool outputFile(ReferencedPath const* file);
	inline std::ostream& out() const							{ return *(const_cast<std::ostream*>(_ostream)); }

	/// Set the output stream to the provided stream (overrides the output file selection)
	bool outputStream(std::ostream& out);


	/// Get/set the input language
	inline Input::type inputLanguage() const					{ return _input_lang; }
	inline void inputLanguage(Input::type lang)					{ _input_lang = lang; }
	
	/// Get/set the output language
	inline Output::type outputLanguage() const					{ return _output_lang; }
	inline void outputLanguage(Output::type lang)				{ _output_lang = lang; }

	/// Get/set whether we're enforcing contant domains
	inline bool enforceDomain() const							{ return _enforceDomain; }
	inline void enforceDomain(bool enforce)						{ _enforceDomain = enforce; }

	/// Get/set whether we're enforcing additive constant domains
	inline bool enforceAdditiveDomain() const					{ return _enforceAdditiveDomain; }
	inline void enforceAdditiveDomain(bool enforce)				{ _enforceAdditiveDomain = enforce; }

	/// Get the timestamp to be used for the provided timestamp class
	/// @param ts_class The timestamp class to get
	ReferencedString const* ts(TS::type ts_class) const;


	/// Add an input file to the beginning of the list
	/// @param file The input file to add
	inline void push_front(ReferencedPath const* path)			{ _input_files.push_front(path); }
	
	/// Add an input file to the end of the list
	/// @param file The input file to add
	inline void push_back(ReferencedPath const* path)			{ _input_files.push_back(path); }

	/// Iterate over the set of input files
	inline const_iterator begin() const							{ return _input_files.begin(); }
	inline iterator begin()										{ return _input_files.begin(); }
	inline const_iterator end() const							{ return _input_files.end(); }
	inline iterator end() 										{ return _input_files.end(); }
	
	inline const_reverse_iterator rbegin() const				{ return _input_files.rbegin(); }
	inline reverse_iterator rbegin()							{ return _input_files.rbegin(); }
	inline const_reverse_iterator rend() const					{ return _input_files.rend(); }
	inline reverse_iterator rend() 								{ return _input_files.rend(); }

	// inherited
	virtual bool good();

protected:
	// inherited
	virtual std::ostream& outputOptions(std::ostream& out) const;
	virtual int parseOption(char const* opt, char const* opt2 = NULL);


};





}}

