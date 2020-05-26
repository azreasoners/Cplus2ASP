#include "bcplus/languages/Language.h"

namespace bcplus {
namespace languages {

char const* Language::Feature::descr(type feature) {
	switch (feature) {
		case DECL_INCLUDE:               		return "include statements";
		case DECL_MACRO:                 		return "macro declarations";
		case DECL_SORT:                  		return "sorts";
		case DECL_OBJECT:                		return "objects";
		case DECL_CONSTANT:              		return "constants";
		case DECL_VARIABLE:              		return "variables";
		case DECL_SHOW:                  		return "show statements";
		case DECL_SHOW_ALL:                		return "show all statements";
		case DECL_HIDE:                  		return "hide statements";
		case DECL_HIDE_ALL:                		return "hide all statements";
		case DECL_MAXAFVALUE:            		return "maxAFtype declarations";
		case DECL_MAXADDITIVE:           		return "maxAdditive declarations";
		case DECL_QUERY:                  		return "queries";

		case MAXSTEP:							return "\"maxstep\" terms";
		case MAXADDITIVE:						return "\"maxAdditive\" terms";
		case MAXAFVALUE:						return "\"maxAFValue\" terms";


		case CONST_ABACTION:					return "\"abAction\" constants";
		case CONST_ACTION:						return "\"action\" constants";
		case CONST_ADDITIVEFLUENT:				return "\"additiveFluent\" constants";
		case CONST_ADDITIVEACTION:				return "\"additiveAction\" constants";
		case CONST_ATTRIBUTE:					return "\"attribute\" constants";
		case CONST_EXTERNALACTION:				return "\"externalAction\" constants";
		case CONST_EXTERNALFLUENT:				return "\"externalFluent\" constants";
		case CONST_EXOGENOUSACTION:				return "\"exogenousAction\" constants";
		case CONST_INERTIALFLUENT:				return "\"inertialFluent\" constants";
		case CONST_RIGID:						return "\"rigid\" constants";
		case CONST_SIMPLEFLUENT:				return "\"simpleFluent\" constants";
		case CONST_SDFLUENT:					return "\"sdFluent\" constants";


		case FORMULA_CONSTANT_ARGS:				return "constants occurring within atom parameters";
		case FORMULA_NESTED:             		return "arbitrary nested formulas";
		case FORMULA_CARDINALITY_BODY:			return "cardinality formulas in the body of a law";
		case FORMULA_CARDINALITY_HEAD:			return "cardinality formulas in the head of a law";
		case FORMULA_QUANTIFIER:				return "quantifiers";
		case FORMULA_NOT_KEYWORD:				return "\"not\" as default negation";
		case FORMULA_NOT_DASH:					return "\"-\" as default negation";
		case FORMULA_NOT_DASH_HEAD:				return "\"-\" as strong negation";
		case FORMULA_TILDE_STRONG_NEG:			return "\"~\" as strong negation";
		
		case MACRO:						 		return "macro expansion";
		case VARIABLE:							return "variables";
		case NUMRANGE_SORT:						return "dynamic numerical range sorts";
		case STAR_SORT:							return "dynamically declared starred sorts";
		case CARROT_SORT:						return "dynamically declared carrotted sorts";
		case SORT_PLUS:							return "dynamically declared additive sorts";
		case QUERY_BIND_STEP:					return "step-bound formulas within queries";

		case CLAUSE_IF:                  		return "\"if\" clauses";
		case CLAUSE_AFTER:               		return "\"after\" clauses";
		case CLAUSE_IFCONS:              		return "\"ifcons\" clauses";
		case CLAUSE_UNLESS:              		return "\"unless\" clauses";
		case CLAUSE_WHERE:               		return "\"where\" clauses";

		case LAW_BASIC_S:                		return "static basic laws (F if G)";
		case LAW_BASIC_D:                		return "dynamic basic laws (F if G after H)";

		case LAW_CAUSED_S:               		return "static causal laws (caused F if G)";
		case LAW_CAUSED_D:               		return "dynamic causal laws (caused F if G after H)";
		case LAW_PCAUSED_S:              		return "defeasible static causal laws (possibly caused F if G)";
		case LAW_PCAUSED_D:              		return "defeasible dynamic causal laws (possibly caused F if G after H)";

		case LAW_IMPL:                   		return "basic implication laws (F <- G)";

		case LAW_CAUSES:                 		return "\"causes\" shorthand laws (G causes F if H)";
		case LAW_INCREMENTS:             		return "additive \"increments\"laws (G increments c by V if H)";
		case LAW_MCAUSE:                 		return "defeasible \"may cause\" shorthand laws (G may cause F if H)";

		case LAW_ALWAYS_S:               		return "static \"always\" constraint laws (always G)";
		case LAW_ALWAYS_D:               		return "dynamic \"always\" constraint laws (always G after H)";
		case LAW_CONSTRAINT_S:           		return "static \"constraint\" laws (constraint G)";
		case LAW_CONSTRAINT_D:           		return "dynamic \"constraint\" laws (constraint G after H)";
		case LAW_IMPOSSIBLE_S:           		return "static \"impossible\" constraint laws (impossible G)";
		case LAW_IMPOSSIBLE_D:           		return "dynamic \"impossible\" constraint laws (impossible G after H)";
		case LAW_NEVER_S:                		return "static \"never\" constraint laws (never G)";
		case LAW_NEVER_D:                		return "dynamic \"never\" constraint laws (never G after H)";

		case LAW_DEFAULT_S:              		return "static default laws (default c=v if G)";
		case LAW_DEFAULT_D:              		return "dynamic default laws (default c=v if G after H)";
		case LAW_EXOGENOUS_S:            		return "static exogenous laws (exogenous c if G)";
		case LAW_EXOGENOUS_D:            		return "dynamic exogenous laws (exogenous c if G after H)";
		case LAW_INERTIAL_S:             		return "static inertial laws (inertial c if G)";
		case LAW_INERTIAL_D:             		return "dynamic inertial laws (inertial c if G after H)";

		case LAW_NONEXECUTABLE:          		return "\"nonexecutable\" constraint laws (nonexecutable F if G)";
		case LAW_RIGID:                  		return "\"rigid\" constraint laws (rigid c)";

		case LAW_OBSERVED:               		return "observational laws (observed c=v at t)";
		case LAW_TEMPORAL_CONSTRAINT:           return "temporal constraint laws (constraint F at t)";

		case NOCONCURRENCY:              		return "no concurrency declarations";
		case STRONG_NOCONCURRENCY:       		return "strong no concurrency declarations";

		case CODE_ASP_CP:                		return "ASP code blocks (:- begin asp. ... :- end asp.)";
		case CODE_ASP_GR:                		return "ASP code blocks (#begin_asp ... #end_asp.)";
		case CODE_F2LP_CP:               		return "F2LP code blocks (:- begin f2lp. ... :- end f2lp.)";
		case CODE_F2LP_GR:               		return "F2LP code blocks (#begin_f2lp ... #end_f2lp.)";
		case CODE_LUA_CP:                		return "LUA code blocks (:- begin lua. ... :- end lua.)";
		case CODE_LUA_GR:                		return "LUA code blocks (#begin_lua ... #end_lua.)";
		case CODE_PYTHON_CP:                	return "PYTHON code blocks (:- begin_python. ... :- end_python.)";
		case CODE_PYTHON_GR:                	return "PYTHON code blocks (#begin_python ... #end_PYTHON.)";
		default:						 		return "<UNKNOWN_FEATURE>";
	}

}








}}

