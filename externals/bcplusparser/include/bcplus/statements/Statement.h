#pragma once

#include <string>
#include <ostream>
#include <sstream>

#include "babb/utils/memory.h"
#include "bcplus/Location.h"

namespace bcplus {
namespace statements {



/// Abstract base class for a statement.
class Statement : public babb::utils::Referenced {
public:
	/****************************************************************************/
	/* Public Types */
	/****************************************************************************/
	/// Container for the list of statement types
	struct Type {
		enum type {
			INCLUDE,					///< An include statement
			MACROS,						///< macro definition
			CONSTANTS,					///< constant definition
			OBJECTS,					///< object definition
			SORTS,						///< sort definition
			VARIABLES,					///< variable definition
			COMMENTS,					///< comment block
			F2LP,						///< F2LP code block
			LUA,						///< LUA code block
			PYTHON,						///< PYTHON code block
			ASP,						///< ASP code block
			SHOW,						///< Show statement
			SHOW_ALL,					///< Show all statement
			HIDE,						///< Hide statement
			HIDE_ALL,					///< Hide all statement
			NOCONCURRENCY,				///< noconcurrency statement
			STRONG_NOCONCURRENCY,		///< strong noconcurrency statement
			MAXAFVALUE,					///< maxAFValue statement
			MAXADDITIVE,				///< maxAdditive statement
			QUERY,						///< A query

			LAW_BASIC,					///< F if G after H...
			LAW_CAUSED,					///< caused F if G after H...
			LAW_PCAUSED,				///< possibly caused F if G after H...
			LAW_IMPL,					///< F <- G...
			LAW_CAUSES,					///< G causes F if H...
			LAW_INCREMENTS,				///< G increments F by V if H...
			LAW_DECREMENTS,				///< G decrements F by V if H...
			LAW_MCAUSE,					///< G may cause F if H...
			LAW_ALWAYS,					///< always G after H...
			LAW_CONSTRAINT,				///< constraint G after H...
			LAW_IMPOSSIBLE,				///< impossible G after H...
			LAW_NEVER,					///< never G after H...
			LAW_DEFAULT,				///< default F if G after H...
			LAW_EXOGENOUS,				///< exogenous c if G after H...
			LAW_INERTIAL,				///< inertial c if G after H...
			LAW_NONEXECUTABLE,			///< nonexecutable F after H...
			LAW_RIGID,					///< rigid c...
			LAW_OBSERVED,				///< observed c=v at V...
			LAW_TEMPORAL_CONSTRAINT		///< constraint F at V...
		};

		/// Get the string representation of the provided type
		static char const* typeString(type v);

	};

private:

	/****************************************************************************/
	/* Private Members */
	/****************************************************************************/
	/// beginning location of the statement
	Location _begin;

	/// ending location of the statement
	Location _end;

	/// Statement type
	Type::type _type;

public:

	/****************************************************************************/
	/* Constructors / Destructors */
	/****************************************************************************/

	/// Basic Contructor
	/// @param type The statement type
	/// @param begin The beginning location of the statmenet
	/// @param end The ending location of the statement
	Statement(Type::type type, Location const& begin = Location(NULL, 0, 0), Location const& end = Location(NULL, 0, 0));

	/// Destructor Stub
	~Statement();


	/****************************************************************************/
	/* Public Functions */
	/****************************************************************************/
	
	/// Get the type of this statement
	inline Type::type type() const									{ return _type; }

	/// Get the string representation of the statement type
	inline char const* typeString() const							{ return Type::typeString(type()); }

	/// Get the beginning location of this statement
	inline Location const& beginLoc() const							{ return _begin; }

	/// Get the ending location of this statement
	inline Location const& endLoc() const							{ return _end; }

	/// Perform a dep copy of this statement
	virtual Statement* copy() const = 0;

};
}}

