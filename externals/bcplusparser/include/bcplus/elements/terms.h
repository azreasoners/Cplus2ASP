#pragma once

#include "bcplus/symbols/ObjectSymbol.h"
#include "bcplus/symbols/VariableSymbol.h"
#include "bcplus/symbols/ConstantSymbol.h"
#include "bcplus/elements/detail/ElementClass.h"
#include "bcplus/elements/detail/BinaryElement.h"
#include "bcplus/elements/detail/UnaryElement.h"
#include "bcplus/elements/detail/NullaryElement.h"
#include "bcplus/elements/detail/IdentifierElement.h"
#include "bcplus/elements/detail/AnonymousElement.h"
#include "bcplus/elements/detail/Binding.h"
#include "bcplus/elements/detail/TermType.h"
namespace bcplus {
namespace elements {

namespace detail {

/// Container describing the binary operators for terms
struct BinaryTermOperator {
		/// The possible binary operators.
		enum type {
			PLUS,
			MINUS,
			TIMES,
			DIVIDE,
			MOD
		};

	/// Functor converting binary operators to cstrings
	struct cstr {
			char const* operator()(type op) {
				switch (op) {
				case PLUS:		return " + ";
				case MINUS:		return " - ";
				case TIMES:		return " * ";
				case DIVIDE:	return " / ";
				case MOD:		return " mod ";
				default:		return "<BAD_TERM_BOP>";
				}
			};

	};
	
	/// Functor to determine if the term is domaintype
	struct domaintype {
		DomainType::type operator()(type op) {
			return DomainType::INTEGRAL;
		}
	};

};

/// Container describing unary oeprators for terms
struct UnaryTermOperator {
	/// The possible unary operators
	enum type {
		NEGATIVE,
		ABS
	};

	/// Functor mapping each operator to its prenix cstring
	struct pre_cstr {

			char const* operator()(type op) {
				switch (op) {
				case NEGATIVE:		return "-";
				case ABS:			return "abs ";
				default:			return "<BAD_TERM_UOP> ";
				}
			}
	};

	/// Functor mapping each operator to its postnix cstring
	struct post_cstr {

			char const* operator()(type op) {
				switch (op) {
				default:			return "";
				}
			}
	};

	/// Functor to determine if the domain type of the term
	struct domaintype {
		DomainType::type operator()(type op) {
			return DomainType::INTEGRAL;
		}
	};

};

struct NullaryTermOperator {
	enum type {
		MAXSTEP,
		MAXADDITIVE,
		MAXAFVALUE
	};

	struct cstr {
		char const* operator()(type op) {
			switch (op) {
			case MAXSTEP: return "maxstep";
			case MAXADDITIVE: return "maxAdditive";
			case MAXAFVALUE: return "maxAFValue";
			default: return "<UNKNOWN_NOP>";
			}
		}
	};

	struct domaintype {
		DomainType::type operator()(type op) {
			return DomainType::INTEGRAL;
		}
	};	
};

}

/**
 * @brief An abritrary term.
 */
typedef detail::ElementClass<
	Element::Type::TERM, 
	detail::TermType> Term;

/**
 * @brief A binary operator term of the form T @ V where T,V are terms and @ is a binary operator.
 */
typedef detail::BinaryElement<
	Term,
	detail::TermType::BINARY, 
	detail::BinaryTermOperator, 
	Term, Term,
	detail::BinaryTermOperator::cstr,
	detail::BinaryTermOperator::domaintype> BinaryTerm;
/**
 * @brief A unary operator term of the form @T where @ is a unary operator and T is a term.
 */
typedef detail::UnaryElement<
	Term,
	detail::TermType::UNARY,  
	detail::UnaryTermOperator, 
	Term,
	detail::UnaryTermOperator::pre_cstr, 
	detail::UnaryTermOperator::post_cstr,
	detail::UnaryTermOperator::domaintype> UnaryTerm;

/**
 * @brief A LUA call.
 */
typedef detail::AnonymousElement<
	Term,
	detail::TermType::LUA,
	Term> LuaTerm;


/*
* @brief A Python Call.
*/
typedef detail::AnonymousElement<
	Term,
	detail::TermType::PYTHON,
	Term> PyTerm;

/**
 * @brief An anonymous variable.
 */
typedef detail::AnonymousElement_bare<
	Term,
	detail::TermType::ANON_VAR> AnonymousVariable;


/**
 * @brief An object constant 't'
 */
typedef detail::IdentifierElement<
	Term, 
	detail::TermType::OBJECT, 
	symbols::ObjectSymbol, 
	Term> Object;

/**
 * @brief A variable 'V'.
 */
typedef detail::IdentifierElement_bare<
	Term, 
	detail::TermType::VARIABLE, 
	symbols::VariableSymbol> Variable;

/** 
 * @brief A bare constant 'c'
 */
typedef detail::IdentifierElement<
	Term,
	detail::TermType::CONSTANT, 
	symbols::ConstantSymbol, 
	Term> Constant;

/**
 * @brief A nullary term, such as keyword "maxstep", "maxAFValue", and "maxAdditive"
 */
typedef detail::NullaryElement<
	Term,
	detail::TermType::NULLARY,
	detail::NullaryTermOperator,
	detail::NullaryTermOperator::cstr,
	detail::NullaryTermOperator::domaintype > NullaryTerm;


/**
 * @brief A step binding of the form i:T where T is a term.
 */
typedef detail::Binding<
	Term,
	detail::TermType::BINDING,
	Term, Term > BindingTerm;


}}

