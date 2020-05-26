#pragma once

#include "babb/utils/memory.h"
#include "memwrappers.h"

namespace as2transition {


class Config : public babb::utils::Referenced {
public:

	/**********************************************/
	/* Types */
	/**********************************************/
	struct PredType {
		enum type {
			POSITIVE			= 0x0001,			///< Predicates which aren't assigned "none" or "false"
			NEGATIVE			= 0x0002,			///< Predicates which are assigned "none" or "false"
			STRONG_NEGATION		= 0x0004,			///< Predicates beginning with a tilde
			CONTRIBUTION		= 0x0008,			///< Additive constant contribution predicates
			XPREDICATE			= 0x0010,			///< Predicates beginning with x_, which marks them as internal
			UNKNOWN				= 0x0020			///< Predicates that don't conform to any known formats.

		};
	};



	struct PredFormat {
		enum type {
			RAW,				///< Show predicates exactly as they were read in
			INNER,				///< Displays each formatted predicate as eql(c,v)
			EQL,				///< Displays each formatted predicate as c=v
			SHORT				///< Similar to EQL except displays c=true as c and c=false as ~c
		};
	};


	struct Solver {
		enum type {
			UNKNOWN,
			SMODELS,
			CMODELS,
			OCLINGO
		};
	};

private:

	/**********************************************/
	/* Private members */
	/**********************************************/

	/// A mask of predicate types determining which to show
	int _showmask;


	/// How we should format our predicates
	PredFormat::type _format;

	/// Whether we should strip "saniObj_" and "saniConst_" from base element names
	bool _stripSanitization;

	/// Whether we should strip "o_", "c_x_x_", and "e_x_x_" from base element names
	bool _stripPrefix;

	/// A string to replace with "none", if anything.
	babb::utils::ref_ptr<const ReferencedString> _noneAlias;

	/// Whether we should separate external constants from their internal counterparts
	bool _separateExternal;

	/// Whether we should separate actions from fluents;
	bool _separateActions;

	/// Whether we should print one predicate per line
	bool _onePredPerLine;


	/// The solver that we're parsing (if known)
	Solver::type _solver;

public:
	

	/**********************************************/
	/* Public members */
	/**********************************************/

	/// Basic Constructor
	/// @param showmask A mask of predicate types to display
	/// @param format The format to put each predicate in
	/// @param noneAlias A string to replace to replace with "none" or NULL
	/// @param stripSanitization Whether we should strip "saniObj_" and "saniConst_" from base element names
	/// @param stripPrefix Whether we should strip "o_" and "c_x_x_" from base element names.
	/// @param separateExternal Whether we should separate external constants from their internal counterpart.
	/// @param separateActions Whether we should separate actions from fluents
	/// @param onePredPerLine Whether we should print each predicate on its own line
	Config(int showmask, PredFormat::type format, ReferencedString const* noneAlias = NULL, bool stripSanitization = true, bool stripPrefix = false, bool separateExternal = true, bool separateActions = true, bool onePredPerLine = false);

	/// Copy constructor...
	Config(Config const& other);


	/// Basic Destructor
	~Config();

	/// Get/set the showmask
	inline int showmask() const								{ return _showmask; }
	inline void showmask(int m)								{ _showmask = m; }

	/// Determine whether or not to show a predicate based on the current showmask
	inline bool show(int predmask) const					{ return !(predmask & ~showmask()); }

	/// Get/set the format
	inline PredFormat::type format() const					{ return _format; }
	inline void format(PredFormat::type f)					{ _format = f; }

	/// Get/set the none Alias
	inline ReferencedString const* noneAlias() const		{ return _noneAlias; }
	inline void noneAlias(ReferencedString const* na)		{ _noneAlias = na; }

	/// Get/set whether we should strip "saniObj_" and "saniConst_" from the base element names
	inline bool stripSanitization() const					{ return _stripSanitization; }
	inline void stripSanitization(bool s) 					{ _stripSanitization = s; }
	
	/// Get/set whether we should strip "o_" and "c_x_x_" from the base element names
	inline bool stripPrefix() const							{ return _stripPrefix; }
	inline void stripPrefix(bool s) 						{ _stripPrefix = s; }

	/// Get/set whether we should separate external constants...
	inline bool separateExternal() const					{ return _separateExternal; }
	inline void separateExternal(bool s) 					{ _separateExternal = s; }

	/// Get/set whether we should separate actions
	inline bool separateActions() const					{ return _separateActions; }
	inline void separateActions(bool s) 				{ _separateActions = s; }

	/// Get/set whether we should print each predicate on its own line
	inline bool onePredPerLine() const					{ return _onePredPerLine; }
	inline void onePredPerLine(bool s) 				{ _onePredPerLine = s; }

	/// Get/set the current solver we're working with
	inline Solver::type solver() const					{ return _solver; }
	inline void solver(Solver::type t) 					{ _solver = t; }

};





}
