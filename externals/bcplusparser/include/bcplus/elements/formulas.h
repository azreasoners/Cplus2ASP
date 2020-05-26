#pragma once

#include "bcplus/elements/terms.h"
#include "bcplus/elements/detail/ElementClass.h"
#include "bcplus/elements/detail/BinaryElement.h"
#include "bcplus/elements/detail/UnaryElement.h"
#include "bcplus/elements/detail/NullaryElement.h"
#include "bcplus/elements/detail/Binding.h"
namespace bcplus {
namespace elements {

namespace detail {

/// Container of enumeration of possible types
struct FormulaType {
	enum type {
		BINARY,
		COMPARISON,
		UNARY,
		QUANTIFIER,
		ATOMIC,
		NULLARY,
		CARDINALITY,
		BINDING
	};
};

/// Structure descrbing binary formula operators.
struct BinaryFormulaOperator {
		enum type {
			AND,
			OR,
			EQUIV,
			IMPL,
			BIND_STEP, // i:F
			REV_IMPL // fake operator to simulate F <- G.
		};

	/// Functor to convert binary formula operators to cstrings.
	struct cstr {

		char const* operator() (type op) {
			switch (op) {
			case AND:			return " & ";
			case OR:			return " ++ ";
			case EQUIV:			return " <-> ";
			case IMPL:			return " -> ";
			case REV_IMPL:		return " <- ";
			default:			return "<UNKNOWN_BOP>";
			}
		}
	};
	
	/// Functor to determine the domain type of the element
	struct domaintype {
		DomainType::type operator()(type op) {
			return DomainType::NO_DOMAIN;
		}
	};
};

/// Structure descrbing comparison between two terms.
struct ComparisonOperator {
		enum type {
			EQ,
			NEQ,
			LTHAN,
			GTHAN,
			LTHAN_EQ,
			GTHAN_EQ
		};

	/// Functor to convert binary formula operators to cstrings.
	struct cstr {

		char const* operator() (type op) {
			switch (op) {
			case EQ:			return " == ";
			case NEQ:			return " \\= ";
			case LTHAN:			return " < ";
			case GTHAN:			return " > ";
			case LTHAN_EQ:		return " =< ";
			case GTHAN_EQ:		return " >= ";
			default:			return "<UNKNOWN_COMPARISON_OP>";
			}
		}
	};
	
	/// Functor to determine the domain type of th element
	struct domaintype {
		DomainType::type operator()(type op) {
			return DomainType::NO_DOMAIN;
		}
	};
};

/// Structure describing unary formula operators.
struct UnaryFormulaOperator {
	enum type {
			NOT,
            NOT_DASH,
            STRONG_NOT,
            NEGATIVE,
            ABS,
            EXOGENOUS,
            INERTIAL,
            RIGID,
            CHOICE
	};

	/// Functor to map unary formula operators to prenix symbols.
	struct pre_cstr {
			char const* operator()(type op) {
				switch (op) {
				case NOT: 			return "not ";
				case NOT_DASH:		return "-";
				case STRONG_NOT:	return "~";
				case NEGATIVE:		return "-";
				case ABS:			return "abs ";
				case EXOGENOUS:		return "exogenous ";
				case INERTIAL:		return "inertial ";
				case RIGID:			return "rigid ";
				case CHOICE:		return "{";
				default:			return "<UNKNOWN_UOP>";
				}
			}
	};

	/// Functor to map unary formula operators to postnix symbols.
	struct post_cstr {
			char const* operator()(type op) {
				switch (op) {
				case CHOICE:		return "}";
				default:			return "";
				}
			}
	};
	
	/// Functor to determine the domain type of the element
	struct domaintype {
		DomainType::type operator()(type op) {
			return DomainType::NO_DOMAIN;
		}
	};

};


struct NullaryFormulaOperator {
	enum type {
		TRUE,
		FALSE
	};

	struct cstr {
		char const* operator()(type op) {
			switch (op) {
			case TRUE: return "true";
			case FALSE: return "false";
			default: return "<UNKNOWN_NOP>";
			}
		}
	};

	struct domaintype {
		DomainType::type operator()(type op) {
			return DomainType::NO_DOMAIN;
		}
	};	
};

}

/**
 * @brief An arbitrary formula that evaluates to true or false.
 */
typedef detail::ElementClass<
	Element::Type::FORMULA, 
	detail::FormulaType > Formula;

/** 
 * @brief A binary formula of the form (F @ G) where '@' is some operator.
 */
typedef detail::BinaryElement<
	Formula,
	detail::FormulaType::BINARY, 
	detail::BinaryFormulaOperator,
	Formula, Formula, 
	detail::BinaryFormulaOperator::cstr,
	detail::BinaryFormulaOperator::domaintype > BinaryFormula;

/** 
 * @brief A binary formula of the form (F @ G) where '@' is some operator.
 */
typedef detail::BinaryElement<
	Formula,
	detail::FormulaType::COMPARISON, 
	detail::ComparisonOperator, 
	Term, Term,
	detail::ComparisonOperator::cstr,
	detail::ComparisonOperator::domaintype > ComparisonFormula;


/**
 * @brief A unary formula of the form @F where '@' is some operator.
 */
typedef detail::UnaryElement<
	Formula, 
	detail::FormulaType::UNARY,
	detail::UnaryFormulaOperator, 
	Formula,
	detail::UnaryFormulaOperator::pre_cstr, 
	detail::UnaryFormulaOperator::post_cstr,
	detail::UnaryFormulaOperator::domaintype > UnaryFormula;


/**
 * @brief A nullary formula "true" or "false"
 */
typedef detail::NullaryElement<
	Formula,
	detail::FormulaType::NULLARY,
	detail::NullaryFormulaOperator,
	detail::NullaryFormulaOperator::cstr,
	detail::NullaryFormulaOperator::domaintype > NullaryFormula;

/**
 * @brief a binding of the form i:F where i is a step and F is a formula.
 */
typedef detail::Binding<
	Formula,
	detail::FormulaType::BINDING,
	Term,
	Formula> BindingFormula;




}}
#include "bcplus/elements/AtomicFormula.h"
#include "bcplus/elements/QuantifierFormula.h"
#include "bcplus/elements/CardinalityFormula.h"
