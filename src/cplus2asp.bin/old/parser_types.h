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


#ifndef PARSER_TYPES_H
#define PARSER_TYPES_H

/**
 * @file parser_types.h
 * @brief Data structures used to hold intermediate data for the parser.
 * @details Declarations of data structures used by the parser to contain
 * intermediate data while it processes grammar rules.
 */

#include <sstream>
#include <string>
#include <list>
#include <utility>
#include <exception>

#include "DataStructs/Constant.h"
#include "DataStructs/Context.h"
#include "DataStructs/Sort.h"
#include "DataStructs/Variable.h"
#include "utilities.h"
#include "types.h"

class Object;



/**
 * Container class to hold information from the constant_binder grammar rule.
 */
class PT_constant_binder_t
{
public:
	Constant::ConstantType constType; ///< The binding constant type (e.g., CONST_EXOGENOUSACTION).
	Sort* domain; ///< The domain of the constant.
	Constant* parent; ///< If the constant binder is an attribute, this is a reference to the connecting action constant.
	
	/**
	 * Default constructor. Sets all dynamic attributes to NULL.
	 */
	PT_constant_binder_t();
	
	/**
	 * Destructor. Just resets everything to NULL.
	 */
	~PT_constant_binder_t();
};

/**
 * General parent of all self-contained, self-translating parse tree elements.
 * Empty except for the virtual translate function.
 */
class ParseElement
{
public:
	/// The possible types of a ParseElement instance.
	enum ParseElementType { PELEM_PELEM, PELEM_UOP, PELEM_BOP, PELEM_QUANT, PELEM_BASELIKE, PELEM_CONSTLIKE, PELEM_OBJLIKE, PELEM_VARLIKE };

	/// Mask values for the various kinds of constants that can be in a formula.
	enum ConstMask {
		MASK_ACTION 	= 0x01,
		MASK_FLUENT 	= 0x02,
		MASK_RIGID  	= 0x04,
		MASK_STATIC_AB 	= 0x08,
		MASK_DYNAMIC_AB	= 0x10,
		MASK_TRUE_FALSE = 0x20,

		MASK_SDFLUENT = 0x40,			// NOTE: sdFluent's are also contained within the FLUENT group.

		MASK_AB = MASK_STATIC_AB | MASK_DYNAMIC_AB,
		MASK_NON_TRIVIAL = ~MASK_TRUE_FALSE,
		MASK_NON_RIGID = MASK_NON_TRIVIAL & ~MASK_RIGID,
		MASK_NON_AB = MASK_NON_TRIVIAL & ~MASK_AB
	};

private:

	std::string mQueryTimeStamp; ///< A timestamp used for query formulas.
	bool mParens; ///< If true, the expression should be wrapped in parentheses
	enum ParseElementType mElemType; ///< The type of the parse element instance.

	/// A global count of how many extra clauses have been created for any element.
	/// Used to help create dynamic variable names that won't clash with each other.
	/// Incremented when an extra clause is created, but not when it is added to anything.
	static size_t sVarCount;
	
public:

	/**
	 * Default constructor. Sets elemType.
	 * @param elemType The type that the element is.
	 * @param parens Whether to add parens around the element.
	 */
	inline ParseElement(ParseElementType elemType, bool parens = false)
		: mQueryTimeStamp(""), mParens(parens), mElemType(elemType)
		{ /* Intentionally Left Blank */ }
	

	/**
	 * Method that generates a translated string representation of the element with 
	 * preceeding @ if a symbol is understood as a lua function call.
	 * @param[out] out - The output stream to write to.
	 * @param context -  The formula context to be used for translation.
	 * @return out.
	 */
	virtual std::ostream& translate(std::ostream& out, Context& context) const = 0;

	/**
	 * Returns true if the element (or any of its children) are constants of the provided types.
	 * @param types A mask containing flags for the types to search for. See ConstMask enum.
	 * @param includeParams Whether to check the parameters of constants.
	 * @param includeEq Whether to check the right side of equality within an atom. (The right hand side of comparisons will be checked regardless.)
	 * @return True if the element, or any elements "beneach" it, are constants, false otherwise.
	 */
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const = 0;
	
	/**
	 * Determines if the element contains a single atom, a unary expression w/ a single atom, or a conjunction thereof.
	 * @param allowComparison Whether to allow comparison operators in the formula.
     * @param allowChoice Whether to allow choice rules in the formula.
	 * @param allowAtomicNegation whether to allow -p or "not p" where p is a boolean atom.
	 * @return True if the element is a definite formula.
	 */
	virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const = 0;

	/**
	 * Whether the formula is a single atom.
	 * @return True if the element is a single atom c=v (or just c if c is boolean).
	 * @param allowAtomicNegation whether to allow -p or "not p" where p is a boolean atom.
	 */
	virtual bool isSingleAtom(bool allowAtomicNegation = false) const = 0;
        
	/**
	 * Determines if the element contains no (real) constants. (i.e. only "true" and/or "false").
	 * @param includeParams Whether we should check parameter lists.
	 * @param includeEq Whether we should check atomic equality.
	 * @return True if the only constants contained in the formula are "true" and "false".
	 */
	inline bool isTrivial(bool includeParams = true, bool includeEq = true) const { return !hasConstants(MASK_NON_TRIVIAL, includeParams, includeEq); }

	/**
	 * Determines if the element is a valid arithmetic expression.
	 * @return True if the element and all it's children are arithmetic expressions.
	 */
	virtual bool isArithExpr() const = 0;

	/**
	 * Determines if the element contains any calls to LUA.
	 * @param includeParams Whether to search base element parameter lists.
	 * @param includeEq Whether to check the right side of equality within an atom. (The right hand side of comparisons will be checked regardless.)
	 * @return True if the element or any of its children is a LUA call.
	 */
	virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const = 0;

	/**
	 * Determines the proper IPart to place the formula in.
	 * @return IPART_BASE if the formula is 0:(...), IPART_VOLATILE if the formula is X:(...), and IPART_CUMULATIVE if the formula is anything else.
	 */
	virtual IPart determineQueryIPart() const = 0;

	/**
	 * Prints the untranslated element string.
	 * @param out The output stream to print to.
	 * @return out.
	 */
	virtual std::ostream& fullName(std::ostream& out) const = 0;

	/// Returns an exact copy of the parse element.
	virtual ParseElement* copy() const = 0;

	/**
	 * Aggregates all of the undefined identifiers (which are assumed to be constants).
	 * @param outIdentifiers The undefined identifiers.
	 */
	virtual void aggregateUndefined(BaseElementList& outIdentifiers) = 0;


	/// Alternative helper to fullName(std::ostream&).
	inline std::string fullName() const { std::ostringstream ret; fullName(ret); return ret.str(); }

	/// Gets the type of the parse element.
	inline ParseElementType getType() const { return mElemType; }

	/**
	 * Sets whether the expression is wrapped in parentheses.
	 * @param parens Whether the expression should be wrapped in parentheses.
	 */
	inline void parens(bool parens) { this->mParens = parens; }

	/// Gets whether the element is encompassed in parens
	inline bool parens() const { return mParens; }

	/**
	 * Destructor. Empty.
	 */
	inline virtual ~ParseElement() { /* Intentionally Left Blank */};

	/*******************************************************************/
	/* Static Helpers */
	/*******************************************************************/

	/**
	 * Gets a new variable whose name is unique within the rule we're translating.
	 * @return The name of the new variable.
	 */
	static inline std::string getNewVar() { return std::string("X_Value_") + utils::to_string(sVarCount++); }


protected:

	/**
	 * Determines if the element has parentheses around it.
	 * @return True if the element has parentheses around it, false otherwise.
	 */
	inline bool hasParens() const { return mParens; }

	/**
	 * Helper to translate that intelligently handles the value of parenBefore.
	 * @param forceParens Whether to force the presence of parens.
	 * @return A blank string if parenBefore is false, "(" if it's true.
	 */
	inline std::string translateBeforeParen(bool forceParens = false) const { return (hasParens() | forceParens) ? "(" : ""; }

	/**
	 * Helper to translate that intelligently handles the value of parenAfter.
	 * @param forceParens Whether to force the presence of parens.
	 * @return A blank string if parenAfter is false, ")" if it's true.
	 */
	inline std::string translateAfterParen(bool forceParens = false) const { return (hasParens() | forceParens) ? ")" : ""; }
};

/**
 * Child of ParseElement that represents a unary operator and the associated elements it operates on.
 */
class SimpleUnaryOperator : public ParseElement
{
public:
	/// Enum of the kinds of unary operators this class can be.
	enum UnaryOperatorType { UOP_UNKNOWN, UOP_NOT, UOP_STRONG_NOT, UOP_NEGATIVE, UOP_ABS, UOP_EXOGENOUS, UOP_INERTIAL, UOP_RIGID, UOP_CHOICE };
	
private:
	enum UnaryOperatorType mOpType;		///< Which kind of operator an instance represents.
	ParseElement* mPostOp; 				///< What comes after (and is affected by) the operator.

public:
	
	/**
	 * Full constructor.
	 * @param opType the type of operation the operator should perform.
	 * @param postOp the subexpression which occurs after the operator.
	 * @param parens Whether to add parens around the element.
	 */
	inline SimpleUnaryOperator(UnaryOperatorType opType, ParseElement* postOp, bool parens = false)
		: ParseElement(PELEM_UOP, parens), mOpType(opType), mPostOp(postOp)
	{ /* IntentionallY Left Blank */ }
	
	// inherited stuffs
	virtual std::ostream& translate(std::ostream& out, Context& context) const;
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const;
	virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const;
    virtual bool isSingleAtom(bool allowAtomicNegation = false) const;
	virtual bool isArithExpr() const;
	virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const 
																{ return postOp() && postOp()->hasLuaCalls(includeParams, includeEq); }
	inline virtual IPart determineQueryIPart() const			{ return (postOp()) ? postOp()->determineQueryIPart() : IPART_BASE; }
	virtual std::ostream& fullName(std::ostream& out) const;
	inline virtual ParseElement* copy() const					{ return new SimpleUnaryOperator(opType(), (postOp()) ? postOp()->copy() : NULL, parens()); }
	inline virtual void aggregateUndefined(BaseElementList& outIdentifiers)	{ if (postOp()) mPostOp->aggregateUndefined(outIdentifiers); }


	/// Gets the operator's type.
	inline UnaryOperatorType opType() const 					{ return mOpType; }

	/// Sets the operator's type.
	inline void opType(UnaryOperatorType op) 					{ mOpType = op; }

	/// Gets the operator's sub expression.
	inline ParseElement const* postOp() const					{ return mPostOp; }

	/// Sets the operator's sub expression, deallocating any existing sub expression.
	inline void postOp(ParseElement* postOp)					{ if (mPostOp) delete mPostOp; mPostOp = postOp; }

	/// Detaches the operator's sub expression, returning it.
	ParseElement* detachPostOp();

	/**
	 * Destructor. Deallocates postOp.
	 */
	virtual ~SimpleUnaryOperator();

protected:

	/**
	 * Simple helper function. Converts the provided operator to a string representation.
	 * @param op The operator to convert
	 * @return A string corresponding to that operator.
	 */
	static std::string translateOpType(UnaryOperatorType op);

};

class SimpleBinaryOperatorWrapper;

/**
 * Child of ParseElement that represents a binary operator and the associated elements it operates on.
 */
class SimpleBinaryOperator : public ParseElement
{
	// The wrapper class is used to efficiently use a new operator in a readonly environment.
	friend class SimpleBinaryOperatorWrapper;

public:
	/// Enum of the kinds of unary operators this class can be.
		enum BinaryOperatorType {
			BOP_UNKNOWN,
			BOP_PLUS,
			BOP_MINUS,
			BOP_TIMES,
			BOP_DIVIDE,
			BOP_MOD,
			BOP_AND,
			BOP_OR,
			BOP_EQUIV,
			BOP_IMPL,
			BOP_REV_IMPL, // fake operator to simulate F <- G.
			BOP_EQ,
			BOP_NEQ,
			BOP_DBL_EQ,
			BOP_LTHAN,
			BOP_GTHAN,
			BOP_LTHAN_EQ,
			BOP_GTHAN_EQ,
			BOP_BIND_STEP
		};
private:
		enum BinaryOperatorType mOpType; ///< Which kind of operator an instance represents.

		ParseElement* mPreOp; ///< The bit(s) bound to the operator that come before it.
		ParseElement* mPostOp; ///< The bit(s) bound to the operator that come after it.
public:
	
	/**
	 * Full constructor.
	 * @param preOp The sub expression that comes before the operator.
	 * @param opType The binary operator.
	 * @param postOp The sub expression that comes after the operator.
	 * @param parens Whether to encase the expression in parens.
	 */
	inline SimpleBinaryOperator(ParseElement* preOp, BinaryOperatorType opType, ParseElement* postOp, bool parens = false)
		: ParseElement(PELEM_BOP, parens), mOpType(opType), mPreOp(preOp), mPostOp(postOp)
	{ /* Intentionally Left Blank */ }
	

	// inherted stuffs
	inline virtual std::ostream& translate(std::ostream& out, Context& context) const { return translate(out, context, opType()); }
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const;
	virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const;
    virtual bool isSingleAtom(bool allowAtomicNegation = false) const;
	virtual bool isArithExpr() const;
	virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const; 
	virtual IPart determineQueryIPart() const;
	virtual std::ostream& fullName(std::ostream& out) const;
	inline virtual ParseElement* copy() const					{ return new SimpleBinaryOperator(preOp()->copy(), opType(), postOp()->copy(), parens()); }
	virtual void aggregateUndefined(BaseElementList& outIdentifiers);

	/// Gets the operator's type.
	inline BinaryOperatorType opType() const 	{ return mOpType; }

	/// Sets the operator's type.
	inline void opType(BinaryOperatorType op) 	{ mOpType = op; }

	/// Determines if this binary operator is an atomic equality.
	/// (ie. c=v where c is a constant and v is some value.
	inline bool isAtomicEq() const { 
		return (opType() == BOP_EQ
					&& preOp() && postOp()
					&& preOp()->getType() == PELEM_CONSTLIKE
					&& preOp()->hasConstants(MASK_NON_TRIVIAL, false, false)
					&& !postOp()->hasConstants(MASK_NON_TRIVIAL, false, true));
	}


	/// Gets the operator's sub expression.
	inline ParseElement const* postOp() const	{ return mPostOp; }
	inline ParseElement const* preOp() const	{ return mPreOp; }

	/// Sets the operator's sub expression, deallocating any existing sub expression.
	inline void postOp(ParseElement* postOp)	{ if (mPostOp) delete mPostOp; mPostOp = postOp; }
	inline void preOp(ParseElement* postOp)		{ if (mPreOp) delete mPreOp; mPreOp = postOp; }

	/// Detaches the operator's sub expression, returning it.
	ParseElement* detachPostOp();
	ParseElement* detachPreOp();

	/**
	 * Destructor. Deallocates preOp and postOp.
	 */
	virtual ~SimpleBinaryOperator();

protected:

	/**
	 * Translates the expression with the provided operator type.
	 * @param[out] out - The output stream to write to.
	 * @param context -  The formula context to be used for translation.
	 * @param type - The operator type to use for the translation.
	 * @param forceParens - Whether to force the presence of parens around the expression.
	 * @return out.
	 */
	std::ostream& translate(std::ostream& out, Context& context, BinaryOperatorType type, bool forceParens = false) const;

	/**
	 * Simple helper function. Converts the provided operator to a string representation.
	 * @param op The operator to convert
	 * @return A string corresponding to that operator.
	 */
	static std::string translateOpType(BinaryOperatorType op);
};

/**
 * Child of ParseElement that represents a big quantifier group and its associated subformula.
 */
class BigQuantifiers : public ParseElement
{
public:
	/// Which kinds of "quantifiers" can be present.
	enum QuantifierType { QUANT_UNKNOWN, QUANT_CONJ, QUANT_DISJ };

	/// A single quantifier
	typedef std::pair<BigQuantifiers::QuantifierType, ParseElement*> Quantifier;

	/// A list of quantifiers and their associated variable.
	typedef std::list<Quantifier*> QuantifierList;


private:
	/// A list of the quantifiers present in this grouping, both the kinds
	/// of quantifiers and references to the quantifying variables.
	QuantifierList mQuants;
	ParseElement* mPostOp; ///< The bit(s) affected by the quantifier(s).
	
public:

	/**
	 * Full Constructor.
	 * @param quants The list of quantifiers that encase the sub expression. (Null initializes the expression to have no quantifiers).
	 * @param postOp The sub expression that occurs after the quantifiers.
	 * @param parens Whether to encase the expression in parens.
	 */
	inline BigQuantifiers(QuantifierList const* quants = NULL, ParseElement* postOp = NULL, bool parens = false)
		: ParseElement(ParseElement::PELEM_QUANT, parens), mPostOp(postOp) {
		if (quants) mQuants = *quants;
	}
	

	// inherited stuffs
	virtual std::ostream& translate(std::ostream& out, Context& context) const;
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const;
	inline virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const			{ return false; }
    inline virtual bool isSingleAtom(bool allowAtomicNegation = false) const                                { return false; }
	inline virtual bool isArithExpr() const						{ return false; }
	inline virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const 	
																{ return postOp() && postOp()->hasLuaCalls(includeParams, includeEq); }
	virtual inline IPart determineQueryIPart() const			{ return (postOp()) ? postOp()->determineQueryIPart() : IPART_BASE; }
	virtual std::ostream& fullName(std::ostream& out) const;
	virtual ParseElement* copy() const;
	virtual void aggregateUndefined(BaseElementList& outIdentifiers);

	/// Gets the begining of the list of quantifiers.
	inline QuantifierList::const_iterator quantsBegin() const 	{ return mQuants.begin(); }

	/// Gets the end of the list of quantifiers.
	inline QuantifierList::const_iterator quantsEnd() const 	{ return mQuants.end(); }

	/// Gets the number of quantifiers associated with this instance.
	inline size_t numQuants() const								{ return mQuants.size(); }


	/// Adds a quantifier to the list.
	inline void addQuant(Quantifier* newQuant)					{ mQuants.push_back(newQuant); }

	/// Gets the operator's sub expression.
	inline ParseElement const* postOp() const	{ return mPostOp; }

	/// Sets the operator's sub expression, deallocating any existing sub expression.
	inline void postOp(ParseElement* postOp)	{ if (mPostOp) delete mPostOp; mPostOp = postOp; }

	/// Detaches the operator's sub expression, returning it.
	ParseElement* detachPostOp();

	/**
	 * Destructor. Deallocates postOp and the contents of quants.
	 */
	virtual ~BigQuantifiers();

protected:
	/// Gets the begining of the list of quantifiers.
	inline QuantifierList::iterator quantsBegin() 	{ return mQuants.begin(); }

	/// Gets the end of the list of quantifiers.
	inline QuantifierList::iterator quantsEnd() 	{ return mQuants.end(); }
};


/**
 * A simple (readonly) wrapper class to easily and efficiently change the operator.
 */
class SimpleBinaryOperatorWrapper : public ParseElement {
private:
	SimpleBinaryOperator const* mWrapped;
	SimpleBinaryOperator::BinaryOperatorType mType;
	bool mForceParens;
public:

	/**
	 * Constructor.
	 * @param wrappedExpr The expression to wrap.
	 * @param type The new operator type.
	 * @param forceParens Whether to force the presence of parens around the expression.
	 */
	inline SimpleBinaryOperatorWrapper(SimpleBinaryOperator const* wrappedExpr, SimpleBinaryOperator::BinaryOperatorType type, bool forceParens = false)
		: ParseElement(wrappedExpr->getType(), wrappedExpr->parens()), mWrapped(wrappedExpr), mType(type), mForceParens(forceParens)
	{ /* Intetionally Left Blank */ }


	virtual inline std::ostream& translate(std::ostream& out, Context& context) const
																{ return mWrapped->translate(out, context, opType(), mForceParens); }
	inline virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const
																{ return mWrapped->hasConstants(types, includeParams); }
	inline virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const			
			{ return mWrapped->isDefinite(allowComparison, allowChoice, allowAtomicNegation); }
	inline virtual bool isSingleAtom(bool allowAtomicNegation = false) const                                { return mWrapped->isSingleAtom(allowAtomicNegation); }
        inline virtual bool isArithExpr() const						{ return mWrapped->isArithExpr(); }
	inline virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const	
																{ return mWrapped->hasLuaCalls(includeParams, includeEq); }
	inline virtual IPart determineQueryIPart() const			{ return mWrapped->determineQueryIPart(); }
	inline virtual std::ostream& fullName(std::ostream& out) const
																{ return mWrapped->fullName(out); }
	inline virtual ParseElement* copy() const					{ return new SimpleBinaryOperatorWrapper(mWrapped, mType, mForceParens); }
	inline virtual void aggregateUndefined(BaseElementList& outIdentifiers) { /* Not Supported */ }

	/// Gets the operator's type.
	inline SimpleBinaryOperator::BinaryOperatorType opType() const 	{ return mType; }

	/// Sets the operator's type.
	inline void opType(SimpleBinaryOperator::BinaryOperatorType op) { mType = op; }



	/// Gets the operator's sub expression.
	inline ParseElement const* postOp() const	{ return mWrapped->postOp(); }
	inline ParseElement const* preOp() const	{ return mWrapped->preOp(); }


	/// Destructor. Does nothing.
	inline virtual ~SimpleBinaryOperatorWrapper() { /* Intetionally Left Blank */ }

};


/**
 * Child of ParseElement that acts as a base class for parsed elements like constants and objects.
 */
class BaseLikeElement : public ParseElement
{
public:


private:
	std::string mBaseName; 		///< The original name of the element, sans parameters.
	ParseElementList mParams; 		///< References to representations of the element's parameters, if any.
	Element const* mSymbolRef;	///< The reference to the global symbol which this element is an instance of.
public:

	/**
	 * Full constructor.
	 * @param elemType The concrete type that's being instantiated.
	 * @param baseName The base name of the element that's being instantiated (without parameters).
	 * @param symbolRef A pointer to the symbol that this element is an instance of.
	 * @param params The list of parameters associated with this instance. Null if there are no parameters.
	 * @param parens Whether the element should be encased in parens.
	 */
	inline BaseLikeElement(ParseElementType elemType, std::string const& baseName, Element const* symbolRef, ParseElementList* params = NULL, bool parens = false)
		: ParseElement(elemType, parens), mBaseName(baseName), mSymbolRef(symbolRef)
	{ 
		if (params) mParams = *params; 
	}

	// inherited stuffs
	virtual std::ostream& translate(std::ostream& out, Context& context) const = 0;
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const = 0;
	virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const = 0;
    inline virtual bool isSingleAtom(bool allowAtomicNegation = false) const = 0;
	inline virtual bool isArithExpr() const 								{ return isNumeric(); }
	virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const = 0;
	virtual IPart determineQueryIPart() const = 0;
	virtual std::ostream& fullName(std::ostream& out) const;
	virtual ParseElement* copy() const = 0;
	inline virtual void aggregateUndefined(BaseElementList& outIdentifiers) { if (!ref()) outIdentifiers.push_back(this); }
	/**
	 * Determines if the element ranges over numeric values.
	 * @return True if the element ranges over strictly numeric values.
	 */
	virtual bool isNumeric() const = 0;

	/**
	 * Determines if the element ranges over boolean values.
	 * @return True if the element ranges over strictly boolean values, false otherwise.
	 */
	virtual bool isBoolean() const = 0;


	/// Gets the base name for the element.
	inline std::string const& baseName() const 					{ return mBaseName; }

	/// Gets a reference to the global symbol which this element is an instance of.
	inline Element const* ref() const 							{ return mSymbolRef; }

	/// Sets the global symbol which this element is an instance of
	inline void ref(Element const* _ref)						{ mSymbolRef = _ref; }

	/// Gets an iterator for the beginning of the parameter list.
	inline ParseElementList::const_iterator paramsBegin() const 	{ return mParams.begin(); }

	/// Gets an iterator for the end of the parameter list.
	inline ParseElementList::const_iterator paramsEnd() const 		{ return mParams.end(); }

	/// Gets the number of parameters the element has.
	inline size_t arity() const										{ return mParams.size(); }


	/// Clears the list of parameters without deallocating them.
	inline void detachParams() 									{ mParams.clear(); }


	/**
	 * Attempts to guess the sorts of each parameter.
	 * This really will only work if every parameter is a variable.
	 * @param outSorts The list of sorts that we've guess (entries may be NULL if we don't know.).
	 * @return True if all sorts have been successfully guessed. False otherwise.
	 */
	virtual bool guessParamSorts(ConstSortList& outSorts);

	/**
	 * Internal helper to translate, creates a list of translated params.
	 * @param[out] out The stream to output to.
	 * @param context The context used for translation.
	 * @param force Whether to force the presence of parens regardless of the presence of arguments.
	 * @return A parentheses-surrounded, comma-separated list of translated
	 * params, or a blank string if params is empty.
	 */
	virtual std::ostream& translateParams(std::ostream& out, Context& context, bool force = false) const;
	
	/**
	 * Destructor. Deallocates contents of params.
	 */
	virtual ~BaseLikeElement();

protected:

	/// Gets an iterator for the beginning of the parameter list.
	inline ParseElementList::iterator paramsBegin() { return mParams.begin(); }

	/// Gets an iterator for the end of the parameter list.
	inline ParseElementList::iterator paramsEnd() { return mParams.end(); }


	/// Determines if the base element has a LUA call in its parameter list.
	bool hasLuaCallParameters() const;

	/// Checks to see if there is a nested constant matching the type mask within the parameter list.
	/// @param types A mask of the types of elements to look for.
	bool hasConstantParameters(unsigned int types) const;

};

/**
 * Child of BaseLikeElement that represents a base element with the behavior of a constant.
 */
class ConstantLikeElement : public BaseLikeElement
{
public:
	
	/**
	 * Full constructor.
	 * @param baseName The base name of the element that's being instantiated (without parameters).
	 * @param symbolRef A pointer to the symbol that this element is an instance of.
	 * @param params The list of parameters associated with this instance. Null if there are no parameters.
	 * @param parens Whether to encase the element in parens.
	 */
	inline ConstantLikeElement(std::string const& baseName, Constant const* symbolRef, ParseElementList* params = NULL, bool parens = false)
		: BaseLikeElement(PELEM_CONSTLIKE, baseName, symbolRef, params, parens)
	{ /* Intentionally Left Blank */ }


	// inherited stuffs
	virtual std::ostream& translate(std::ostream& out, Context& context) const;
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const;
	inline virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const		
		{ return ref() && ((Constant const*)ref())->isBoolean(); }
    inline virtual bool isSingleAtom(bool allowAtomicNegation = false) const                                                        
		{ return ref() && ((Constant const*)ref())->isBoolean(); }
	inline virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const	
														{ return includeParams && hasLuaCallParameters(); }
	inline virtual IPart determineQueryIPart() const 		{ return IPART_CUMULATIVE; }
	virtual ParseElement* copy() const;


	inline virtual bool isNumeric() const 					{ return ref() && ref()->isNumeric(); }
	inline virtual bool isBoolean() const 					{ return !ref() || ref()->isBoolean(); }

	/// Gets the domain of the constant ( or null )
	inline Sort const* domain() const							{ return (ref()) ? ((Constant const*)ref())->domain() : NULL; }
	inline bool isContribConstant() const						{ return ref() && ((Constant const*)ref())->isContribConstant(); }

	/**
	 * Destructor. Does not deallocate anything.
	 */
	inline virtual ~ConstantLikeElement() { /* Intentionally Left Blank */ }

protected:

	/**
	 * Outputs the string this constant uses as a prefix for the atom.
	 * @return A prefix to the atom's name, usually "h(eql(".
	 */
	inline std::string translatePrefix() const
		{ return (ref()) ? Constant::translatePrefix(((Constant const*)ref())->constType()) : ""; }

	/**
	 * Outputs the string used as a postfix. This is usually ",<value>),<timestamp>)".
	 * @param context - The context to be used for translation.
	 * @return A postfix to the atom's name.
	 */
	inline std::string translatePostfix(Context& context) const
		{ return (ref()) ? Constant::translatePostfix(((Constant const*)ref())->constType(), context) : ""; }

	/**
	 * Helper translator method to handle translating the constant as an atom.
	 * @param[out] out - The output stream to write the translation to.
	 * preceeding @ if a symbol is understood as a lua function call.
	 * @param context -  The formula context to be used for translation.
	 * Uses constant translating conventions for the base name.
	 * @return A string containing the translated form of the element.
	 */
	virtual std::ostream& translateAsConstant(std::ostream& out, Context& context) const;

	/**
	 * Helper translator method to handle translating the constant as a variable.
	 * @param[out] out - The output stream to write the translation to.
	 * preceeding @ if a symbol is understood as a lua function call.
	 * @param context -  The formula context to be used for translation.
	 * Uses constant translating conventions for the base name.
	 * @return A string containing the translated form of the element.
	 */
	virtual std::ostream& translateAsVariable(std::ostream& out, Context& context) const;

};

/**
 * Child of BaseLikeElement that represents a base element with the behavior of an object.
 */
class ObjectLikeElement : public BaseLikeElement
{
public:

	/**
	 * Full constructor.
	 * @param baseName The base name of the element that's being instantiated (without parameters).
	 * @param symbolRef A pointer to the symbol that this element is an instance of.
	 * @param params The list of parameters associated with this instance. Null if there are no parameters.
	 * @param parens Whether to encase the element in parens.
	 */
	inline ObjectLikeElement(std::string const& baseName, Object const* symbolRef, ParseElementList* params = NULL, bool parens = false)
		: BaseLikeElement(PELEM_OBJLIKE, baseName, symbolRef, params, parens)
	{ /* Intentionally left blank */ }
	

	// inherited stuffs
	virtual std::ostream& translate(std::ostream& out, Context& context) const;
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const;
	inline virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const		
		{ return baseName() == "true" || baseName() == "false"; }
    inline virtual bool isSingleAtom(bool allowAtomicNegation = false) const           
		{ return false; }
    virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const	
														{ return (ref() && ((Object const*)ref())->isLua()) || (includeParams && hasLuaCallParameters()); }
	inline virtual IPart determineQueryIPart() const 	{ return IPART_BASE; }
	virtual ParseElement* copy() const;

	virtual bool isNumeric() const;
	inline virtual bool isBoolean() const 					{ return (ref() && ref()->isBoolean() ) || baseName() == "true" || baseName() == "false"; }

	/**
	 * Destructor. Does not deallocate anything.
	 */
	inline virtual ~ObjectLikeElement() { /* Intentionally Left Blank */ }
};


/**
 * Child of BaseLikeElement that represents a base element with the behavior of a variable.
 */
class VariableLikeElement : public BaseLikeElement
{
public:
	/**
	 * Full constructor.
	 * @param baseName The base name of the element that's being instantiated (without parameters).
	 * @param symbolRef A pointer to the symbol that this element is an instance of.
	 * @param parens Whether to encase the element in parens.
	 */
	VariableLikeElement(std::string const& baseName, Variable const* symbolRef, bool parens = false)
		: BaseLikeElement(PELEM_VARLIKE, baseName, symbolRef, NULL, parens)
	{ /* Intentionally left blank */ }
	
	// inherited stuffs
	virtual std::ostream& translate(std::ostream& out, Context& context) const;
	virtual bool hasConstants(unsigned int types, bool includeParams = true, bool includeEq = true) const;
	inline virtual bool isDefinite(bool allowComparison = false, bool allowChoice = false, bool allowAtomicNegation = false) const			
		{ return ref() && isConstantVariable() && ref()->isBoolean(); }
    inline virtual bool isSingleAtom(bool allowAtomicNegation = false) const                                                                
		{ return ref() && isConstantVariable() && ref()->isBoolean(); }
	inline virtual bool hasLuaCalls(bool includeParams = true, bool includeEq = true) const	
																{ return false; }
	inline virtual IPart determineQueryIPart() const 			{ return IPART_BASE; }
	inline virtual ParseElement* copy() const					{ return new VariableLikeElement(baseName(), (Variable const*)ref(), parens()); }

	inline virtual bool isNumeric() const							{ return ref() && ref()->isNumeric(); }
	inline virtual bool isBoolean() const							{ return ref() && ref()->isBoolean(); }

	/// Gets the domain of the variable ( or null )
	inline Sort const* domain() const							{ return (ref()) ? ((Variable const*)ref())->domain() : NULL; }

	/**
	 * Checks if a variable-like element represents a constant of some kind.
	 * @return True if the element is a constant variable, false otherwise.
	 */
	inline bool isConstantVariable() const					{ return ref() && ((Variable const*)ref())->isConstantVariable(); }

	/**
	 * Destructor. Does not deallocate anything.
	 */
	inline virtual ~VariableLikeElement() { /* Intentionally Left Blank */ }

protected:

	/**
	 * Helper translator method to handle translating the variable as an atom.
	 * @param[out] out - The output stream to write the translation to.
	 * preceeding @ if a symbol is understood as a lua function call.
	 * @param context -  The formula context to be used for translation.
	 * Uses constant translating conventions for the base name.
	 * @return A string containing the translated form of the element.
	 */
	virtual std::ostream& translateAsConstant(std::ostream& out, Context& context) const;

	/**
	 * Helper translator method to handle translating the variable as a variable.
	 * @param[out] out - The output stream to write the translation to.
	 * preceeding @ if a symbol is understood as a lua function call.
	 * @param context -  The formula context to be used for translation.
	 * Uses constant translating conventions for the base name.
	 * @return A string containing the translated form of the element.
	 */
	virtual std::ostream& translateAsVariable(std::ostream& out, Context& context) const;
};

#endif // PARSER_TYPES_H
