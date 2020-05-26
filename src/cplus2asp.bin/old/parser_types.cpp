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


#include <sstream>
#include <string>
#include <list>
#include <utility>

#include "DataStructs/Sort.h"
#include "DataStructs/Variable.h"
#include "utilities.h"
#include "DataStructs/Context.h"
#include "parser_types.h"
#include "Translator.h"
#include "parser.h"

extern std::ostringstream ltsyyossErr;

/*******************************************************************/
/* PT_constant_binder_t */
/*******************************************************************/

// Default constructor. Initializes attributes to NULL.
PT_constant_binder_t::PT_constant_binder_t()
{
	constType = Constant::CONST_UNKNOWN;
	domain = NULL;
	parent = NULL;
}

// Destructor. Resets all to NULL.
PT_constant_binder_t::~PT_constant_binder_t()
{
	constType = Constant::CONST_UNKNOWN;
	domain = NULL;
	parent = NULL;
}

/*******************************************************************/
/* ParseElement */
/*******************************************************************/

size_t ParseElement::sVarCount = 0;

/*******************************************************************/
/* SimpleUnaryOperator */
/*******************************************************************/

// Translator method.
std::ostream& SimpleUnaryOperator::translate(std::ostream& out, Context& context) const
{
	Context localContext;

	out << translateBeforeParen();

	// Sanity check: is the argument to the operator not NULL?
	if(!postOp()) {
		// TODO: Throw warning
		return out;
	}

	// Then, perform different translations based on what the operator is.
	switch(opType())
	{

	case UOP_STRONG_NOT: // Logical Strong negation "~"

		// Only applicable to bare booleans.
		if(postOp()->getType() == PELEM_CONSTLIKE && !postOp()->hasConstants(MASK_AB)) {

			// some validation, make sure it's boolean
			if (!((ConstantLikeElement*)postOp())->isBoolean()) {
				// Not a boolean constant, but still used in bare "~c" style, this doesn't make sense.
				ltsyystartParseError();
				ltsyyossErr << "Can't use \"~\" to negate non-Boolean constant \"";
				postOp()->fullName(ltsyyossErr);
				ltsyyossErr	<< "\".";
				ltsyyreportError();
				return out;
			}

			// output the constant with value false
			localContext = context.mkValue(Context::FALSE_STR);
			postOp()->translate(out, localContext);

		} else if(postOp()->getType() == PELEM_VARLIKE && ((VariableLikeElement*)postOp())->isConstantVariable()) {

			// This variable actually stands for a constant of some kind, treat this as "~constant".
			// Throw up a warning, as using "~V" with V being a constant is not guaranteed to work.
			VariableLikeElement* varPostOp = (VariableLikeElement*)postOp();

			ltsyystartWarning();
			ltsyyossErr << "Using \"~\" on variable \"";
			varPostOp->fullName(ltsyyossErr);
			ltsyyossErr	<< "\" representing constant type \"";
			ltsyyossErr << ((Variable const*)varPostOp->ref())->domain()->fullName();
			ltsyyossErr	<< "\" is not a good idea, and is almost guaranteed to not work as expected unless you know exactly what you're doing.";
			ltsyyreportWarning();

			localContext = context.mkValue(Context::FALSE_STR);
			varPostOp->translate(out, localContext);


		} else {
			// Not a bare constant,
			ltsyystartParseError();
			ltsyyossErr << "Invalid use of \"~\" preceeding \"";
			postOp()->fullName(ltsyyossErr);
			ltsyyossErr << "\". \"~\" must immediately preceede a bare boolean constant.";
			ltsyyreportError();
			return out;

		}

		break;

	case UOP_NOT: // Logical negation.

		// Negation works differently for bare booleans within the scope of negation (or if this is C+), check for them.
		if ((context.getLang() == LANG_CPLUS || context.getNegated()) && postOp()->getType() == PELEM_CONSTLIKE && !postOp()->hasConstants(MASK_AB)) {

			// some validation, make sure it's boolean
			if (!((ConstantLikeElement*)postOp())->isBoolean()) {
				// Not a boolean constant, but still used in bare "not c" style, this doesn't make sense.
				ltsyystartParseError();
				ltsyyossErr << "Can't use \"not\" to negate non-Boolean constant \"";
				postOp()->fullName(ltsyyossErr);
				ltsyyossErr	<< "\" without specifying which value is being negated. Try something like \"";
				postOp()->fullName(ltsyyossErr);
				ltsyyossErr << "=some_value\".";
				ltsyyreportError();
				return out;
			}

			// output the constant with value false
			localContext = context.mkValue(Context::FALSE_STR);
			postOp()->translate(out, localContext);

		} else {

			// Just a normal negation.

			// Sanity check, make sure that we're not in the head of a rule before capturing clauses../
			// Make sure we capture the clauses locally...
			if (context.getPos() != Context::POS_HEAD && context.getPos() != Context::POS_CHOICE_RULE) {

				out << translateOpType(opType()) + "(";
				Translator::bindAndTranslate(out, postOp(), context, false);
				out << ")";

			} else {

				// crap, we are. Just pass em up and hope for the best.
				// TODO

				localContext = context.mkNegated();
				out << translateOpType(opType()) << "(";
				postOp()->translate(out, localContext);
				out << ")";

			}
		}
		break;


	case UOP_NEGATIVE: // Numeric negative.
	case UOP_ABS: 	// Absolute value.

		localContext = context.mkPos(Context::POS_ARITHEXPR);
		out << translateOpType(opType()) << "(";
		postOp()->translate(out, localContext);
		out << ")";
		break;

	case UOP_EXOGENOUS: // Exogenous declaration.
	case UOP_INERTIAL: // Inertial declaration.
	case UOP_RIGID: // Rigid declaration.

		// All three "operators" do the same thing, it's just a matter of what name to use.
		if(postOp()->getType() == PELEM_CONSTLIKE)
		{
			localContext = context.mkPos(Context::POS_INTERNAL);
			out << translateOpType(opType()) << "(";
			postOp()->translate(out, localContext);
			out << ")";
		}
		else
		{
			ltsyystartParseError();
			ltsyyossErr << "Declaring \"";
			postOp()->fullName(ltsyyossErr);
			ltsyyossErr << "\" as " << translateOpType(opType()) << " is not supported.";
			ltsyyreportError();
		}
		break;
                
        case UOP_CHOICE:
            
                localContext = context.mkPos(Context::POS_CHOICE_RULE);
		out << "{";
		postOp()->translate(out, localContext);
		out << "}";
                break;
                
	default:
		// Unknown operator, just do a blind translation.
		out << translateOpType(opType()) << "(";
		postOp()->translate(out, context);
		out << ")";
		break;
	}

	out << translateAfterParen();
	return out;
}

bool SimpleUnaryOperator::hasConstants(unsigned int types, bool includeParams, bool includeEq) const {
	if (postOp()) {
		if ((types & MASK_RIGID))
			return  opType() == UOP_EXOGENOUS
						|| opType() == UOP_INERTIAL
						|| opType() == UOP_RIGID
						|| postOp()->hasConstants(types, includeParams, includeEq);
		else
			return  opType() != UOP_EXOGENOUS
						&& opType() != UOP_INERTIAL
						&& opType() != UOP_RIGID
						&& postOp()->hasConstants(types, includeParams, includeEq);
	} else return false;

}


// Returns true if this expression corresponds to a single atom.
bool SimpleUnaryOperator::isDefinite(bool allowComparison, bool allowChoice, bool allowAtomicNegation) const
{
	// Three cases:
	// the expression is '~p' as this is shorthand for p=false.
	// or this is a declarative law such as exogenous p. where p is a constant.
	// Alternatively, in the event we are working with a declarative law it may be one of UOP_EXOGENOUS, UOP_INERTIAL, UOP_RIGID
        // Finally, If we are allowing choice rules, then we should check if this is a choice rule and if below us is a single assignment
        // or boolean atom.
    
	// sanity
	if (!postOp()) return false;

	// case 1
	if (isSingleAtom(allowAtomicNegation)) return true;

	// case 2
	if (opType() == UOP_INERTIAL || opType() == UOP_RIGID || opType() == UOP_EXOGENOUS) {
		return postOp()->getType() == ParseElement::PELEM_CONSTLIKE
				|| (postOp()->getType() == ParseElement::PELEM_VARLIKE);
	}
        
	// case three
	if (allowChoice && opType() == UOP_CHOICE) {
		return (postOp()->isSingleAtom(allowAtomicNegation));
	}
        

	return false;
}

// Checks if the expression is a single atom.
bool SimpleUnaryOperator::isSingleAtom(bool allowAtomicNegation) const
{
	if (opType() == UOP_STRONG_NOT || (allowAtomicNegation && opType() == UOP_NOT)) {
		return (postOp()->getType() == ParseElement::PELEM_CONSTLIKE || postOp()->getType() == ParseElement::PELEM_VARLIKE)
				&& (((BaseLikeElement*)postOp())->ref()->isBoolean());
	}
	return false;
}

// Determines if the element is a valid arithmetic expression.
bool SimpleUnaryOperator::isArithExpr() const {
	if (!postOp()) return false;
	else {
		switch (opType()) {
		case UOP_NEGATIVE:
		case UOP_ABS:
			return postOp()->isArithExpr();
		case UOP_UNKNOWN:
		case UOP_NOT:
		case UOP_STRONG_NOT:
		case UOP_EXOGENOUS:
		case UOP_INERTIAL:
		case UOP_RIGID:
		default:
			return false;
		}
	}
}

// Outputs the original string representation of the element.
std::ostream& SimpleUnaryOperator::fullName(std::ostream& out) const
{
	if(postOp())
	{
		// Create a CCalc-compatible representation of the operator first.
		switch(opType())
		{
		case UOP_STRONG_NOT:
			out << "~";
			break;
		case UOP_NOT:
			out << "not ";
			break;
		case UOP_NEGATIVE:
			out << "-";
			break;
		case UOP_ABS:
			out << "abs ";
			break;
		case UOP_EXOGENOUS:
			out << "exogenous ";
			break;
		case UOP_INERTIAL:
			out << "inertial ";
			break;
		case UOP_RIGID:
			out << "rigid ";
			break;
		default:
			// Unknown operator, this should never happen
			out << " UNKNOWN ";
			break;
		}
		postOp()->fullName(out);
	}
	return out;
}

// Detaches the operator's sub expression, returning it.
ParseElement* SimpleUnaryOperator::detachPostOp() {
	ParseElement* ret = mPostOp;
	mPostOp = NULL;
	return ret;
}


std::string SimpleUnaryOperator::translateOpType(UnaryOperatorType op) {
	switch(op)
	{
	case UOP_STRONG_NOT:
		return "~";
	case UOP_NOT:
		return "not ";
	case UOP_NEGATIVE:
		return "-";
	case UOP_ABS:
		return "#abs";
	case UOP_EXOGENOUS:
		return "exogenous";
	case UOP_INERTIAL:
		return "inertial";
	case UOP_RIGID:
		return "rigid";
	default:
		return " UNKNOWN ";
	}
}

// Destructor.
SimpleUnaryOperator::~SimpleUnaryOperator()
{
	if(mPostOp) delete mPostOp;
}



/*******************************************************************/
/* SimpleBinaryOperator */
/*******************************************************************/


// Translator method.
std::ostream& SimpleBinaryOperator::translate(std::ostream& out, Context& context, BinaryOperatorType type, bool forceParens) const
{
	Context localContext;
	std::ostringstream tmp;
	std::string tmpstr;
	SimpleBinaryOperator* tmpOp;

	out << translateBeforeParen(forceParens);

	if (!preOp() && !postOp()) {
		// TODO: Throw error
		return out;
	} else if(!preOp()) {
		// If just preOp is NULL, pretend there's just postOp and translate it.
		postOp()->translate(out, context);
	} else if(!postOp()) {
		// If just postOp is NULL, pretend there's just preOp and translate it.
		preOp()->translate(out, context);
	}
	else
	{ 	// Then, perform different translations based on what the operator is.
		// Lack of breaks is intentional.
		switch(type) {

		case BOP_BIND_STEP:
			// This is an expression of the form X:F which binds F to have maximum step value X.

			localContext = context.mkPos(Context::POS_ARITHEXPR);
			preOp()->translate(tmp, localContext);
			tmpstr = tmp.str();

			localContext = context.mkTime(tmpstr);
			Translator::bindAndTranslate(out, postOp(), localContext, false);
			break;

		/* This special case isn't actually neccessary.
		case BOP_NEQ:

			// Special case. X\=Y <-> not (X==Y)
			out << "not (";

			tmpOp = new SimpleBinaryOperatorWrapper(this, BOP_EQ);
			localContext = context.mkNegated();
			Translator::bindAndTranslate(out, tmpOp, localContext, false);
			delete tmpOp;
			out << ")";
			break;
		*/
		case BOP_EQ:

			if (isAtomicEq()) {

				// special case, direct assignment to a constant...
				// just translate the constant with the appropriate value.
				localContext = context.mkPos(Context::POS_TERM);
				postOp()->translate(tmp, localContext);
				std::string tmpstr = tmp.str();
				localContext = context.mkValue(tmpstr);
				preOp()->translate(out, localContext);
				break;
			} else if (preOp()->getType() == PELEM_CONSTLIKE && postOp()->getType() == PELEM_CONSTLIKE) {

				// special case, equality between two constants.
				// c=d is translated to (exists x)(c(x) & d(x))

				// Setup the new variable
				std::string tmpvar = getNewVar();

				Sort const* domain = (((ConstantLikeElement const*)preOp())->domain()) ?
						((ConstantLikeElement const*)preOp())->domain()
						: ((ConstantLikeElement const*)postOp())->domain();

				context.addFreeVariable(tmpvar, domain);
				localContext = context.mkValue(tmpvar);

				// Make sure the constants are equal.
				preOp()->translate(out, localContext);
				out << " & ";
				postOp()->translate(out, localContext);
				break;
			}
			/* no break */
		case BOP_DBL_EQ:
		case BOP_NEQ:
		case BOP_PLUS:
		case BOP_MINUS:
		case BOP_TIMES:
		case BOP_DIVIDE:
		case BOP_MOD:
		case BOP_LTHAN:
		case BOP_GTHAN:
		case BOP_LTHAN_EQ:
		case BOP_GTHAN_EQ:
			// arithmetic expressions...
			if (context.getPos() == Context::POS_ARITHEXPR 
				|| context.getPos() == Context::POS_TERM 
				|| context.getPos() == Context::POS_MAXIMIZED_INTERNAL
				|| context.getPos() == Context::POS_CHOICE_RULE ) {
				// Already inside an arithmetic expression.
				// pass all the variables up to the highest arith-expr level.
				preOp()->translate(out, context);
				out << translateOpType(type);
				postOp()->translate(out, context);

			} else {

				// Entering an arithmetic expression
				// Bind all of the local variables as we may need existential quantifiers.
				localContext = context.mkPos(Context::POS_ARITHEXPR);
				Translator::bindAndTranslate(out, this, localContext, true);

			}
			break;

		case BOP_EQUIV:
			// Special case: F <-> G
			// F2LP can't handle equivalence in general.
			// Translate to (F -> G) & (G -> F)

			// Setup the wrapper expression
			tmpOp = new SimpleBinaryOperator(
					new SimpleBinaryOperatorWrapper(this, BOP_IMPL, true),
					BOP_AND,
					new SimpleBinaryOperatorWrapper(this, BOP_REV_IMPL, true)
			);

			// Translate
			tmpOp->translate(out, context);

			// Tear down
			delete tmpOp;
			break;

		case BOP_REV_IMPL:
			// Special case: F <- G.
			// Translate as G -> F.
			Translator::bindAndTranslate(out, postOp(), context, false);
			out << translateOpType(BOP_IMPL);
			Translator::bindAndTranslate(out, preOp(), context, false);
			break;

		default:
			// Logical operations...
			// If this isn't conjunction, then we want to capture local clauses for each term.
			// otherwise we'll just pass the clauses upward.

			if (type != BOP_AND && context.getPos() != Context::POS_HEAD && context.getPos() != Context::POS_CHOICE_RULE) {

				// preop
				Translator::bindAndTranslate(out, preOp(), context, false);

				// operand
				out << translateOpType(type);

				// postOp
				Translator::bindAndTranslate(out, postOp(), context, false);

			} else {
				// conjunction (hopefully), just pass the clauses upward.
				preOp()->translate(out, context);
				out << translateOpType(type);
				postOp()->translate(out, context);
			}
			break;
		}
	}

	out << translateAfterParen(forceParens);
	return out;
}

bool SimpleBinaryOperator::hasConstants(unsigned int types, bool includeParams, bool includeEq) const {
	bool ret = false;

	if (preOp()) {
		ret = preOp()->hasConstants(types, includeParams, includeEq);

		if (!ret) {

			if (isAtomicEq()) {

				// Atomic Equality
				ret = includeEq && postOp() && postOp()->hasConstants(types, includeParams, includeEq);

			} else {
				// not atomic equality.
				ret = postOp() && postOp()->hasConstants(types, includeParams, includeEq);
			}
		}
	} else if (postOp()) {
		ret = postOp() && postOp()->hasConstants(types, includeParams, includeEq);
	}
	return ret;
}

bool SimpleBinaryOperator::isDefinite(bool allowComparison, bool allowChoice, bool allowAtomicNegation) const {
	// The only time when this is true is if one of the children is NULL and the other is a single atom OR if it's of the form c=v
	// OR if it's a conjunction of single atoms.
	if (!preOp() && postOp()) return postOp()->isDefinite(allowComparison, allowChoice, allowAtomicNegation);
	else if (!postOp() && preOp()) return preOp()->isDefinite(allowComparison, allowChoice, allowAtomicNegation);
	else if (opType() == BOP_AND) {
		return preOp()->isDefinite(allowComparison, allowChoice, allowAtomicNegation) && postOp()->isDefinite(allowComparison, allowChoice, allowAtomicNegation);
	} else if (opType() == BOP_EQ) {
		return ((preOp()->getType() == ParseElement::PELEM_CONSTLIKE) || !preOp()->hasConstants(MASK_NON_TRIVIAL, false, true))
				&& !postOp()->hasConstants(MASK_NON_TRIVIAL, false, true);
	} else if (allowComparison 
			&& ( opType() == BOP_DBL_EQ || opType() == BOP_NEQ || opType() == BOP_LTHAN 
				|| opType() == BOP_GTHAN || opType() == BOP_LTHAN_EQ || opType() == BOP_GTHAN_EQ)) {

		// Comparison-like operator. Make sure that we aren't comparing two constants.
		return !postOp()->hasConstants(MASK_NON_TRIVIAL, false, true) 
			|| !preOp()->hasConstants(MASK_NON_TRIVIAL, false, true);

	} else return false;
}

// Returns true if this expression corresponds to a single atom.
bool SimpleBinaryOperator::isSingleAtom(bool allowAtomicNegation) const {
	// The only time when this is true is if one of the children is NULL and the other is a single atom OR if it's of the form c=v
	if (!preOp() && postOp()) return postOp()->isSingleAtom(allowAtomicNegation);
	else if (!postOp() && preOp()) return preOp()->isSingleAtom(allowAtomicNegation);
	else if (opType() == BOP_EQ) {
		return (preOp()->getType() == ParseElement::PELEM_CONSTLIKE)
				&& !postOp()->hasConstants(MASK_NON_TRIVIAL, false, true);
	} else return false;
}


bool SimpleBinaryOperator::hasLuaCalls(bool includeParams, bool includeEq) const { 
	bool ret = false;
	if (preOp()) {
		ret = preOp()->hasLuaCalls(includeParams, includeEq);

		if (postOp() && !ret) {

			if (isAtomicEq()) {
				
				// This is equality within an atom.
				ret = includeEq && postOp()->hasLuaCalls(includeParams, includeEq);
			} else {
				// Not atomic equality
				ret = postOp()->hasLuaCalls(includeParams, includeEq);
			}
		}
	}
	else if (postOp()) {
		ret = postOp()->hasLuaCalls(includeParams, includeEq);
	}

	return ret;
}

IPart SimpleBinaryOperator::determineQueryIPart() const {
	std::ostringstream tmp;
	IPart ret1, ret2;

	if (opType() == BOP_BIND_STEP) {
		// This is a bind statement. Figure out what we're doing.
		preOp()->fullName(tmp);
		if (tmp.str() == "0") {
			return IPART_BASE;
		} else {
			return IPART_VOLATILE;
		}
	} else {
		ret1 = (preOp()) ? preOp()->determineQueryIPart() : IPART_BASE;
		ret2 = (postOp()) ? postOp()->determineQueryIPart() : IPART_BASE;

		if (ret1 == IPART_CUMULATIVE || ret2 == IPART_CUMULATIVE) return IPART_CUMULATIVE;
		else if (ret1 == IPART_VOLATILE || ret2 == IPART_VOLATILE) return IPART_VOLATILE;
		else return IPART_BASE;
	}

}


// Determines if the element is a valid arithmetic expression.
bool SimpleBinaryOperator::isArithExpr() const {
	if (!postOp() && !preOp()) return false;
	else if (!preOp()) return postOp()->isArithExpr();
	else if (!postOp()) return preOp()->isArithExpr();
	else {
		switch (opType()) {
		case BOP_PLUS:
		case BOP_MINUS:
		case BOP_TIMES:
		case BOP_DIVIDE:
		case BOP_MOD:
			return postOp()->isArithExpr() && preOp()->isArithExpr();
		case BOP_UNKNOWN:
		case BOP_AND:
		case BOP_OR:
		case BOP_EQUIV:
		case BOP_IMPL:
		case BOP_EQ:
		case BOP_NEQ:
		case BOP_DBL_EQ:
		case BOP_LTHAN:
		case BOP_GTHAN:
		case BOP_LTHAN_EQ:
		case BOP_GTHAN_EQ:
		default:
			return false;
		}
	}
}


// Returns an original string representation of this operator and its operands.
std::ostream& SimpleBinaryOperator::fullName(std::ostream& out) const
{
	if(preOp() && postOp()) {
		preOp()->fullName(out);
		out << translateOpType(opType());
		postOp()->fullName(out);
	}
	else if(!preOp() && postOp()) {
		postOp()->fullName(out);
	}
	else if(!postOp() && preOp()) {
		preOp()->fullName(out);
	}
	return out;
}

void SimpleBinaryOperator::aggregateUndefined(BaseElementList& outIdentifiers)	{
		if (preOp()) mPreOp->aggregateUndefined(outIdentifiers);
		if (postOp()) mPostOp->aggregateUndefined(outIdentifiers);
}

// Detaches the operator's sub expression, returning it.
ParseElement* SimpleBinaryOperator::detachPostOp() {
	ParseElement* ret = mPostOp;
	mPostOp = NULL;
	return ret;
}

// Detaches the operator's sub expression, returning it.
ParseElement* SimpleBinaryOperator::detachPreOp() {
	ParseElement* ret = mPreOp;
	mPreOp = NULL;
	return ret;
}

// Converts an operatory to a string representation.
std::string SimpleBinaryOperator::translateOpType(BinaryOperatorType op) {
	std::string opStr;
	switch(op)
	{
	case BOP_PLUS:
		opStr = " + ";
		break;
	case BOP_MINUS:
		opStr = " - ";
		break;
	case BOP_TIMES:
		opStr = " * ";
		break;
	case BOP_DIVIDE:
		opStr = " / ";
		break;
	case BOP_MOD:
		opStr = " \\ ";
		break;
	case BOP_AND:
		opStr = " & ";
		break;
	case BOP_OR:
		opStr = " | ";
		break;
	case BOP_EQUIV:
		opStr = " <--> ";
		break;
	case BOP_IMPL:
		opStr = " -> ";
		break;
	case BOP_EQ:
		opStr = " == ";
		break;
	case BOP_NEQ:
		opStr = " != ";
		break;
	case BOP_DBL_EQ:
		opStr = " == ";
		break;
	case BOP_LTHAN:
		opStr = " < ";
		break;
	case BOP_GTHAN:
		opStr = " > ";
		break;
	case BOP_LTHAN_EQ:
		opStr = " <= ";
		break;
	case BOP_GTHAN_EQ:
		opStr = " >= ";
		break;
	default:
		// Unknown, this is a problem...
		opStr = " BAD_OP ";
		break;
	}
	return opStr;
}

// Destructor.
SimpleBinaryOperator::~SimpleBinaryOperator()
{
	if(mPreOp)
	{
		delete mPreOp;
		mPreOp = NULL;
	}
	if(mPostOp)
	{
		delete mPostOp;
		mPostOp = NULL;
	}
}

/*******************************************************************/
/* BigQuantifier */
/*******************************************************************/


// Translator method.
std::ostream& BigQuantifiers::translate(std::ostream& out, Context& context) const
{

	// The local context is used for variable list.
	// They _shouldn't_ be declaring free variables and clauses.
	Context localContext = context.mkPos(Context::POS_TERM).mkBindVars(NULL).mkBindClauses(NULL);
	out << translateBeforeParen();



	for(QuantifierList::const_iterator lIter = quantsBegin(); lIter != quantsEnd(); lIter++)
	{

		switch((*lIter)->first)
		{
		case QUANT_CONJ:
			out << "![";
			if((*lIter)->second)
				(*lIter)->second->translate(out, localContext);
			out << "]:(";
			break;
		case QUANT_DISJ:
			out << "?[";
			if((*lIter)->second)
				(*lIter)->second->translate(out, localContext);
			out << "]:(";
			break;
		default:
			// Unknown, just open a parenthesis.
			out << "(";
			break;
		}
		// Instead of having a bunch of linked lists lying around, we'll just push on a marker string for every nested quantifier.
	}

	// Next, translate whatever subformula was in the big quantifiers.
	if(postOp())
		Translator::bindAndTranslate(out, postOp(), context, false);

	// Nested parens
	for(QuantifierList::const_iterator lIter = quantsBegin(); lIter != quantsEnd(); lIter++) {
		out << ")";
	}

	out << translateAfterParen();

	return out;
}


bool BigQuantifiers::hasConstants(unsigned int types, bool includeParams, bool includeEq) const {
	return postOp() && postOp()->hasConstants(types, includeParams, includeEq);
}

// Returns an original string representation of the quantifiers and what's getting quantified.
std::ostream& BigQuantifiers::fullName(std::ostream& out) const
{
	if(postOp() && numQuants())
	{
		out << "[ ";
		for(QuantifierList::const_iterator lIter = quantsBegin(); lIter != quantsEnd(); lIter++)
		{
			if((*lIter) && (*lIter)->second)
			{
				switch((*lIter)->first)
				{
				case QUANT_CONJ:
					out << "/\\ ";
					break;
				case QUANT_DISJ:
					out << "\\/ ";
					break;
				default:
					break;
				}
				(*lIter)->second->fullName(out);
			}
		}
		out << " | ";
		postOp()->fullName(out);
		out << " ]";
	}
	return out;
}

ParseElement* BigQuantifiers::copy() const {
	BigQuantifiers* res = new BigQuantifiers(NULL, NULL, parens());
	res->postOp((postOp()) ? postOp()->copy() : NULL);
	for (QuantifierList::const_iterator it = quantsBegin(); it != quantsEnd(); it++) {
		if ((*it)->second)
			res->addQuant(new Quantifier((*it)->first, (*it)->second->copy()));
	}
	return res;
}

void BigQuantifiers::aggregateUndefined(BaseElementList& outIdentifiers)	{
	for (QuantifierList::const_iterator it = quantsBegin(); it != quantsEnd(); it++) {
		if ((*it)->second) {
			(*it)->second->aggregateUndefined(outIdentifiers);
		}
	}
	if (postOp()) mPostOp->aggregateUndefined(outIdentifiers);
}

// Detaches the operator's sub expression, returning it.
ParseElement* BigQuantifiers::detachPostOp() {
	ParseElement* ret = mPostOp;
	mPostOp = NULL;
	return ret;
}

// Destructor.
BigQuantifiers::~BigQuantifiers()
{
	for(QuantifierList::iterator lIter = quantsBegin(); lIter != quantsEnd(); lIter++)
	{
		if((*lIter)->second)
		{
			delete (*lIter)->second;
		}
		delete (*lIter);
	}
	if(mPostOp)
		delete mPostOp;
}


/*******************************************************************/
/* BaseLikeElement */
/*******************************************************************/

// Guesses the sorts associated with this element.
bool BaseLikeElement::guessParamSorts(ConstSortList& outSorts) {
	bool foundAll = true;
	outSorts.clear();

	for (ParseElementList::const_iterator it = paramsBegin(); it != paramsEnd(); it++) {
		// Check if it's a variable, if it is then use it's sort.
		if ((*it)->getType() == PELEM_VARLIKE) {
			outSorts.push_back(((VariableLikeElement const*)*it)->domain());
		} else {
			// nope, it's not.
			outSorts.push_back(NULL);
			foundAll = false;
		}
	}

	return foundAll;
}



// Translator helper method for params.
std::ostream& BaseLikeElement::translateParams(std::ostream& out, Context& context, bool force) const
{
	Context localContext;

	if (context.getPos() != Context::POS_INTERNAL 
			&& context.getPos() != Context::POS_TERM
			&& context.getPos() != Context::POS_MAXIMIZED_INTERNAL)
		localContext = context.mkPos(Context::POS_TERM);
	else
		localContext = context;

	int i = 0;
	if(arity() || force) {
		out <<  "(";
		for(ParseElementList::const_iterator vIter = paramsBegin(); vIter != paramsEnd(); vIter++)
		{	
			if (*vIter) {
				if(i++ > 0) out << ",";
				(*vIter)->translate(out, localContext);
			}
		}
		out << ")";
	}
	return out;
}

// Outputs the original full name of the element, including parameters.
std::ostream& BaseLikeElement::fullName(std::ostream& out) const
{
	out << baseName();
	if(arity()) {
		out << "(";
		int i = 0;
		for(ParseElementList::const_iterator vIter = paramsBegin(); vIter != paramsEnd(); vIter++) {
			if(*vIter)
			{
				if(i++ > 0)	out <<",";
				(*vIter)->fullName(out);
			}
		}
		out << ")";
	}
	return out;
}

// Determines if the base element has a LUA call in its parameter list.
bool BaseLikeElement::hasLuaCallParameters() const 
{

	for(ParseElementList::const_iterator vIter = paramsBegin(); vIter != paramsEnd(); vIter++) {
		if (*vIter && (*vIter)->hasLuaCalls()) return true;
	}
	return false;
}

// Checks to see if there is a nested constant matching the type mask within the parameter list.
bool BaseLikeElement::hasConstantParameters(unsigned int types) const
{

	for(ParseElementList::const_iterator vIter = paramsBegin(); vIter != paramsEnd(); vIter++) {
		if (*vIter && (*vIter)->hasConstants(types, true)) return true;
	}
	return false;

}


// Destructor.
BaseLikeElement::~BaseLikeElement()
{
	for(ParseElementList::iterator vIter = paramsBegin(); vIter != paramsEnd(); vIter++) {
		if(*vIter) delete *vIter;
	}
}

/*******************************************************************/
/* ConstantLikeElement */
/*******************************************************************/

ParseElement* ConstantLikeElement::copy() const {
	ParseElementList params;

	for (ParseElementList::const_iterator it = paramsBegin(); it != paramsEnd(); it++) {
		params.push_back((*it)->copy());
	}

	return new ConstantLikeElement(baseName(), (Constant const*)ref(), &params, parens());
}


// Translator method.
std::ostream& ConstantLikeElement::translate(std::ostream& out, Context& context) const
{
	// This tries it's best to output a wrapped atom (i.e. h(eql(<name>(<args>),<value>),<timestep>)).
	// If, for one reason or another, the constant's type isn't available, it'll wing it producing a bare atom <name>(<args>).
	switch (context.getPos()) {
	case Context::POS_INTERNAL:
		// Used for internal sorts and whatnot, just return the bare name.
		out << (ref() ? ref()->baseTransName() : Translator::sanitizeConstantName(baseName()));
		translateParams(out, context);
		break;
	case Context::POS_HEAD:
	case Context::POS_BODY:
	case Context::POS_QUERY:
	case Context::POS_CHOICE_RULE:
		translateAsConstant(out, context);
		break;
	case Context::POS_TERM:
	case Context::POS_ARITHEXPR:
	case Context::POS_MAXIMIZED_INTERNAL:
		translateAsVariable(out, context);
		break;
	}

	return out;
}

bool ConstantLikeElement::hasConstants(unsigned int types, bool includeParams, bool includeEq) const {
	bool ret = false;
	Constant::ConstantType type = (ref()) ? ((Constant const*)ref())->constType() : Constant::CONST_UNKNOWN;

	if (types & MASK_ACTION) 		ret |= Constant::isActionType(type);
	if (types & MASK_FLUENT) 		ret |= Constant::isFluentType(type, false);
	if (types & MASK_SDFLUENT)		ret |= type == Constant::CONST_SDFLUENT;
	if (types & MASK_RIGID)  		ret |= type == Constant::CONST_RIGID || type == Constant::CONST_UNKNOWN;
	if (types & MASK_STATIC_AB)		ret |= type == Constant::CONST_STATICAB;
	if (types & MASK_DYNAMIC_AB)	ret |= type == Constant::CONST_DYNAMICAB;

	if (!ret && includeParams) {
		// check the parameters for constants.
		// If this is a contribution constant we should skip the first layer.
		if (isContribConstant()) {
			for(ParseElementList::const_iterator vIter = paramsBegin(); vIter != paramsEnd(); vIter++) {
				if (((ConstantLikeElement const*)(*vIter))->hasConstantParameters(types)) {
					ret = true;
					break;
				}
			}
		} else {
			ret = hasConstantParameters(types);
		}
	}


	return ret;
}


// Translator helper
std::ostream& ConstantLikeElement::translateAsConstant(std::ostream& out, Context& context) const {
	// We are in a logical formula, translate it as an atom
	out <<	translateBeforeParen()
				<< translatePrefix()
				<< (ref() ? ref()->baseTransName() : Translator::sanitizeConstantName(baseName()));

	Context localContext;

	if ( isContribConstant()) {
		localContext = context.mkPos(Context::POS_INTERNAL);
	} else {
		localContext = context;
	}

	translateParams(out, localContext);
	out << translatePostfix(context)
				<< translateAfterParen();
	return out;
}

// Translator helper
std::ostream& ConstantLikeElement::translateAsVariable(std::ostream& out, Context& context) const {
	Context localContext;
	std::ostringstream tmp;
	// We are in a term or arithmetic expression, translate it as a temporary variable.
	if (ref() && !((Constant const*)ref())->isAbnormal())
	{
		// The variable that holds the intermediate value (and register it!).
		std::string var = getNewVar();
		context.addFreeVariable(var, domain());

		// add the appropriate extra clause
		if (context.getPos() != Context::POS_MAXIMIZED_INTERNAL) {
			localContext = context.mkPos(Context::POS_BODY).mkValue(var);
			translate(tmp, localContext);
			context.addExtraClause(tmp.str());
		} else {
			context.addExtraClause(domain()->fullTransName() + "(" + var + ")");
		}

		// return the variable name
//		out << translateBeforeParen() + var + translateAfterParen();
		out << var;

	} else if (!ref()) {
		ltsyystartParseError();
		ltsyyossErr << "An error occurred while translating the constant ";
		fullName(ltsyyossErr);
		ltsyyossErr << ". The constant doesn't appear to be properly declared.";
		ltsyyreportError();

	} else {
		// This should never happen, as the syntax to trigger this is prohibited in the parser.
		ltsyystartParseError();
		ltsyyossErr << "An error occurred while translating an abnormality constant ";
		fullName(ltsyyossErr);
		ltsyyossErr << ". The constant appears to be used as a function.";
		ltsyyreportError();
	}

	return out;
}

/*******************************************************************/
/* ObjectLikeElement */
/*******************************************************************/

ParseElement* ObjectLikeElement::copy() const {
	ParseElementList params;

	for (ParseElementList::const_iterator it = paramsBegin(); it != paramsEnd(); it++) {
		params.push_back((*it)->copy());
	}

	return new ObjectLikeElement(baseName(), (Object const*)ref(), &params, parens());
}

// Translator method.
std::ostream& ObjectLikeElement::translate(std::ostream& out, Context& context) const
{

//	out << translateBeforeParen();

	if(ref())
	{	// Use the canonical translated object name if it's available.
		if(((Object const*)ref())->isLua()) out << "#spatom{";//need to make f2lp pass this through to ASP
		out << ref()->baseTransName();
	}
	else
	{	// Otherwise, just wing it with this instance's base name.
		out << Translator::sanitizeObjectName(baseName());
	}


	translateParams(out, context, ((Object const*)ref())->isLua());
//	out << translateAfterParen();


	if(((Object const*)ref())->isLua()) {
			out << "}";
			if (context.getPos() != Context::POS_ARITHEXPR
					&& context.getPos() != Context::POS_TERM
					&& context.getPos() != Context::POS_MAXIMIZED_INTERNAL)
				out << " == 1";
	}

	return out;
}

bool ObjectLikeElement::hasConstants(unsigned int types, bool includeParams, bool includeEq) const {
	return ((types & MASK_TRUE_FALSE) && arity() == 0 && (baseName() == "true" || baseName() == "false"))
		||  (includeParams && hasConstantParameters(types));
}

// Determines if the element is a valid arithmetic expression.
bool ObjectLikeElement::isNumeric() const {
	// This is only the case if the object is numeric.
	int tmp;
	std::stringstream fullname;
	fullName(fullname);

	return (ref() && ref()->isNumeric())
			|| (!ref() && utils::from_string(tmp,fullname.str()));
}


/*******************************************************************/
/* VariableLikeElement */
/*******************************************************************/

// Translator method. Ignores parameters, as variables shouldn't have any.
std::ostream& VariableLikeElement::translate(std::ostream& out, Context& context) const {

	switch (context.getPos()) {

	case Context::POS_HEAD:
	case Context::POS_BODY:
	case Context::POS_QUERY:
		// This occurs within the scope of a formula, we need to translate it as a constant.
		translateAsConstant(out, context);
		break;

	case Context::POS_TERM:
	case Context::POS_ARITHEXPR:
	case Context::POS_INTERNAL:
	case Context::POS_MAXIMIZED_INTERNAL:
		// Occurs as a term or within an arithmetic expression, translate as a variable.
		translateAsVariable(out, context);
		break;
	}

	return out;
}

bool VariableLikeElement::hasConstants(unsigned int types, bool includeParams, bool includeEq) const {
	return false;
}

std::ostream& VariableLikeElement::translateAsVariable(std::ostream& out, Context& context) const {
	out << translateBeforeParen();
	if(ref()) {
		// Use the canonical translated variable name if it's available.
		out << ref()->fullTransName();
	} else {
		// Otherwise, just wing it with this instance's base name.
		out << Translator::sanitizeVariableName(baseName());
	}
	out << translateAfterParen();

	return out;
}

// Translates the variable as though it were a constant.
std::ostream& VariableLikeElement::translateAsConstant(std::ostream& out, Context& context) const {

	out << translateBeforeParen();

	Constant::ConstantType type = ref() ?
			((Variable const*)ref())->getConstantType()
			: Constant::CONST_UNKNOWN;

	if (type == Constant::CONST_UNKNOWN) {
		ltsyystartWarning();
		ltsyyossErr << "In trying to treat variable \"";
		fullName(ltsyyossErr);
		ltsyyossErr << "\" like a constant, unable to determine the type of the constant. Using default behavior.";
		ltsyyreportWarning();
	}

	// start the predicate
	out << Constant::translatePrefix(type);

	if (ref()) {
		// Use the canonical translated variable name if it's available.
		out << ref()->fullTransName();
	} else {
		// Otherwise, just wing it with this instance's information.
		out << Translator::sanitizeVariableName(baseName());
	}

	// finish up the predicate
	out << Constant::translatePostfix(type, context);
	out << translateAfterParen();

	return out;
}
