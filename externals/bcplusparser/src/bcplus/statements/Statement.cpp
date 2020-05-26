
#include "bcplus/Location.h"
#include "bcplus/statements/Statement.h"

namespace bcplus {
namespace statements {

char const* Statement::Type::typeString(type v) {
	switch (v) {
	case INCLUDE:					return "include";
	case MACROS:					return "macros";
	case CONSTANTS:					return "constants";
	case OBJECTS:					return "objects";
	case SORTS:						return "sorts";
	case VARIABLES:					return "variables";
	case COMMENTS:					return "comments";


	case F2LP:						return "F2LP block";
	case LUA:						return "LUA block";
	case PYTHON:					return "PYTHON block";
	case ASP:						return "ASP block";
	case SHOW:						return "show";
	case SHOW_ALL:					return "show (all)";
	case HIDE:						return "hide";
	case HIDE_ALL:					return "hide (all)";
	case NOCONCURRENCY:				return "noconcurrency";
	case STRONG_NOCONCURRENCY:		return "strong noconcurrency";
	case MAXAFVALUE:				return "maxAFValue";
	case MAXADDITIVE:				return "maxAdditive";
	case QUERY:						return "query";
	case LAW_BASIC:					return "basic";
	case LAW_CAUSED:				return "caused";
	case LAW_PCAUSED:				return "possibly caused";
	case LAW_IMPL:					return "implication";
	case LAW_CAUSES:				return "causes";
	case LAW_INCREMENTS:			return "increments";
	case LAW_MCAUSE:				return "may cause";
	case LAW_ALWAYS:				return "always";
	case LAW_CONSTRAINT:			return "constraint";
	case LAW_IMPOSSIBLE:			return "impossible";
	case LAW_NEVER:					return "never";
	case LAW_DEFAULT:				return "default";
	case LAW_EXOGENOUS:				return "exogenous";
	case LAW_INERTIAL:				return "inertial";
	case LAW_NONEXECUTABLE:			return "nonexecutable";
	case LAW_RIGID:					return "rigid";
	case LAW_OBSERVED:				return "observed";

	default:						return "<BAD_STMT_TYPE>";
	}

}



Statement::Statement(Type::type type, Location const& begin, Location const& end) 
	: _begin(begin), _end(end), _type(type) {
	/* Intentionally left blank */
}

Statement::~Statement() {
	/* Intentionally left blank */
}


}}


