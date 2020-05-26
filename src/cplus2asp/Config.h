/* 
 * Copyright (c) 2010-2013 <Joseph Babb, Michael Cassollary, Joohyung Lee>
 *
 * For information on how to contact the authors, please visit
 *	http://reasoning.eas.asu.edu/cplus2asp
 *
 * This file is part of the cplus2asp system.
 *
 * cplus2asp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cplus2asp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
 * Config.h
 *
 *  Created on: Jan 23, 2013
 *      Author: Joseph Babb
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <vector>
#include <map>
#include <list>

/*********************************************************************************************/
/* Configuration Defaults */
/*********************************************************************************************/


#define DEF_RUNNING_MODE					MODE_INCREMENTAL					///< The default running mode.
#define DEF_LANG							LANG_CPLUS							///< The default input language.
#define DEF_DISCARD_F2LP					true								///< Whether to discard intermediate F2LP files by default.
#define DEF_SUPPRESS_INTERACTION			false								///< Whether to suppress user interaction by default.
#define DEF_NONE_HACK						false								///< Whether we should simulate none being an integral type by default.
#define DEF_SHIFT							false								///< Whether we should assert the --shift flag, which shifts disjunction into the body of the rules.

#define DEF_NUM_SOLN						1									///< Default # of solutions to find.
#define DEF_INTERNAL_PORT					35981								///< Default port # to use to communicate w/ oClingo.

#define DEF_STATS							false								///< Default for whether we should show solver stats.

#define DEF_NO_DOMAIN_ENFORCE				false								///< Default for whether we should suppress domain enforcing rules from being generated
#define DEF_NO_ADD_DOMAIN_ENFORCE			false								///< Default for whether we should suppress domain enforcing rules from being generated for additive constants

// Default commands
#define DEF_TRANS_CMD						"cplus2asp3.bin"					///< Default translator command.
#define DEF_PREPROC_CMD						"f2lp"								///< Default preprocessor command.
#define DEF_C4_PREPROC_CMD					"clingo3to4"						///< Default clingo3to4 preprocessor command.
#define DEF_GRD_CMD							""									///< Default grounder command.
#define DEF_SO_CMD							"clingo"							///< Default solver command.
#define DEF_POSTPROC_CMD					"as2transition"						///< Default postprocessor command.
#define DEF_REACTIVE_BRIDGE_CMD				"bcbridge"							///< Default reactive client command. TODO

#define DEF_GRD_REACTIVE_CMD				""									///< The default grounder to use in reactive mode.
#define DEF_SO_REACTIVE_CMD					"clingo"							///< The default solver to use in reactive mode.

#define DEF_GRD_STATIC_CMD 					""									///< The default grounder to use in static mode.
#define DEF_SO_STATIC_CMD 					"clingo"							///< The default solver to use in static mode.

#define DEF_SO_INCREMENTAL_CMD				"clingo"							///< The default solver to use in incremental mode.
#define DEF_GRD_INCREMENTAL_CMD				""									///< The default grounder to use in incremental mode.

// Default command options
#define DEF_TRANS_OPTS						""									///< Default translator options.
#define DEF_PREPROC_OPTS					""									///< Default preprocessor options.
#define DEF_C4_PREPROC_OPTS					""									///< Default clingo3to4 preprocessor options.
// #define DEF_GRD_OPTS						""									///< Default grounder options.
#define DEF_SO_OPTS							""									///< Default solver options.
#define DEF_GRSO_OPTS						""									///< Default grounder/solver options.
#define DEF_POSTPROC_OPTS					""									///< Default postprocessor options.
#define DEF_REACTIVE_BRIDGE_OPTS			""									///< Default reactive client

// f2lp files
#define F2LP_INPUT_FILE						".f2lp_input.fof"
#define F2LP_OUTPUT_FILE					".solver_input.lp"

#define DEF_INT_TRANS_FILE					".cplus2asp_trans.out"				///< Name of the intermediate output from the translator.
#define DEF_INT_PREPROC_FILE				".cplus2asp_pre.out"				///< Name of our intermediate output from the pre-processor.
#define DEF_INT_GRD_FILE 					".cplus2asp_gr.out"					///< Name of our intermediate output from the grounder.
#define DEF_INT_SO_FILE						".cplus2asp_so.out"					///< Name of our intermediate output from the solver.

// Known constant names
#define CONST_MAXSTEP						"maxstep"							///< Specifies the maximum step to use.
#define CONST_MINSTEP						"minstep"							///< Specifies the minimum step to use.
#define CONST_QUERY							"query"								///< Specifies the query to run.
#define CONST_MAXADDITIVE					"maxAdditive"						///< Specifies the maximum additive value to use.
#define CONST_DEFAULT						"default"							///< Specifies a default value.

// special query values
#define CONST_QUERY_NONE					"sat"								///< The value to provide to the query to indicate that no query should be applied.
#define CONST_QUERY_STATES					"states"							///< The value to provide to the query to indicate that we should find the system's states.
#define CONST_QUERY_TRANSITIONS				"transitions"						///< The value to provide to the query to indicate that we should find the system's transitions.
#define CONST_MAXSTEP_INFINITE				"infinite"							///< Value used to set the maximum step to infinite.


// etc
#define DEF_NONE_HACK_VAL					"50000"								///< The value to treat none as if we're performing the gringo none hack.

/**
 * A basic structure to hold and manage various configuration options for the cplus2asp system.
 */
class Config {
public:
	/***************************************************************/
	/* Types */
	/***************************************************************/
	/// A list of each component of the toolchain.
	enum Toolchain
	{
		_TC_BEGIN_ = 0x00,						///< An alias for the beginning of the toolchain.

		TC_TRANSLATOR = 0x00,					///< translator
		TC_PREPROC = 0x01,						///< pre-processor
		// TC_GROUNDER = 0x02,						///< grounder
		TC_C4_PREPROC = 0x02,
		TC_SOLVER = 0x03,						///< solver
		TC_POSTPROC = 0x04,						///< post-processor. Ignored in reactive mode.
		TC_REACTIVE_BRIDGE = 0x05,				///< The process to launch which interacts with a reactive solver.

		_TC_END_ = 0x06,						///< Dummy constant to provide the end of the toolchain command list.
		_TC_LENGTH_ = 0x06						///< (Not a tool) Provides the total length of the toolchain command.
	};


	/// The various operating modes that we can use.
	enum Mode
	{
		MODE_UNKNOWN = 0x00,					///< Unknown running mode. This should _only_ be used when specifying multiple modes.
		MODE_STATIC_AUTO = 0x01,				///< Automatically advancing static translation.
		MODE_STATIC_MANUAL = 0x02,				///< Manually advancing static translation.
		MODE_INCREMENTAL = 0x04,				///< Incremental translation.
		MODE_REACTIVE = 0x08					///< Reactive controller mode.
	};

	/// The various languages that we support
	enum Language
	{ 
		LANG_CPLUS,								///< The action language C+.
		LANG_BC,								///< The action language BC.
		LANG_BCPLUS,								///< Prototype extension of BC.
		LANG_MVPF								///< Multi-valued propositional logic formulas.
	};

	/// The various extra configurations that can be set.
	enum ConfigOption
	{
		_OPT_BEGIN_ = 0x00,				///< Dummy constant to mark the beginning of the configuration list.

		// boolean
		OPT_DISCARD_F2LP = _OPT_BEGIN_,	///< Whether we should discard all F2LP intermediate files when we're done.
		OPT_SUPPRESS_INTERACTION,		///< Whether we should operate in silent mode.
		OPT_SHIFT,						///< Whether we should assert the '--shift' flag, which tells gringo to shift disjunction into the body of the rules.
		OPT_STATS,						///< Whether we should show solving stats.
		OPT_NO_DOMAIN_ENFORCE,			///< Whether we should suppress domain enforcing rules from being generated
		OPT_NO_ADD_DOMAIN_ENFORCE,		///< Whether we should suppress domain enforcing rules from being generated for additive constants

		// integer
		OPT_MINSTEP,					///< The currently configured minimum step, or UNDEFINED.
		OPT_MAXSTEP,					///< The currently configured maximum step, or UNDEFINED.
		OPT_MAXADDITIVE,				///< The currently configured maximum additive, or UNDEFINED.
		OPT_NUM_SOLN,					///< The currently configured # of solutions to find.
		OPT_EXT_PORT,					///< The port that we should make available for the user.
		OPT_INT_PORT,					///< The port which we should use internally to communicate with oClingo.
		_OPT_LENGTH_,					///< Dummy constant to provide the number configuration options.
		_OPT_END_ = _OPT_LENGTH_		///< Dummy constant to mark the end of the configuration list.
	};

	/// A list of all string options
	enum StrOption
	{
		// Toolchain commands
		_STR_BEGIN_ = 0,										///< psuedo option marking the beginning of the string options.
		_STR_TC_BEGIN_ = 0,										///< psuedo option marking the toolchain beginning.
		STR_TRANSLATOR = TC_TRANSLATOR,							///< translator
		STR_PREPROC = TC_PREPROC,								///< pre-processor
		STR_C4_PREPROC = TC_C4_PREPROC,							///< CLINGO3TO4 pre-processor	
		// STR_GROUNDER = TC_GROUNDER,								///< grounder
		STR_SOLVER = TC_SOLVER,									///< solver
		STR_POSTPROC = TC_POSTPROC,								///< post-processor. Ignored in reactive mode.
		STR_REACTIVE_BRIDGE = TC_REACTIVE_BRIDGE,				///< The process to launch which interacts with a reactive solver.


		// Toolchain options
		_STR_TC_OPT_BEGIN_ = STR_REACTIVE_BRIDGE + 1,			///< psuedo string option marking the toolchain options beginning
		STR_OPT_TRANSLATOR = _STR_TC_OPT_BEGIN_ + TC_TRANSLATOR,
		STR_OPT_PREPROC = _STR_TC_OPT_BEGIN_ + TC_PREPROC,
		STR_OPT_C4_PREPROC = _STR_TC_OPT_BEGIN_ + TC_C4_PREPROC,
		// STR_OPT_GROUNDER = _STR_TC_OPT_BEGIN_ + TC_GROUNDER,
		STR_OPT_SOLVER = _STR_TC_OPT_BEGIN_ + TC_SOLVER,
		STR_OPT_POSTPROC = _STR_TC_OPT_BEGIN_ + TC_POSTPROC,
		STR_OPT_REACTIVE_BRIDGE = _STR_TC_OPT_BEGIN_ + TC_REACTIVE_BRIDGE,

		
	
		// Toolchain output files
		_STR_TC_OUT_BEGIN_ = STR_OPT_REACTIVE_BRIDGE+1,				///< psuedo string option marking the toolchain output files beginning
		STR_OUT_TRANSLATOR = _STR_TC_OUT_BEGIN_ + TC_TRANSLATOR,
		STR_OUT_PREPROC = _STR_TC_OUT_BEGIN_ + TC_PREPROC,
		STR_OUT_C4_PREPROC = _STR_TC_OUT_BEGIN_ + TC_C4_PREPROC,
		// STR_OUT_GROUNDER = _STR_TC_OUT_BEGIN_ + TC_GROUNDER,
		STR_OUT_SOLVER = _STR_TC_OUT_BEGIN_ + TC_SOLVER,
		STR_OUT_POSTPROC = _STR_TC_OUT_BEGIN_ + TC_POSTPROC,
		STR_OUT_REACTIVE_BRIDGE = _STR_TC_OUT_BEGIN_ + TC_REACTIVE_BRIDGE,

		// other options
		STR_QUERY,													///< The query to run
		STR_SYMTAB_FILE,											///< The symbol table output file.
		STR_NONE_ALIAS,												///< The value to replace none with.

		_STR_LENGTH_											///< psuedo string option marking the end of the options list.

	};

	/// A flag-type used to specify multiple modes simultaneously
	typedef unsigned char Modes;

	/// A basic structure used to identify the tool and running mode associated with a file.
	struct FileType
	{
		bool known;				///< Whether the file type is known.
		Toolchain tool;			///< The tool the file is associated with.
		Modes runningMode;		///< The running mode the file is associated with.

		inline FileType()
			: known(false), tool(_TC_BEGIN_), runningMode(MODE_UNKNOWN)
			{ /* Intentionally Left Blank */ }

		inline FileType(bool _known, Toolchain _tool, Modes _runningMode)
			: known(_known), tool(_tool), runningMode(_runningMode)
			{ /* Intentionally Left Blank */ }

		inline virtual ~FileType() { /* Intentionally Left Blank */ }
	};

	/// A structure which is used to describe a query.
	struct Query
	{
		std::string name;		///< The human-readable name of the query.
		std::string cmd;		///< The short name or command used for query selection.
		unsigned int minstep;	/// Minimum step for the query (if specified, UNDEFINED otherwise).
		unsigned int maxstep;	/// Maximum step for the query (if specified, UNDEFINED otherwise).

		inline Query()
			: name(""), cmd(""), minstep(UNDEFINED), maxstep(UNDEFINED)
			{ /* Intentionally Lef Blank */ }

		inline Query(std::string const& _name, std::string const& _cmd, unsigned int _minstep, unsigned int _maxstep)
			: name(_name), cmd(_cmd), minstep(_minstep), maxstep(_maxstep)
			{ /* Intentionally Left Blank */ }

		inline virtual ~Query() { /* Intentionally Left Blank */ }

	};

	/// A structure used to describe the current runtime subconfiguration (used for iterative runs where the configuration can change).
	struct RunConfig
	{
		std::string query;						///< The query that should be run.
		unsigned int minstep;					///< The minimum step that should be solved for.
		unsigned int maxstep;					///< The maximum step that should be solved for.
		unsigned int numSoln;					///< The number of solutions to display.
		mutable bool is_pyscript_added = false;					///< Boolean value to check if the lua include has been added or not during the first run

	};

	typedef std::map<std::string, Query> QueryMap;						///< A map of queries indexed by their commands.
	typedef std::map<std::string,std::string> ConstantMap;				///< A map between constant names and their definitions.
	typedef std::vector<std::string> InputList;							///< A list of input files.

private:

	/***************************************************************/
	/* Constants */
	/***************************************************************/
public:
	/// Indicates an undefined configuration setting.
	static unsigned int const UNDEFINED;

	/// Indicates an infinite value.
	static unsigned int const INF;
	
	/***************************************************************/
	/* Members */
	/***************************************************************/
private:

	std::string mErrFile;							///< A temporary error file to use for each tool being run.

	Mode mRunningMode;								///< The currently configured running mode.
	unsigned char mCustomRunningMode;				///< Whether the running mode has been manually set.

	Language mLang;								///< The currently configured input language.
	unsigned char mCustomLang;					///< Whether the input language has been manually set.

	// Misc Configurations
	unsigned int	mConfigOpts   [_OPT_LENGTH_];		///< The available configuration options.
	unsigned char	mCustomConfig [_OPT_LENGTH_];		///< Whether each configuration option has been set previously.

	QueryMap mQueries;								///< The set of all known queries.

	// Toolchain configuration
	std::string 	mStrOpts	[_STR_LENGTH_];		///< The set of string options.
	unsigned char 	mCustomStr 	[_STR_LENGTH_];		///< Whether each string option has been modified.
	bool 			mRun	 	[_TC_LENGTH_];		///< Whether we should run each part of the toolchain.
	unsigned char	mCustomRun	[_TC_LENGTH_];		///< Whether each toolchain component has been custom set to run or not.
	InputList		mInputs 	[_TC_LENGTH_];		///< The files which are designated dynamically as input for each of the toolchain components.
	ConstantMap		mConstDefs;						///< The constants which have been defined and should be passed to the grounder.

public:
	/***************************************************************/
	/* Constructors / Destructors */
	/***************************************************************/
	/**
	 * Default Constructor
	 * @param name The name of this executable.
	 * Initializes the configuration to defaults.
	 */
	Config(std::string const& name = "");

	/**
	 * Destructor
	 * Does nothing.
	 */
	inline virtual ~Config() { /* Intentionally Left Blank */ }

	/***************************************************************/
	/* Psuedo Accessors */
	/***************************************************************/

	/***************************************************************/
	/* Basic accessors / mutators */
	/***************************************************************/

	/// Basic Accessor. Does what's on the box.

	// mode
	///@return The currently configured running mode
	inline Mode mode() const										{ return mRunningMode; }
	///@return Whether the running mode has been manually modified.
	inline bool customMode() const									{ return (bool)mCustomRunningMode; }

	///@return The currently configured input language
	inline Language lang() const									{ return mLang; }
	///@return Whether the input language has been manually modified.
	inline bool customLang() const									{ return (bool)mLang; }

	// config options
	///@return The boolean value of the specified option.
	inline bool boolConfigOpt(ConfigOption opt) const				{ return (bool)mConfigOpts[opt]; }
	///@return The integral value of the specified option.
	inline unsigned int intConfigOpt(ConfigOption opt) const		{ return mConfigOpts[opt]; }
	///@return Whether the specified option has been manually modified.
	inline bool customConfigOpt(ConfigOption opt) const				{ return (bool)mCustomConfig[opt]; }
	///@return The string value of the specified option.
	inline std::string const& strOpt(StrOption opt) const			{ return mStrOpts[opt]; }
	///@return Whether the specified option has been manually modified.
	inline bool customStrOpt(StrOption opt) const					{ return (bool)mCustomStr[opt]; }

	// command options
	///@return The base command for the specifed tool.
	inline std::string const& command(Toolchain tool) const			{ return strOpt((StrOption)(tool + _STR_TC_BEGIN_)); }
	///@return Whether the base command for the tool has been manually modififed.
	inline bool customCmd(Toolchain tool) const						{ return customStrOpt((StrOption)(tool + _STR_TC_BEGIN_)); }
	///@return Whether the tool is configured to run.
	inline bool run(Toolchain tool) const							{ return mRun[tool]; }
	///@return Whether the run status of the tool has been manually modified.
	inline bool customRun(Toolchain tool) const						{ return (bool)mCustomRun[tool]; }
	///@return The user-defined command options for the specified tool.
	inline std::string const& opts(Toolchain tool) const			{ return strOpt((StrOption)(tool + _STR_TC_OPT_BEGIN_)); }
	///@return Whether the command options have been manually modififed.
	inline bool customOpts(Toolchain tool) const					{ return customStrOpt((StrOption)(tool + _STR_TC_OPT_BEGIN_)); }


	// constants
	inline bool isDefined(std::string const& constant) const;
	std::string const* def(std::string const& constant) const;

	// queries
	inline size_t queryCount() const								{ return mQueries.size(); }
	inline bool hasQuery(std::string const& queryCmd) const			{ return (bool)mQueries.count(queryCmd); }
	inline Query const* activeQuery() const							{ return query(strOpt(STR_QUERY));  }
	Query const* query(std::string const& queryCmd) const;

	// etc
	///@return The file we should use for stderr from our children.
	inline std::string const& errFile() const						{ return mErrFile; }
	///@return A description of the output file for the provided tool.
	inline std::string const& output(Toolchain tool) const			{ return strOpt((StrOption)(tool + _STR_TC_OUT_BEGIN_)); }
	///@return Whether the output file for the specified tool has been manually modified.
	inline bool customOut(Toolchain tool) const						{ return customStrOpt((StrOption)(tool + _STR_TC_OUT_BEGIN_)); }

	/// Basic Mutators.

	/// @return If the running mode has been previously set.
	bool mode(Mode newMode);

	/// @return If the input language has been previously set.
	bool lang(Language newLang);

	/// @return If the option was previously set.
	bool boolConfigOpt(ConfigOption opt, bool val);

	/// @return If the option was previously set.
	bool intConfigOpt(ConfigOption opt, unsigned int val);

	/// @return If the option was previously set.
	bool strOpt(StrOption opt, std::string const& val);

	/// @return If the constant was previously defined.
	bool def(std::string const& constant, std::string const& value);

	/// Sets the base command for a tool.
	/// An empty value will disable the tool.
	/// A non-empty value will enable the tool.
	/// @return If the command has previously been changed.
	inline bool command(Toolchain tool, std::string const& val) {
		return strOpt((StrOption)(tool + _STR_TC_BEGIN_), val) |/*non-shortcutting*/ run(tool, val != "");
	}

	/// @return If the run status has been previously changed.
	bool run(Toolchain tool, bool val); 

	/// @return Whether the option has been previously set.
	bool opts(Toolchain tool, std::string const& val) {
		return strOpt((StrOption)(tool + _STR_TC_OPT_BEGIN_), val);
	}


	/// Sets the target output file for the toolchain component.
	/// @param file The new output file.
	/// @return True if this overrode a previous configuration.
	bool output(Toolchain tool, std::string const& file) {
		return strOpt((StrOption)(tool + _STR_TC_OUT_BEGIN_), file);
	}

	/**
	 * Attempts to add a query to the list of known queries.
	 * @param name The human readable name of the query.
	 * @param cmd The command used to select this query.
	 * @param maxstep The maxstep of the query (or UNDEFINED).
	 * @param minstep The minstep of the query (or UNDEFINED).
	 * @return true if the add operation was a success. False if it failed (duplicate ID).
	 */
	bool addQuery(std::string const& name, std::string const& cmd, unsigned int maxstep = UNDEFINED, unsigned int minstep = UNDEFINED);

	/**
	 * Attempts to add a query to the list of known queries.
	 * @param query The query to add.
	 * @return true if the add operation was a success. False if it failed (duplicate ID).
	 */
	bool addQuery(Query const& query);

	/***************************************************************/
	/* Advanced functionality / Helpers */
	/***************************************************************/


	/// Parses the language represented in the string.
	/// @param str The string containing the language specification.
	/// @param[out] outLang The language stored in the string.
	/// @return True if successful, false otherwise.
	bool parseLang(char const* str, Language& outLang);

	/**
	 * Adds an input file to the list, determining which toolchain component it appears to belong to based on its extension.
	 * @param input The name of the input file.
	 * @return Flags indicating the input's associated tool and running mode.
	 */
	FileType addInput(std::string const& input);

	/**
	 * Adds an input file the given tool's list.
	 * @param tool The tool to add the input file to.
	 * @param input The name of the input file.
	 */
	inline void addInput(Toolchain tool, std::string const& input)
															{ mInputs[tool].push_back(input); }


	/**
	 * Tallies the number of user inputs that will be added to the provided too.
	 * This tally includes intermediate files from the previous step that will be
	 * ran, as well as input files from all steps between the specified tool and
	 * the previous step that was ran.
	 * @param tool The tool to get an input tally of.
	 * @return The number of user input files that will be passed to that tool.
	 */
	size_t cumulativeInputCount(Toolchain tool) const;


	/**
	 * Gets an iterator pointing to the beginning of the list of input files for a tool.
	 * Note: Be aware that these are user-specified files and do not include the files belonging to previous unran tools or their intermediate output.
	 * @param tool The tool to get the iterator for.
	 * @return An iterator pointing the beginning of the input list.
	 */
	inline InputList::const_iterator
	beginInput(Toolchain tool) const						{ return mInputs[tool].begin(); }

	/**
	 * Gets an iterator pointing to the end of the list of input files for a tool.
	 * @param tool The tool to get the iterator for.
	 * @return An iterator pointing the end of the input list.
	 */
	inline InputList::const_iterator
	endInput(Toolchain tool) const							{ return mInputs[tool].end(); }

	/**
	 * Gets an iterator pointing to the beginning of the list of constant definitions.
	 * @return An iterator pointing the beginning of the constant definitions.
	 */
	inline ConstantMap::const_iterator
	beginConstants() const									{ return mConstDefs.begin(); }

	/**
	 * Gets an iterator pointing to the end of the list of constant definitions.
	 * @return An iterator pointing the end of the constant definitions.
	 */
	inline ConstantMap::const_iterator
	endConstants() const									{ return mConstDefs.end(); }

	/**
	 * Gets an iterator pointing to the beginning of the list of query definitions.
	 * @return An iterator pointing the beginning of the query definitions.
	 */
	inline QueryMap::const_iterator
	beginQueries() const									{ return mQueries.begin(); }


	/**
	 * Gets an iterator pointing to the beginning of the list of query definitions.
	 * @return An iterator pointing the beginning of the query definitions.
	 */
	inline QueryMap::const_iterator
	endQueries() const										{ return mQueries.end(); }

	/**
	 * Gets an iterator pointing to the end of the list of input files for a tool.
	 * @param tool The tool to get the iterator for.
	 * @return An iterator pointing the end of the input list.
	 */
	inline InputList::iterator
	beginInput(Toolchain tool)								{ return mInputs[tool].begin(); }

	/**
	 * Gets an iterator pointing to the beginning of the list of input files for a tool.
	 * @param tool The tool to get the iterator for.
	 * @return An iterator pointing the beginning of the input list.
	 */
	inline InputList::iterator
	endInput(Toolchain tool)								{ return mInputs[tool].begin(); }

	/**
	 * Gets an iterator pointing to the beginning of the list of constant definitions.
	 * @return An iterator pointing the beginning of the constant definitions.
	 */
	inline ConstantMap::iterator
	beginConstants()										{ return mConstDefs.begin(); }

	/**
	 * Gets an iterator pointing to the end of the list of constant definitions.
	 * @return An iterator pointing the end of the constant definitions.
	 */
	inline ConstantMap::iterator
	endConstants()											{ return mConstDefs.end(); }

	/**
	 * Gets an iterator pointing to the beginning of the list of query definitions.
	 * @return An iterator pointing the beginning of the query definitions.
	 */
	inline QueryMap::iterator
	beginQueries()										{ return mQueries.begin(); }


	/**
	 * Gets an iterator pointing to the beginning of the list of query definitions.
	 * @return An iterator pointing the beginning of the query definitions.
	 */
	inline QueryMap::iterator
	endQueries()										{ return mQueries.end(); }

	/**
	 * Compiles the command line call for a specific element in the toolchain.
	 * @param tool The toolchain element we want to compile the call for.
	 * @param subconfig The subconfiguration used to override settings for this specific run.
	 * @return A command to send to the system to call the element.
	 */
	std::string compileCommandLine(Toolchain tool, RunConfig const* subconfig = NULL) const;
	
	/**
	 * Compiles the set of arguments used to call a specific element in the toolchain.
	 * @param tool The toolchain element we want to compile the call for.
	 * @param subconfig The subconfiguration used to override settings for this specific run.
	 * @param[out] arglist The compiled list of arguments. The list is not cleared.
	 */
	void compileArgs(Toolchain tool,  std::list<std::string>& arglist, RunConfig const* subconfig = NULL) const;

	/**
	 * Sets the toolchain to run everything up to the desired tool and nothing else.
	 * @param tool The toolchain tool to run to.
	 * @return True if this command has overidden one or more previous configuration options.
	 */
	bool setRunTo(Toolchain tool);

	/**
	 * Sets the toolchain to run everything from the desired tool (including the tool) and nothing else.
	 * @param tool The toolchain tool to run from.
	 * @return True if this command has overriden one or more previous configuration options.
	 */
	bool setRunFrom(Toolchain tool);

	/**
	 * Sets the toolchain to run only the specified tool and nothing else.
	 * @param tool The toolchain tool to run.
	 * @return True if this command has overriden one or more previous configuration options.
	 */
	bool setOnlyRun(Toolchain tool);

	/**
	 * Parses a maxstep specification in the form a single unsigned integer or an integer range %d..%d.
	 * @param maxstep The maxstep specification.
	 * @return A pair (minstep,maxstep) corresponding to the specification.
	 * If the specification is malformed both will be UNDEFINED.
	 * If it's a single integer, minstep will be UNDEFINED.
	 */
	static std::pair<unsigned int, unsigned int> parseMaxstep(std::string const& maxstep);

	/// Whether the output from the provided toolchain component should be captured and filtered.
	// TODO
	bool captureOutput(Toolchain tool);

	
	const char * getTextForEnum(int enumVal);
	


protected:

	/**
	 * Compiles a command which aggregates all the appropriate input files for a toolchain tool into stdout.
	 * @param tool The tool to compile the inputs for.
	 * @param[out] arglist The list of args to append the input args to.
	 */
	void compileInputArgs(Toolchain tool, std::list<std::string>& arglist) const;


	/**
	 * Attempts to identify the provided extension, providing information about which tool and running
	 * mode it's associated with.
	 * @param ext The extension to attempt to identify.
	 * @param[out] out Flags indicating the associated tool and running mode.
	 * @return out.
	 */
	FileType& checkExtension(char const* ext, FileType& out) const;



};



#endif /* CONFIG_H_ */
