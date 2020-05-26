#pragma once

#include "babb/utils/memory.h"

namespace bcplus {
namespace languages {

class Language : public babb::utils::Referenced {
public:
	/******************************************************************************/
	/* Public Types */
	/******************************************************************************/
	/// Container for language features to be supported
	struct Feature {
		enum type {
			DECL_INCLUDE,				///< include statements.
			DECL_MACRO,					///< macro definitions
			DECL_SORT,					///< sort definition
			DECL_OBJECT,				///< object definition
			DECL_CONSTANT,				///< constant definition
			DECL_VARIABLE,				///< variable definition
			DECL_SHOW,					///< show statement
			DECL_SHOW_ALL,				///< show all statement
			DECL_HIDE,					///< hide statement
			DECL_HIDE_ALL,				///< hide all statement
			DECL_MAXAFVALUE,			///< Maximum additive fluent value (:- maxAFtype)
			DECL_MAXADDITIVE,			///< Maximum additive fluent value (:- maxAdditive)
			DECL_QUERY,					///< Query statements	

			CONST_ABACTION,				///< abAction constant type
			CONST_ACTION,
			CONST_ADDITIVEFLUENT,		///< additiveFluent constant type
			CONST_ADDITIVEACTION,		///< additiveAction constant type
			CONST_ATTRIBUTE,			///< attribute constant type
			CONST_EXTERNALACTION,		///< externalAction constant type
			CONST_EXTERNALFLUENT,		///< externalFluent constant type
			CONST_EXOGENOUSACTION,		///< exogenousAction constant type
			CONST_INERTIALFLUENT,		///< inertialFluent constant type
			CONST_RIGID,				///< rigid constant type
			CONST_SIMPLEFLUENT,			///< simpleFluent constant type
			CONST_SDFLUENT,				///< sdFluent constant type


			FORMULA_NESTED,				///< nested formulas
			FORMULA_CONSTANT_ARGS,		///< Constants ocurring within arguments of an atom.
			FORMULA_CARDINALITY_BODY,	///< Cardinality rules in the body of laws
			FORMULA_CARDINALITY_HEAD,	///< Cardinality (choice) rules in the head of laws
			FORMULA_QUANTIFIER,			///< Quantifier formulas in the body of laws
			FORMULA_NOT_KEYWORD,		///< Whether to allow the 'not' keyword in a formula
			FORMULA_NOT_DASH,			///< Whether to allow the '-' symbol in a formula to indicate negation
			FORMULA_NOT_DASH_HEAD,		///< Whether to allow the '-' symbol in the head of a law to indicate strong negation
			FORMULA_TILDE_STRONG_NEG,	///< Whether to allow the '~' symbol indicating strong negation

			MACRO,						///< Macro expansion
			VARIABLE,					///< Variables
			NUMRANGE_SORT,				///< Whether number ranges are allowed as dynamic sort declarations
			STAR_SORT,					///< Whether dynamically declared star sorts "sortname*" are allowed
			CARROT_SORT,				///< Whether dynamically declared carrot sorts "sortname^" are allowed		
			SORT_PLUS,					///< Whether dynamically declared additive sorts "sortname+object" are allowed		
			MAXSTEP,					///< Allow use of the maxstep keyword
			MAXADDITIVE,				///< Allow use of the maxAdditive keyword
			MAXAFVALUE,					///< Allow use of the maxAFValue keyword
			
			QUERY_BIND_STEP,			///< Whether to accept step bindings (i:F) in query formulas.
			QUERY_MAXSTEP,				///< Whether to accept a maxstep declaration within a query.
			QUERY_LABEL,				///< Whether to accept a label declaration within a query

			CLAUSE_IF,					///< "if G" optional rule body
			CLAUSE_AFTER,				///< "after H" optional rule body
			CLAUSE_IFCONS,				///< "ifcons M" optional rule body
			CLAUSE_UNLESS,				///< "unless ab" optional rule body
			CLAUSE_WHERE,				///< "where M" optional rule body

			LAW_BASIC_S,				///< F if G
			LAW_BASIC_D,				///< F if G after H
			LAW_BASIC_FACT,				///< A single fact "c=v"


			LAW_CAUSED_S,				///< caused F if G.
			LAW_CAUSED_D,				///< caused F if G after H
			LAW_PCAUSED_S,				///< possibly caused F if G
			LAW_PCAUSED_D,				///< possibly caused F if G
			
			LAW_IMPL,					///< F <- G (also: <- G).

			LAW_CAUSES,					///< G causes F if H.
			LAW_INCREMENTS,				///< G increments c by v if H.
			LAW_DECREMENTS,				///< G decrements c by v if H.
			LAW_MCAUSE,					///< G may cause F if H.

			LAW_ALWAYS_S,				///< always G
			LAW_ALWAYS_D,				///< always G after H
			LAW_CONSTRAINT_S,			///< co	nstraint G
			LAW_CONSTRAINT_D,			///< constraint G after H
			LAW_IMPOSSIBLE_S,			///< impossible G
			LAW_IMPOSSIBLE_D,			///< impossible G after H
			LAW_NEVER_S,				///< never G
			LAW_NEVER_D,				///< never G after H
	
			LAW_DEFAULT_S,				///< default F if G
			LAW_DEFAULT_D,				///< default F if G after H
			LAW_EXOGENOUS_S,			///< exogenous c if G
			LAW_EXOGENOUS_D,			///< exogenous c if G after H
			LAW_INERTIAL_S,				///< inertial c if G
			LAW_INERTIAL_D,				///< inertial c if G after H

			LAW_NONEXECUTABLE,			///< nonexecutable F if G		
			LAW_RIGID,					///< rigid c			
			LAW_OBSERVED,				///< observed c=v at v2.
			LAW_TEMPORAL_CONSTRAINT,	///< constraint F at v2.

			NOCONCURRENCY,				///< noconcurrency.
			STRONG_NOCONCURRENCY,		///< strong noconcurrency.

			CODE_ASP_CP,				///< CCalc style ASP code block
			CODE_ASP_GR,				///< Gringo style ASP code block
			CODE_F2LP_CP,				///< CCalc style F2LP code block
			CODE_F2LP_GR,				///< Gringo style F2LP code block
			CODE_LUA_CP,				///< CCalc style LUA code block
			CODE_LUA_GR,					///< Gringo style LUA code block

			CODE_PYTHON_CP,				///< CCalc style PYTHON code block
			CODE_PYTHON_GR				///< Gringo style PYTHON code block
		};


		/// Get a description of the provided feature
		/// @param feature The feature to retrieve the description of
		/// @return A short description of the feature
		static char const* descr(type feature);

	};


	/// Get the description of the provided feature
	char const* featureDescription(Feature::type feature) const				{ return Feature::descr(feature); }

	/// Get the name of this language.
	/// @return The language name
	virtual char const* name() const = 0;

	/// Determine if this language supports the provided feature.
	/// @param feature The feature to check
	/// @param True if the language supports the feature, false otherwise.
	virtual bool support(Feature::type feature) const = 0;


	/// Get a unique integral identifier for this language.
	virtual size_t id() const = 0;

};

}}
