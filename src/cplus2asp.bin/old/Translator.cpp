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


#include <list>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "types.h"

#include "DataStructs/Context.h"
#include "parser_types.h"
#include "DataStructs/Attribute.h"
#include "DataStructs/Constant.h"
#include "DataStructs/Element.h"
#include "DataStructs/Object.h"
#include "DataStructs/NumberRange.h"
#include "DataStructs/Sort.h"
#include "DataStructs/Variable.h"
#include "DataStructs/Query.h"
#include "parser.h"
#include "DataStructs/ElementCounter.h"
#include "DataStructs/Constant.h"

#include "DataStructs/utilities.h"

#include "parser_types.h"
#include "parser.h"

#include "Translator.h"

/// When converting a number range to a sort name, this is the prefix that will be attached.
#define NUMRANGE_TO_SORT_PREFIX "numRange"
/// The prefix to stick on constant names when they need to be sanitized.
#define SANITIZE_CONST_PREFIX "saniConst_"
/// The prefix to stick on object names when they need to be sanitized.
#define SANITIZE_OBJ_PREFIX "saniObj_"
/// The prefix to stick on sort names when they need to be sanitized.
#define SANITIZE_SORT_PREFIX "saniSort_"
/// The prefix to stick on variable names when they need to be sanitized.
#define SANITIZE_VAR_PREFIX "SaniVar_"
/// The prefix used to variable-ify sort names.
#define SORT_TO_VAR_PREFIX "V_"
/// What to use to replace "bad" characters in a string that needs to be sanitized.
#define SANITIZE_STR_REPLACE "x"

/* Static class methods */

// Santitizes a constant name so it's compatible with ASP naming conventions.
std::string Translator::sanitizeConstantName(std::string const& originalName)
{
	std::string tempStr = "";
	// If the name starts with an underscore, number, or upper-case letter,
	// prepend a sanitizing prefix, otherwise just use the original name.
	// Does not check originalName for "invalid" characters.
	if(originalName.length() > 0)
	{
		if(originalName[0] == '_' || utils::isDigit(originalName[0]) || utils::isUppercase(originalName[0]))
		{
			tempStr += SANITIZE_CONST_PREFIX;
			tempStr += originalName;
		}
		else
		{
			tempStr += originalName;
		}
	}
	return tempStr;
}

// Santitizes an object name so it's compatible with ASP naming conventions.
std::string Translator::sanitizeObjectName(std::string const& originalName)
{
	std::string tempStr = "";
	// If the name starts with an underscore or upper-case letter,
	// prepend a sanitizing prefix, otherwise just use the original name.
	// Checks for math symbols that need transforming.
	if(originalName.length() > 0)
	{
		if(originalName[0] == '_' || utils::isUppercase(originalName[0]))
		{
			tempStr += SANITIZE_OBJ_PREFIX;
			tempStr += originalName;
		}
		else
		{
			tempStr += originalName;
		}
		// Turn CCalc integer division into ASP integer division.
		size_t transPos = tempStr.find("//");
		while(transPos != std::string::npos)
		{
			tempStr.replace(transPos, 2, "/");
			transPos = tempStr.find("//");
		}
		// Turn CCalc "mod" into ASP modulus.
		transPos = tempStr.find("mod");
		while(transPos != std::string::npos)
		{
			tempStr.replace(transPos, 3, "\\");
			transPos = tempStr.find("mod");
		}
	}
	return tempStr;
}

// Santitizes a sort name so it's compatible with ASP naming conventions.
std::string Translator::sanitizeSortName(std::string const& originalName)
{
	std::string tempStr = "";
	// If the name starts with an underscore, number, or upper-case letter,
	// prepend a sanitizing prefix, otherwise just use the original name.
	// Checks original name for "*" and turns it into "_star".
	if(originalName.length() > 0)
	{
		if(originalName[0] == '_' || utils::isDigit(originalName[0]) || utils::isUppercase(originalName[0]))
		{
			tempStr += SANITIZE_SORT_PREFIX;
		}
		for(size_t i = 0; i < originalName.length(); i++)
		{
			if(originalName[i] == '*')
			{
				tempStr += "_star";
			}
			else
			{
				tempStr += originalName[i];
			}
		}
	}
	return tempStr;
}

// Santitizes a variable name so it's compatible with ASP naming conventions.
std::string Translator::sanitizeVariableName(std::string const& originalName)
{
	std::string tempStr = "";
	// If the name starts with an underscore, number, or lower-case letter,
	// prepend a sanitizing prefix, otherwise just use the original name.
	// Does not check originalName for "invalid" characters.
	if(originalName.length() > 0)
	{
		if(originalName[0] == '_' || utils::isDigit(originalName[0]) || utils::isLowercase(originalName[0]))
		{
			tempStr += SANITIZE_VAR_PREFIX;
			tempStr += originalName;
		}
		else
		{
			tempStr += originalName;
		}
	}
	return tempStr;
}

// Sanitizes an arbitrary string so it's compatible with ASP syntax for predicates.
std::string Translator::sanitizeString(std::string const& originalString)
{
	std::string tempStr = "";
	if(originalString.length() > 0)
	{
		// If the string starts with an underscore or upper-case letter,
		// prepend a sanitizing prefix.
		if(originalString[0] == '_' || utils::isUppercase(originalString[0]))
		{
			tempStr += SANITIZE_STR_REPLACE;
		}
		// If any characters in the string are not alphanumeric or underscores,
		// replace them with a sanitized character.
		for(size_t i = 0; i < originalString.length(); i++)
		{
			if(originalString[i] != '_' && !utils::isLetter(originalString[i]) && !utils::isDigit(originalString[i]))
			{
				tempStr += SANITIZE_STR_REPLACE;
			}
			else
			{
				tempStr += originalString[i];
			}
		}
	}
	return tempStr;
}

// Static version of translateVariableDecl that returns the result as a string instead of sending the result to an output stream.
std::string Translator::translateVariableDeclToString(Variable const* transVar)
{
	std::ostringstream retStream;
	// Sanity check: make sure the variable isn't NULL and neither is its sort.
	if(transVar && transVar->domain())
	{
		// Variable declarations are just domain variable declarations.
		retStream << "#domain " << transVar->domain()->fullTransName() << "(";
		retStream << transVar->fullTransName() << ").";
	}
	return retStream.str();
}

// Returns a standardized sort name representing a range of numbers.
std::string Translator::numRangeToSortName(std::string const& min, std::string const& max)
{
	std::string tempStr = NUMRANGE_TO_SORT_PREFIX;
	tempStr += Context::ANON_STR;
	tempStr += Translator::sanitizeString(min);
	tempStr += Context::ANON_STR;
	tempStr += Translator::sanitizeString(max);
	return tempStr;
}

// Turns a sort name into a variable version of itself.
std::string Translator::sortNameToVariable(std::string const& sortName, bool sanitizeFirst)
{
	std::string tempStr = "";
	std::string baseName = "";
	if(sanitizeFirst)
	{
		baseName = Translator::sanitizeSortName(sortName);
	}
	else
	{
		baseName = sortName;
	}
	// Prepend a variable-compatible prefix to the sort name, then call
	// the variable name sanitizer on the result to ensure it's a good name.
	if(baseName.length() > 0)
	{
		tempStr += SORT_TO_VAR_PREFIX;
		tempStr += baseName;
		tempStr = Translator::sanitizeVariableName(tempStr);
	}
	return tempStr;
}

// Merges two sub-formulas together into one formula.
ParseElement* Translator::mergeSubFormulas(ParseElement* first, ParseElement* second)
{
	return new SimpleBinaryOperator(first, SimpleBinaryOperator::BOP_AND, second);
}

/* Normal class methods */

// Default constructor. Initializes attributes to blank/empty.
Translator::Translator(Language lang)
{
	Sort *newSort, *additiveConstantSort, *actionSort;
	SortList tmpList;

	mLanguage = lang;

	/**************************************** Initialize IO ********************************************/
	// Open a null file output and set ostNull to it.
	fNull.open("/dev/null");
	ostNullPtr = new std::ostream(fNull.rdbuf());

	// Start ostOutPtr and ostErrPtr using fNull as well.
	ostOutPtr = new std::ostream(ostNullPtr->rdbuf());
	ostErrPtr = new std::ostream(ostNullPtr->rdbuf());

	// Allocate an initial tempQuery.
	tempQuery = new Query();

	/**************************************** Initialize Sorts ********************************************/

	// Create boolean / boolean* domains
	newSort = this->createInternalSort("boolean*", NULL);
	newSort = this->getSort("boolean");
	this->createInternalObject("true", NULL, newSort);
	this->createInternalObject("false", NULL, newSort);

	// Create step/astep domains, the action time sort.
	if (lang != LANG_MVPF) {
		newSort = this->createInternalSort("astep", NULL);
		this->createInternalNumRange("0..maxstep-1", newSort);
		tmpList.push_back(newSort);
		newSort = this->createInternalSort("step", &tmpList);
		tmpList.clear();
		this->createInternalObject("maxstep", NULL, newSort);

		// The additiveConstant sort is a supersort for both additiveFluent and additiveAction.
		additiveConstantSort = this->createInternalSort("additiveConstant", NULL);

		// Action sorts.
		tmpList.push_back(this->createInternalSort("abAction", NULL));
		tmpList.push_back(this->createInternalSort("attribute", NULL));
		tmpList.push_back(this->createInternalSort("exogenousAction", NULL));
		newSort = this->createInternalSort("additiveAction", NULL);
		additiveConstantSort->addSubsort(newSort);
		tmpList.push_back(newSort);
		actionSort = this->createInternalSort("action", &tmpList);
		tmpList.clear();

		// Fluent sorts.
		tmpList.push_back(this->createInternalSort("inertialFluent", NULL));
		newSort = this->createInternalSort("additiveFluent", NULL);
		additiveConstantSort->addSubsort(newSort);
		tmpList.push_back(newSort);
		newSort = this->createInternalSort("simpleFluent", &tmpList); 			// inertialFluent and additiveFluent are both simpleFluents
		tmpList.clear();
		tmpList.push_back(newSort);
		tmpList.push_back(this->createInternalSort("sdFluent", NULL));
		tmpList.push_back(this->createInternalSort("rigid", NULL));				// Rigids aren't technically "fluents" after translation, but in CCalc they still are.
		this->createInternalSort("fluent", &tmpList);
		tmpList.clear();

		// Additive range sorts
		newSort = this->createInternalSort("nnAdditiveInteger", NULL);				// positive integer sort.
		this->createInternalNumRange("0..maxAdditive", newSort);
		newSort = this->createInternalSort("additiveInteger", NULL);				// positive + negative integer sort.
		this->createInternalNumRange("-maxAdditive..maxAdditive", newSort);
	
		// Apparently afValue is a synonym for this sort.
		newSort = this->createInternalSort("afValue", NULL);
		this->createInternalNumRange("-maxAdditive..maxAdditive", newSort);
	
		// Add the contribution constant for additive constants.
		tmpList.push_back(actionSort);
		tmpList.push_back(additiveConstantSort);
		Constant* contribution = new Constant("contribution", newSort, Constant::CONST_ACTION, true, (ConstSortList*)&tmpList);
		tmpList.clear();
		addSymbol(contribution);
	}

	// Computed sort for LUA functions
	this->createInternalSort("computed", NULL);

	// we want to use a dynamic translation.
	setStaticTranslation(lang != LANG_MVPF);

	blnFoundAbnormalities = false;
	blnFoundAdditive = false;

	// Initialize the incremental part
	mCurrentPart = IPART_NONE;

}



// Searches for a Query object with the given label
Query* Translator::getQuery(std::string const& testLabel)
{
	Query* retVal = NULL;
	for(std::list<Query*>::iterator qIter = queries.begin(); qIter != queries.end(); qIter++)
	{
		if((*qIter)->label == testLabel)
		{
			retVal = *qIter;
			break;
		}
	}
	return retVal;
}


Object* Translator::getOrCreateObject(std::string const& symName, Object::ObjectType type, bool internal, size_t arity) {
	Element* ret = getSymbol(symName, arity);

	if (!ret) {
		ret = new Object(symName, type, internal, arity);
		if (addSymbol(ret) != SymbolTable::ADDSYM_OK) {
			delete ret;
		}
	} else if (ret->getElemType() != Element::ELEM_OBJ) {
		error("Detected conflicting definition of identifier \"" + symName + "/" + utils::to_string(arity)+"\".", true);
	}


	return (Object*)ret;
}


// Tries to create a Sort out of an identifier and (possibly empty) subsort list.
Sort* Translator::addSort(std::string const& sortName, bool internal, SortList* subsorts, bool translateDeclaration, bool warnOnDup)
{
	std::string trimmedName = utils::trimWhitespace(sortName);

	Element* retVal = NULL;
	bool needTrans = false;

	if ((retVal = getSymbol(trimmedName))) {

		if (retVal->getElemType() != Element::ELEM_SORT) {
			error("Detected conflicting definition of identifier \"" + trimmedName + std::string("/0") + "\".", true);
		} else if (warnOnDup) {
			// Duplicate.
			warn("Found duplicate definition of identifier \"" + trimmedName + std::string("/0") + "\".", true);
		}

	} else {
		// It doesn't exist already. Add it.
		needTrans = true;

		// Select a unique variable.
		std::string baseName = sortNameToVariable(trimmedName, true);
		size_t i = 0;
		std::string varName = baseName;

		while (getSymbol(varName, 0)) {
			varName = baseName + utils::to_string(++i);
		}
		Variable* sortVar = new Variable(varName, true, NULL);

		mSymbolTable.addSymbol(sortVar);

		retVal = new Sort(trimmedName, sortVar, internal);
		addSymbol(retVal);
	}


	// If the sort is "something*" we should ensure that its parent exists and that
	// the appropriate subsorts are present.
	if (trimmedName.at(trimmedName.length()-1) == '*') {
		Sort* parentSort = addSort(trimmedName.substr(0, trimmedName.length()-1), internal, subsorts, translateDeclaration, false);
		needTrans |= ((Sort*)retVal)->addSubsort(parentSort, true);

		// In addition, we should add the 'none' object.
		Object* none = getOrCreateObject("none", Object::OBJ_NAME);
		bool needObjTrans = ((Sort*)retVal)->addObject(none, true);

		// Translate it, if necessary.
		if (translateDeclaration && needObjTrans) {
			translateObjectDecl(none, (Sort*)retVal);
		}

	} else {
		if (subsorts) {
			for (SortList::iterator it = subsorts->begin(); it != subsorts->end(); it++)
				needTrans |= ((Sort*)retVal)->addSubsort(*it, true);
		}
	}

	// Translate the declaration if anything has changed or the sort is new.
	if (translateDeclaration && needTrans) {
		translateSortDecl((Sort*)retVal);
	}

	return ((Sort*)retVal);
}


// Attempts to add a new Query object to the translator's data structures.
int Translator::addQuery(Query* newQuery)
{
	int retVal = SymbolTable::ADDSYM_ERR; // Start pessimistic.
	if(newQuery)
	{
		// Do some basic sanity checks, then try adding it into local data structures.
		if(newQuery->label == "")
		{
			newQuery->label = "0"; // An unlabeled query defaults to being query 0.
		}
		// Note: We no longer require queries to specify a maximum step.
		// If it turns out we do need a max step (ie static translation)
		// the wrapper will take care of it.
		if(this->getQuery(newQuery->label) == NULL)
		{
			queries.push_back(newQuery);
			retVal = SymbolTable::ADDSYM_OK;
		}
		else
		{	// A query with this label already exists, flag the dupe.
			retVal = SymbolTable::ADDSYM_DUP;
		}
	}
	return retVal;
}

// Translates a Constant element into an ASP-compatible constant declaration.
void Translator::translateConstantDecl(Constant const* transConst)
{
	// Sanity check: make sure the element isn't NULL.
	if(transConst)
	{

		// Detect if it's an abnormality constant and set the appropriate flag.
		if (transConst->isAbnormal()) {
			blnFoundAbnormalities = true;
		}

		// Detect if it's an additive constant and set the appropriate flag.
		if (transConst->isAdditive()) {
			blnFoundAdditive = true;
		}

		ElementCounter eCount; 											// Used to track occurrences of identical parameters (to avoid name clashes).
		std::vector<std::pair<Sort const*,std::string> > parameterMap;	// An ordered mapping from variables to parameters.

		std::string translatedConst;
		ClauseList localClauses;

		std::ostringstream stmtBuilder;									// Used to build each individual statement required for this declaration.
		StmtList stmts;													// Used to aggregate the statements we need and output them all simultaneously.

		// Translate the constant and save it for later..
		transConst->translate(stmtBuilder, localClauses, &eCount, NULL, &parameterMap);
		translatedConst = stmtBuilder.str();
		stmtBuilder.str("");
		
		// Ouput the class of constant, then its translated name.
		stmtBuilder << transConst->constTypeStr() << "(" << translatedConst << ")";

		if (localClauses.size() > 0) {
			stmtBuilder << " <- ";
			outputClauses(stmtBuilder, localClauses, false);
		}
		stmtBuilder << ".";

		// Add the first statement to the list.
		stmts.push_back(Statement(stmtBuilder.str(),IPART_BASE));
		stmtBuilder.str("");
		

		// We intentionally don't clear the clause list.

		// Output the line connecting a constant to its domain.
		if(transConst->domain())
		{
			stmtBuilder << "constant_sort(" << translatedConst << ",";
			stmtBuilder << transConst->domain()->fullTransName() << ")";

			if (localClauses.size() > 0) {
				stmtBuilder << " <- ";
				outputClauses(stmtBuilder, localClauses, false);
			}

			stmtBuilder << ".";

			stmts.push_back(Statement(stmtBuilder.str(), IPART_BASE));
			stmtBuilder.str("");
		}

		// If the constant is an attribute, connect the attribute to its action.
		if(transConst->constType() == Constant::CONST_ATTRIBUTE && ((Attribute const*)transConst)->parent())
		{
			stmtBuilder << "action_attribute(";
			bool matched = ((Attribute const*)transConst)->parent()->translate(stmtBuilder, localClauses, &eCount, &parameterMap, NULL);
			stmtBuilder << ", " << translatedConst << ")";

			if (localClauses.size() > 0) {
				stmtBuilder << " <- ";
				outputClauses(stmtBuilder, localClauses, false);
			}
			stmtBuilder << ".";

			if (!matched) {
				// The parameter prefixes don't match. Warn the user.
				warn(std::string("The attribute \"")
						+ transConst->fullName()
						+ std::string("\" does not contain all arguments within its parent action \"")
						+ ((Attribute const*)transConst)->parent()->fullName()
						+ std::string("\". This will likely cause unintended results."),
						true,
						true);
			}

			stmts.push_back(Statement(stmtBuilder.str(), IPART_BASE));
			stmtBuilder.str("");
		}

		// Output all of the statements...
		outputStmts(stmts);
	}
}

// Translates an Object element into an ASP-compatible declaration of an
// object as a member of a sort's domain.
void Translator::translateObjectDecl(Object const* transObj, Sort const* sortObj)
{
	// Sanity check: make sure the object isn't NULL, and neither is its sort.
	if(transObj && sortObj)
	{
		// An object delcaration is the sort's name, and then the object
		// name in parentheses.
		std::stringstream stmtBuilder;					// Used to build each individual statement required for this declaration.
		ClauseList localClauses;


		stmtBuilder << sortObj->fullTransName() << "(";
		transObj->translate(stmtBuilder, localClauses);
		stmtBuilder << ")";

		if (localClauses.size()) {
			stmtBuilder << " <- ";
			outputClauses(stmtBuilder, localClauses, false);
		}


		stmtBuilder << ".";

		// Output the statement
		output(stmtBuilder.str(), IPART_BASE, true);
	}
}


// Handles a LUA call by adding the result to the computed sort.
void Translator::handleLUACall(ObjectLikeElement const* lua_elem) {
	StmtList stmts;
	Sort *sortObj = getSort("computed");
	ClauseList extraClauses, freeVars;
	Context c(lang(), Context::POS_MAXIMIZED_INTERNAL, IPart::IPART_BASE, Context::BASE_STR, &extraClauses, &freeVars, false, true, &stmts);
	std::stringstream stmtBuilder;					// Used to build each individual statement required for this declaration.

	stmtBuilder << sortObj->fullTransName() << "(";
	lua_elem->translate(stmtBuilder, c);
	stmtBuilder << ")";
	

	if (extraClauses.size()) {
		stmtBuilder << " <- ";
		outputClauses(stmtBuilder, extraClauses, false);
	}

	
	stmtBuilder << ".";

	// Output the statement
	output(stmtBuilder.str(), IPART_BASE, true);
}



// Translates a Sort element into an ASP-compatible sort declaration.
void Translator::translateSortDecl(Sort const* transSort)
{
	// Sanity check: make sure the sort isn't NULL.
	if(transSort)
	{
		StmtList stmts;									// Used to aggregate the statements we need and output them all simultaneously.

		// A translated sort declaration involves three things:
		// Declare the sort name.
		stmts.push_back(Statement("sort(" + transSort->fullTransName() + ").",IPART_BASE));

		// The sort's default variable and the line connecting the sort 
		// to its objects (if the variable reference isn't NULL).
		if(transSort->var())
		{
			stmts.push_back(
				Statement(
						"#domain " + transSort->fullTransName()
						+ "(" + transSort->var()->fullTransName() + ").", IPART_BASE
						)
				);

			stmts.push_back(
				Statement(
						"sort_object(" + transSort->fullTransName() + ","
						+ transSort->var()->fullTransName() + ").", IPART_BASE
						)
				);
		}
		else
		{
			/// @todo Error if sortVar == NULL.
		}

		// If the sort has subsorts, add lines indicating that all members of
		// the subsorts are also members of the sort.
		if(transSort->numSubsorts())
		{
			for (SortList::const_iterator it = transSort->beginSubsorts(); it != transSort->endSubsorts(); it++) {
				Translator::translateSubsortDecl(transSort, *it);
			}
		}

		// Additionally, we're going to hide the sort from view to prevent clutter
		stmts.push_back(
			Statement(
					"#hide " + transSort->fullTransName() + "("
					+ transSort->var()->fullTransName() + ").\n", IPART_BASE
					)
			);

		// Output all the statements!
		outputStmts(stmts);
	}
}

// Translates a declaration of a super/sub-sort relationship between two sorts.
void Translator::translateSubsortDecl(Sort const* transSupersort, Sort const* transSubsort)
{
	if(transSupersort && transSubsort)
	{

		if(transSubsort->var())
		{
			output(
					transSupersort->fullTransName() + "(" + transSubsort->var()->fullTransName() + ")."
					, IPART_BASE, true
					);
		}
		else
		{
			/// @todo Error if transSubsort's sortVar == NULL.
		}
	}
}

// Translates a Variable element into an ASP-compatible variable declaration.
void Translator::translateVariableDecl(Variable const* transVar)
{
	// We've got a version that already creates a string of the declaration,
	// just use that and send it to output.
	output(Translator::translateVariableDeclToString(transVar), IPART_BASE, true);
}


// Translates a Query into an ASP-compatible query declaration.
void Translator::translateQuery(Query const* transQuery)
{
	ClauseList localClauses;
	StmtList stmts;

	// TODO: These aren't behaving correctly...

	if(transQuery && transQuery->label != "")
//			&& (!blnStaticTrans || transQuery->maxstep != ""))
	{
		std::ostringstream ossOutputBuffer; // Holds translated output so things can be easily added on before or after the normal output.

		// Output special "hint" flags/comments for this query.
		std::string hintMaxstep = transQuery->maxstep;

		// f2lp does weird things with periods in comments, so replace the dots in a ranged maxstep with something else.
		if(hintMaxstep.find("..") != std::string::npos)
		{
			hintMaxstep.replace(hintMaxstep.find(".."), 2, "::");
		}
		stmts.push_back(
			Statement(
					"% [Query: Label:" + transQuery->label + ((hintMaxstep != "") ? ", Maxstep:" + hintMaxstep : "" ) + "]",
					IPART_NONE
					)
			);

		// For each item in queryConditions, output an ASP-style constraint for it.
		for(ParseElementList::const_iterator lIter = transQuery->queryConditions.begin(); lIter != transQuery->queryConditions.end(); ++lIter)
		{
			IPart part = IPART_BASE;
			bool alteredTimestamp = false;
			bool malformed = false;
			int tmpLoc;
			std::string tmpQueryTimeStamp;

			// set up everything for our use
			ossOutputBuffer.str("");

			// Transform 'maxstep' to the appropriate timestamp
			if (!blnStaticTrans) {
				part = (*lIter)->determineQueryIPart();
			}

			// Quick checking for undeclared identifiers....
			BaseElementList maybeUndefined, undefined;
			(*lIter)->aggregateUndefined(maybeUndefined);

			if (!resolveDynamicDeclarations(maybeUndefined, undefined)) {
				// One or more undefined elements.
				// Throw an error
				std::ostringstream tmpErr;
				tmpErr << "Undeclared identifiers were encountered. The following identifiers are undeclared: ";
				for (BaseElementList::const_iterator it = undefined.begin(); it != undefined.end(); ) {
					tmpErr << "\"";
					(*it)->fullName(tmpErr);
					tmpErr << "\"";
					if (++it != undefined.end()) tmpErr << ", ";
				}
				tmpErr << ".";
				error(tmpErr.str(),true);
				malformed = true;
			}


			// Don't translate malformed queries
			if (malformed) continue;


			// Translate the query.
			std::ostringstream stmtBuilder;
			Context localContext = Context(lang(),
					Context::POS_QUERY,
					part,
					Context::EMPTY_STR,
					NULL,
					NULL,
					true,
					true,
					&stmts
			);


			stmtBuilder << "false <- query_label(" << transQuery->label << ") & not (";

			bindAndTranslate(stmtBuilder, (*lIter), localContext, false, false);

			// end the statement.
			stmtBuilder << ").";

			// push the results in the statements list.
			stmts.push_back(Statement(stmtBuilder.str(), part));

		}

		// output all the resulting statements.
		outputStmts(stmts);
	}
}

// Translates a causal law into ASP-compatible rule(s).
void Translator::translateCausalLaw(
	ParseElement* head,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* afterBody,
	ParseElement* unlessBody,
	ParseElement* whereBody
	)
{

	// The initialized values default to "static law" (i.e., fluents only, no "after").
	RuleType type = RULE_STATIC;	// The type of the rule we're currently working with.

	bool assumingNotNot = false;	// Whether the assuming body should be encased in double negation.
	bool malformed = false; 		// true if one or more problems with the law have been detected.

	std::ostringstream ossOutputBuffer; // Holds translated output so things can be easily added on before or after the normal output.

	std::string baseTimeStamp;	// The timestamp used to index this rule with.

	// lists used to capture additional clauses and statments.
	StmtList stmts;

	// Do some basic checking on the structure of the law. This includes determining the type of law we're working with
	// and performing some basic sanity checking on each of the components...

	ParseElement* tmpAssuming = NULL;
	SimpleBinaryOperator *tmp = NULL;
        
    bool allowChoiceInHead = false; 	// Whether choice rules are allowed in the head of a rule.
	bool allowAtomicNegationInHead = false;	// whether atomic negation (-p) is allowed in the head of a rule.    
    
	// step -1: Check language specific constructs
	switch (lang()) {
	case LANG_CPLUS:
                allowChoiceInHead = false;
				allowAtomicNegationInHead = true;
		if (!ifBody) tmpAssuming = assumingBody;	
		else if (!assumingBody) { 
			tmpAssuming = ifBody;
			ifBody = NULL;
		} else {
			ifBody->parens(true);
			assumingBody->parens(true);
			tmpAssuming = tmp = new SimpleBinaryOperator(ifBody, SimpleBinaryOperator::BOP_AND, assumingBody);
			ifBody = NULL;
		}
		break;
	case LANG_BC:
		tmpAssuming = assumingBody;
        allowChoiceInHead = false;
		allowAtomicNegationInHead = false;

		// Enforce conjunction of literals in the rule bodies.

#define ENFORCE_BC_ASSUMING_CONJ
#ifdef ENFORCE_BC_ASSUMING_CONJ
		// TODO: Do we want to enforce that the assuming body
		// is a conjunction of literals?
		if (assumingBody && !assumingBody->isDefinite(true)) {
			error("Language BC does not support arbitrary formulas in the assuming clause of a rule.", true);
			malformed = true;
		}
#endif

		if (ifBody && !ifBody->isDefinite(true)) {
			error("Language BC does not support arbitrary formulas in the if clause of a rule.", true);
			malformed = true;
		}
		if (afterBody && !afterBody->isDefinite(true)) {
			error("Language BC does not support arbitrary formulas in the after clause of a rule.", true);
			malformed = true;
		}
		break;

	case LANG_BCPLUS:
		tmpAssuming = assumingBody;
        allowChoiceInHead = true;
		allowAtomicNegationInHead = false;
                
		break;
	case LANG_MVPF:
		tmpAssuming = assumingBody;
		allowAtomicNegationInHead = false;
		allowChoiceInHead = true;

		if (afterBody) {
			error("The after clause is not supported in MVPF.", true);
			malformed = true;
		}


		break;

	}



	// Step 0, check to ensure that there are no undefined identifiers (outside the unless clause).
	BaseElementList maybeUndefined, undefined;
	if (head) head->aggregateUndefined(maybeUndefined);
	if (ifBody) ifBody->aggregateUndefined(maybeUndefined);
	if (tmpAssuming) tmpAssuming->aggregateUndefined(maybeUndefined);
	if (afterBody) afterBody->aggregateUndefined(maybeUndefined);


	if (!resolveDynamicDeclarations(maybeUndefined, undefined))
	{
		// One or more undefined elements.
		// Throw an error
		std::ostringstream tmpErr;
		tmpErr << "Undeclared identifiers were encountered. The following identifiers are undeclared: ";
		for (BaseElementList::const_iterator it = undefined.begin(); it != undefined.end(); ) {
			tmpErr << "\"";
			(*it)->fullName(tmpErr);
			tmpErr << "\"";
			if (++it != undefined.end()) tmpErr << ", ";
		}
		tmpErr << ".";
		error(tmpErr.str(),true);
		malformed = true;
	}



	// step 1: ensure the head is non-null and in the correct form
	if (!head || !head->isDefinite(false, allowChoiceInHead, allowAtomicNegationInHead)) {
		error("Definite causal laws must have exactly one constant in the head.\n");
		malformed = true;
		return;
	}

	// step 2: Ensure that LUA calls only occur in the law's where clause (if it exists).
	if ( head->hasLuaCalls(false,false)
			|| (unlessBody && unlessBody->hasLuaCalls(false,false))
			|| (ifBody && ifBody->hasLuaCalls(false,false))
			|| (afterBody && afterBody->hasLuaCalls(false,false))
			|| (tmpAssuming && tmpAssuming->hasLuaCalls(false,false)))
	{
		// They have at least one call to lua outside the where clause.
		// Throw an error.
		error("External function calls are only permitted in the where clauses of a causal law.\n");
		malformed = true;
	}




	// step 3: Ensure that abnormalities are where they are supposed to be...
	if (lang() != LANG_BCPLUS &&
		(
			head->hasConstants(ParseElement::MASK_AB)
			|| (ifBody && ifBody->hasConstants(ParseElement::MASK_AB))
			|| (afterBody && afterBody->hasConstants(ParseElement::MASK_AB))
			|| (tmpAssuming && tmpAssuming->hasConstants(ParseElement::MASK_AB))
			|| (unlessBody && unlessBody->hasConstants(ParseElement::MASK_AB))
			|| (whereBody && whereBody->hasConstants(ParseElement::MASK_AB))
		)) { 
		error("External constants are only supported in BC+.", true);
		malformed = true;
	} else if ( lang() == LANG_BCPLUS && 
			(
				head->hasConstants(ParseElement::MASK_AB)
				|| (unlessBody && unlessBody->hasConstants(ParseElement::MASK_AB))
				|| (whereBody && whereBody->hasConstants(ParseElement::MASK_AB))
			)) {
		error("External constants can not occur in the head of a law or in it's 'unless' or 'where' body.", true);
		malformed = true;
	}

	// step 3.5: Ensure that the where clause doesn't contain anything special.
	if ( whereBody && whereBody->hasConstants(ParseElement::MASK_NON_TRIVIAL)) {
		error("The 'where' clause of a law must not contain constants.\n");
		malformed = true;
	}


//#define VERBOSE_DEBUG

#ifdef VERBOSE_DEBUG
	std::cout << "# Head: ";
	if (head) head->fullName(std::cout);
	else std::cout << "NULL";
	std::cout << "\n#\tAssuming: ";
	if (tmpAssuming) tmpAssuming->fullName(std::cout);
	else std::cout << "NULL";
	std::cout << "\n#\tIf: ";
	if (ifBody) ifBody->fullName(std::cout);
	else std::cout << "NULL";
	std::cout << "\n#\tAfter: ";
	if (afterBody) afterBody->fullName(std::cout);
	else std::cout << "NULL";
	std::cout << "\n#\tUnless: ";
	if (unlessBody) unlessBody->fullName(std::cout);
	else std::cout << "NULL";
	std::cout << "\n#\tWhere: ";
	if (whereBody) whereBody->fullName(std::cout);
	else std::cout << "NULL";
#endif


	// Step 4a: Determine if the law is a 'rigid' law, which contains a rigid fluent in the head (Alternatively, if it contains 'exogenous' or 'inertial').
	if (head->hasConstants(ParseElement::MASK_RIGID)) {
		type = RULE_RIGID;
#ifdef VERBOSE_DEBUG
		std::cout << "# RIGID LAW\n";
#endif
		// Verification: We don't allow any non-rigid fluents in the rule.
		if (head->hasConstants(ParseElement::MASK_NON_RIGID)
				|| (ifBody && ifBody->hasConstants(ParseElement::MASK_NON_RIGID))
				|| (tmpAssuming && tmpAssuming->hasConstants(ParseElement::MASK_NON_RIGID))
				|| (unlessBody && unlessBody->hasConstants(ParseElement::MASK_NON_RIGID))
			)
		{
			error("A causal law w/ rigid fluents in the head and non-rigid fluents in the body is (currently) unsupported.\n");
			malformed = true;
		}

		if (afterBody) {
			error("A dynamic law w/ rigid fluents in the head is (currently) unsupported.\n");
			malformed = true;
		}
	}

	// Step 4ab: Rigid constraints
	else if (!head->hasConstants(ParseElement::MASK_NON_RIGID)
				&& (!ifBody || !ifBody->hasConstants(ParseElement::MASK_NON_RIGID))
				&& (!tmpAssuming || !tmpAssuming->hasConstants(ParseElement::MASK_NON_RIGID))
				&& (!afterBody)
				&& (!unlessBody || !unlessBody->hasConstants(ParseElement::MASK_NON_RIGID))) {

		type = RULE_RIGID;
#ifdef VERBOSE_DEBUG
		std::cout << "# RIGID LAW\n";
#endif
	}

	// Step 4b: Determine whether the law is a static law, an action dynamic law, or a fluent dynamic law...
	// Static laws...
	else if (head->hasConstants(ParseElement::MASK_FLUENT | ParseElement::MASK_TRUE_FALSE) && !head->hasConstants(ParseElement::MASK_ACTION)
			&& (!ifBody || !ifBody->hasConstants(ParseElement::MASK_ACTION))
			&& (!tmpAssuming || !tmpAssuming->hasConstants(ParseElement::MASK_ACTION))
			&& !afterBody)
	{
		type = RULE_STATIC;
		baseTimeStamp = staticTimeStamp;
#ifdef VERBOSE_DEBUG
		std::cout << "# STATIC LAW\n";
#endif
	}

	// fluent dynamic laws...
	else if (!head->hasConstants(ParseElement::MASK_ACTION)
			&& (!ifBody || !ifBody->hasConstants(ParseElement::MASK_ACTION))
			&& (!tmpAssuming || !tmpAssuming->hasConstants(ParseElement::MASK_ACTION)))
	{
		type = RULE_FLUENTDYNAMIC;
		baseTimeStamp = dynamicTimeStamp;
#ifdef VERBOSE_DEBUG
		std::cout << "# FLUENT DYNAMIC LAW\n";
#endif
	}
	// action dynamic laws...
	else if (head->hasConstants(ParseElement::MASK_ACTION) && !head->hasConstants(ParseElement::MASK_FLUENT)
			&& !afterBody)
	{
		type = RULE_ACTIONDYNAMIC;
		baseTimeStamp = dynamicTimeStamp + "-1";
#ifdef VERBOSE_DEBUG
		std::cout << "# ACTION DYNAMIC LAW\n";
#endif
	}
	// Malformed laws...
	else
	{
		error("A malformed causal law was encountered. Please ensure that all laws are either static, action dynamic, or fluent dynamic laws.\n");
		malformed = true;
	}

	// Now check if we actually need "not not (...)" encasing the law's body (if it has one).
	if (!head->isTrivial() && tmpAssuming && !tmpAssuming->isTrivial()) {
			assumingNotNot = true;
#ifdef VERBOSE_DEBUG
			std::cout << "# ADDING NOT NOT...";
#endif
	}


	// Step 5, declare undefined identifiers in the unless clause.
	maybeUndefined.clear();
	undefined.clear();
	if (unlessBody) {
		unlessBody->aggregateUndefined(maybeUndefined);
		resolveDynamicDeclarations(maybeUndefined, undefined);


		ConstSortList tmpParamSorts;
		for (BaseElementList::const_iterator it = undefined.begin(); it != undefined.end(); it++) {
			tmpParamSorts.clear();
			bool success = (*it)->guessParamSorts(tmpParamSorts);

			if (success) {
				Constant::ConstantType abType =
						(type == RULE_STATIC || type == RULE_RIGID)
						? Constant::CONST_ABFLUENT
						: Constant::CONST_ABACTION;

				// We successfully guessed every sort that was required. Finish the declaration.
				Constant* newConst = new Constant(
						(*it)->baseName(),
						getSort("boolean"),
						abType,
						false,
						&tmpParamSorts
				);

				translateConstantDecl(newConst);
				if (addSymbol(newConst) != SymbolTable::ADDSYM_OK) {
					error("An error ocurred while declaring the ab constant '" + newConst->baseName() + "/" + utils::to_string(newConst->arity()) + "'", true);
					delete newConst;
					malformed = true;
				} else {
					// Make sure we update the declaration.
					(*it)->ref(newConst);
				}
			} else {
				// Unable to dynamically declare the constant.
				// Tell the user.
				error("Unable to dynamically declare \"" + ((ParseElement*)(*it))->fullName() + "\" as the constant's parameter sorts could not be determined. Declaring the constant beforehand will fix this issue.",true);
				malformed = true;
			}
		}
	}


	// Step 6: Check that SDFluents don't occur in the head of dynamic laws.
	if (type == RULE_FLUENTDYNAMIC && head->hasConstants(ParseElement::MASK_SDFLUENT)) {
		error("An sdFluent cannot occur in the head of a dynamic law.", true);
		malformed = true;
	}

	// catch malformed rules and don't translate them.
	if (malformed) return;

	// Let's do the translation!

	// We may need a 'static' copy for the base module
	if (!blnStaticTrans && (type == RULE_STATIC || type == RULE_RIGID)) {
		makeCausalTranslation(
			ossOutputBuffer,
			stmts,
			IPART_BASE,
			assumingNotNot,
			"0",
			head,
			ifBody,
			tmpAssuming,
			unlessBody,
			afterBody,
			whereBody
		);
		stmts.push_back(Statement(ossOutputBuffer.str(),IPART_BASE));
		ossOutputBuffer.str("");
	}

	// We also are going to need a dynamic version of the rule, unless the rule is rigid.
	if (blnStaticTrans || type != RULE_RIGID) {
		makeCausalTranslation(
			ossOutputBuffer,
			stmts,
			IPART_CUMULATIVE,
			assumingNotNot,
			baseTimeStamp,
			head,
			ifBody,
			tmpAssuming,
			unlessBody,
			afterBody,
			whereBody
		);
		stmts.push_back(Statement(ossOutputBuffer.str(),IPART_CUMULATIVE));
		ossOutputBuffer.str("");
	}


	// write the resulting statements to the output
	outputStmts(stmts);

	// cleanup
	if (tmp) {
		tmp->detachPostOp();
		tmp->detachPreOp();
		delete tmp;
	}

}

// Helper method for the translation.
std::ostream& Translator::makeCausalTranslation(
	std::ostream& output,
	StmtList& extraStmts,
	IPart ipart,
	bool assumingNotNot,
	std::string const& baseTimeStamp,
	ParseElement* head,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* unlessBody,
	ParseElement* afterBody,
	ParseElement* whereBody
	)
{
	ClauseList localClauses;
	Context localContext;
	bool bodyContent = false; 		// true if the law's body has any content to-date.

	std::string actionTimeStamp = baseTimeStamp + "-1";

	// Translate!

	// The head
	/// @todo If head and ifBody are: not NULL, both const-like or both UOP_NOT(const-like), make a choice rule out of head and translate that.

	localContext = Context(lang(), Context::POS_HEAD, ipart, baseTimeStamp, &localClauses, NULL, false, true, &extraStmts);
	bindAndTranslate(output, head, localContext, true, true);

	// The body, if there is one.
	if(ifBody
			|| assumingBody
			|| unlessBody
			|| afterBody
			|| whereBody
			|| localClauses.size() > 0)
	{
		output << " <- ";

		// head clauses should be placed in the scope of double negation.
		if (localClauses.size()) {
			bodyContent = true;
			output << "not not (";
			outputClauses(output, localClauses, false);
			output << ")";
			localClauses.clear();

		}


		// "if" part of body, if there is one.
		if(ifBody)
		{
			if(bodyContent)	output << " & ";
			else bodyContent = true;

			// If we're translating a law that needs a "not not (...)" body wrapper to break cycles, add it.
			localContext = Context(lang(), Context::POS_BODY, ipart, baseTimeStamp, &localClauses, NULL, false, false, &extraStmts);
			ifBody->translate(output, localContext);
		}

		// "assuming" part of body, if there is one.
		if(assumingBody)
		{
			// add a connective if necessary.
			if(bodyContent)	output << " & ";
			else bodyContent = true;

			// If we're translating a law that needs a "not not (...)" body wrapper to break cycles, add it.
			if(assumingNotNot) {
				output << "not not (";
				localContext = Context(lang(), Context::POS_BODY, ipart, baseTimeStamp, NULL, NULL, true, false, &extraStmts);
				bindAndTranslate(output, assumingBody, localContext, false, true);
				output << ")";
			}
			else {
				localContext = Context(lang(), Context::POS_BODY, ipart, baseTimeStamp, &localClauses, NULL, false, false, &extraStmts);
				assumingBody->translate(output, localContext);
			}
		}

		// "unless" part of body, if there is one.
		if(unlessBody)
		{
			// add a connective if necessary.
			if(bodyContent)	output << " & ";
			else bodyContent = true;

			// Translate the unless body wrapped in negation.
			output << "not (";

			std::string tmp = 
					unlessBody->hasConstants(ParseElement::MASK_ACTION)
							? dynamicTimeStamp + "-1"
							: staticTimeStamp; 

			localContext = Context(lang(), Context::POS_BODY, ipart, tmp,
					NULL, NULL, true, false, &extraStmts);
			bindAndTranslate(output, unlessBody, localContext, false, true);
			output << ")";
		}

		// "after" part of body, if there is one.
		if(afterBody)
		{
			// add a connective if necessary.
			if(bodyContent)	output << " & ";
			else bodyContent = true;

			// If we're translating a law that needs a "not not (...)" body wrapper to break cycles, add it.
//			if(afterNotNot) {
//				output << "not not (";
//				localContext = Context(Context::POS_BODY, ipart, actionTimeStamp, NULL, NULL, true, false, &extraStmts);
//				bindAndTranslate(output, afterBody, localContext, false, true);
//				output << ")";
//			}
//			else {
				localContext = Context(lang(), Context::POS_BODY, ipart, actionTimeStamp, &localClauses, NULL, false, false, &extraStmts);
				afterBody->translate(output, localContext);
//			}

		}
		

		// "where" part of body, if there is one.
		if(whereBody)
		{
			// add a connective if necessary
			if(bodyContent)	output << " & ";
			else bodyContent = true;
			localContext = Context(lang(), Context::POS_BODY, ipart, baseTimeStamp, &localClauses, NULL, false, false, &extraStmts);
			whereBody->translate(output, localContext);
		}
		
		// handle extra clauses
		outputClauses(output, localClauses, bodyContent);
	}
	output << ".";

	return output;
}

// Handles a ':- show' declaration, adding the corresponding #show statements to the translation footer.
void Translator::translateShowStmt(ParseElementList const& atomicFormulas) {
	StmtList stmts;

	// By default, user defined atoms are shown in the answer set.
	// The presence of a show statement overrides this and makes them all
	// default to hidden.
	if (!blnEncounteredShowStmt) {
		blnEncounteredShowStmt = true;
		stmts.push_back(Statement("#hide h/2.", IPART_BASE));
		stmts.push_back(Statement("#hide h/1.", IPART_BASE));
	}


	// Iterate through each element and add the appropriate show statements.
	for (ParseElementList::const_iterator it = atomicFormulas.begin(); it != atomicFormulas.end(); it++) {

		// Sanity check: Non-null parse elements.
		if (!(*it)) continue;


		if ((*it)->getType() == ParseElement::PELEM_CONSTLIKE) {
			// It's a bare constant. Take this to be a shortcut for 'c=v' where V is a variable ranging of the constant's domain
			ConstantLikeElement* constlike = ((ConstantLikeElement*)(*it));
			if (constlike->domain())
			{
				makeShowStmt(*it, stmts, constlike->domain()->var());
			} else {
				// This appears to be missing one or more references...
				// Just try a normal translation and hope for the best.
				makeShowStmt(*it, stmts);
			}
		} else {
			// It's must be 'c=v'. We can translate this without any further ado
			makeShowStmt(*it, stmts);
		}
	}

	// Add the statements to the footer.
	addToFooter(stmts);
}

// Makes a '#show' statement for the provided elemenvoid Translator::makeShowStmt(ParseElement* elem, StmtList& extraStmts, Variable* eql) {
void Translator::makeShowStmt(ParseElement* elem, StmtList& stmts, Variable const* eql) {
	Context localContext;
	bool action = elem->hasConstants(ParseElement::MASK_ACTION); 
	std::stringstream tmp;

	if (!action) {
		if (eql) localContext = Context(lang(), Context::POS_BODY, IPART_BASE, Context::BASE_STR, eql->fullTransName(), NULL, NULL, false, false, &stmts);
		else localContext = Context(lang(), Context::POS_BODY, IPART_BASE, Context::BASE_STR, NULL, NULL, false, false, &stmts);
		tmp << "#show ";
		elem->translate(tmp, localContext);
		tmp << ".";
		stmts.push_back(Statement(tmp.str(), IPART_BASE));
		tmp.str("");
	}


	tmp << dynamicTimeStamp;
	if (action) tmp << "-1";
	std::string tmpstr = tmp.str();


	if (eql) localContext = Context(lang(), Context::POS_BODY, IPART_CUMULATIVE, 
		tmpstr, 
		eql->fullTransName(), NULL, NULL, false, false, &stmts);
	else localContext = Context(lang(), Context::POS_BODY, IPART_CUMULATIVE, 
		tmpstr, 
		"", NULL, NULL, false, false, &stmts);
	
	tmp.str("");
	tmp << "#show ";
	elem->translate(tmp, localContext);
	tmp << ".";
	stmts.push_back(Statement(tmp.str(), IPART_CUMULATIVE));


}

// Transforms declarative laws ("inertial p", "exogenous a(X)", "rigid q", etc.) to basic form and calls the translator for them.
bool Translator::translateDeclarativeLaw(
	ParseElement* head,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* afterBody,
	ParseElement* unlessBody,
	ParseElement* whereBody,
	SimpleUnaryOperator::UnaryOperatorType opType
	)
{
	bool retVal = false; // Start pessimistic.
	if(head != NULL)
	{
		// head needs to be wrapped in the appropriate declarative operator.
		SimpleUnaryOperator* tempPE = new SimpleUnaryOperator(opType, head);

		// This law becomes "delaration(F) [if G] [after H] [unless J] [when K] [following L] [where M]."
		translateCausalLaw(tempPE, ifBody, assumingBody, afterBody, unlessBody, whereBody);
		retVal = true;

		tempPE->detachPostOp();
		delete tempPE;
	}
	return retVal;
}

// Transforms a causal law of the form "always F [when H] [where G]." to basic form, then calls the translator for it.
bool Translator::translateAlwaysLaw(
	ParseElement* constraint,
	ParseElement* unlessBody,
	ParseElement* whereBody
	)
{
	bool retVal = false; // Start pessimistic.
	if(constraint != NULL)
	{
		// "not constraint" is the afterBody.
		SimpleUnaryOperator* tempPE = new SimpleUnaryOperator(SimpleUnaryOperator::UOP_NOT, constraint);

		// The head is "false".
		ObjectLikeElement* tempObj = new ObjectLikeElement("false", getOrCreateObject("false"));

		// The law becomes "caused false after -F when H where G."
		translateCausalLaw(tempObj, NULL, NULL, tempPE, unlessBody, whereBody);
		delete tempObj;
		tempPE->detachPostOp();
		delete tempPE;
		retVal = true;
	}
	return retVal;
}

// Transforms a causal law of the form "constraint F [after H] [where L]." to basic form, then calls the translator for it.
bool Translator::translateConstraintLaw(
	ParseElement* constraint,
	ParseElement* afterBody,
	ParseElement* unlessBody,
	ParseElement* whereBody,
	bool positive
	)
{
	bool retVal = false; // Start pessimistic.
	if(constraint != NULL)
	{
		// constraint has to be a fluent formula or the law is malformed. Soft warn if we can't tell what it is (i.e., not a fluent or action formula).
		if(!constraint->hasConstants(ParseElement::MASK_ACTION))
		{
			if(!constraint->hasConstants(ParseElement::MASK_FLUENT|ParseElement::MASK_RIGID))
			{
				warn("Cannot determine if \"" + constraint->fullName() + "\" is a fluent formula or not, it might not work as F in a \"constraint F\" style law.", true, true);
			}

			// "not constraint" is the ifBody.
			ParseElement* tempPE = NULL; // Points at whatever ends up becoming the head of the rule.
			SimpleUnaryOperator* tempUOP = NULL;

			if (positive) {
				// If the constraint is "not something", then we can just drop the not (we'd end up with "not not something", which is equivalent to "something" since "false" is the head).
				if(constraint->getType() == ParseElement::PELEM_UOP
						&& ((SimpleUnaryOperator*)constraint)->opType() == SimpleUnaryOperator::UOP_NOT)
				{
					tempPE = ((SimpleUnaryOperator*)constraint)->detachPostOp();
				}
				else
				{	// It's not an optimizable case, just tack "not" onto the constraint.
					tempPE = tempUOP = new SimpleUnaryOperator(SimpleUnaryOperator::UOP_NOT, constraint);
				}
			}
			else {
				// We are working with the negative version of the constraint, we don't have to negate the body.
				tempPE = constraint;
			}

			// The head is "false".
			ObjectLikeElement* tempObj = new ObjectLikeElement("false", getOrCreateObject("false"));

			// The law becomes "caused false if -F after H where L."
			translateCausalLaw(tempObj, tempPE, NULL, afterBody, unlessBody, whereBody);
			delete tempObj;
			if (tempUOP) {
				tempUOP->detachPostOp();
				delete tempUOP;
			} else if (tempPE != constraint) {
				// Put the constraint's postop back, just in case.
				((SimpleUnaryOperator*)constraint)->postOp(tempPE);
			}
			retVal = true;
		}
		else
		{
			error("\"" + constraint->fullName() + "\" is not a fluent formula, can't use it as F in a \"constraint F\" style law.", true, true);
		}
	}
	return retVal;
}

// Transforms a causal law of the form "default F [if G] [assuming H] [after J] [where M]." to basic form, then calls the translator on it.
bool Translator::translateDefaultLaw(
	ParseElement* head,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* afterBody,
	ParseElement* unlessBody,
	ParseElement* whereBody
	)
{
	bool retVal = false; // Start pessimistic.
	if(head != NULL)
	{
		// head and ifBody are going to end up together no matter what.
		if (head) head->parens(true);
		if (assumingBody) assumingBody->parens(true);
		SimpleBinaryOperator* tempPE = new SimpleBinaryOperator(head, SimpleBinaryOperator::BOP_AND, assumingBody);

		// This law becomes "caused F if G assuming F [& H] [after J] [where M]."
		translateCausalLaw(head, ifBody, tempPE, afterBody, unlessBody, whereBody);
		retVal = true;

		tempPE->detachPostOp();
		tempPE->detachPreOp();
		delete tempPE;

	}
	return retVal;
}

// Transforms a causal law of the form "nonexecutable F [if G] [unless U] [where K]." to basic form, then calls the translator for it.
bool Translator::translateNonexecutableLaw(
	ParseElement* nonEx,
	ParseElement* ifBody,
	ParseElement* unlessBody,
	ParseElement* whereBody
	)
{
	bool retVal = false; // Start pessimistic.
	if(nonEx != NULL)
	{
		// nonEx has to be an action formula or the law is malformed. Soft warn if its neither kind of formula.
		if(!nonEx->hasConstants(ParseElement::MASK_FLUENT))
		{
			if(!nonEx->hasConstants(ParseElement::MASK_ACTION))
			{
				warn("Cannot determine if \"" + nonEx->fullName() + "\" is an action formula or not, it might not work as F in a \"nonexecutable F\" style law.", true, true);
			}

			// nonEx and ifBody are going to end up together no matter what.
			if (ifBody) ifBody->parens(true);
			if (nonEx) nonEx->parens(true);
			SimpleBinaryOperator* tempPE = new SimpleBinaryOperator(nonEx, SimpleBinaryOperator::BOP_AND, ifBody);

			// Create a head of "false".
			ObjectLikeElement* tempObj = new ObjectLikeElement("false", getOrCreateObject("false"));

			// becomes caused false after F [& G] where K.
			translateCausalLaw(tempObj, NULL, NULL, tempPE, unlessBody, whereBody);

			delete tempObj;
			tempPE->detachPostOp();
			tempPE->detachPreOp();
			delete tempPE;

			retVal = true;
		}
		else
		{
			error("\"" + nonEx->fullName() + "\" is not an action formula, can't use it as F in a \"nonexecutable F\" style law.", true, true);
		}
	}
	return retVal;
}

// Transforms a causal law of the form "possibly caused F [if G] [assuming H] [after J] [unless K] [where N]." to basic form, then calls the translator on it.
bool Translator::translatePossiblyCausedLaw(
	ParseElement* head,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* afterBody,
	ParseElement* unlessBody,
	ParseElement* whereBody
	)
{
	bool retVal = false; // Start pessimistic.
	if(head != NULL)
	{
		// head and ifBody are going to end up together no matter what.
		if (assumingBody) assumingBody->parens(true);
		if (head) head->parens(true);
		SimpleBinaryOperator* tempPE = new SimpleBinaryOperator(head, SimpleBinaryOperator::BOP_AND, assumingBody);

		// This law becomes "caused F if G assuming F [& H] [after J] [unless K] [when L] [following M] [where N]."
		translateCausalLaw(head, ifBody, tempPE, afterBody, unlessBody, whereBody);

		retVal = true;
		tempPE->detachPostOp();
		tempPE->detachPreOp();
		delete tempPE;
	}
	return retVal;
}

// Transforms a causal law of the form "G may cause F [if H] [assuming L] [where K]." to basic form, then calls the translator on it.
bool Translator::translateMayCauseLaw(
	ParseElement* causer,
	ParseElement* causee,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* whereBody
	)
{
	bool retVal = false; // Start pessimistic.
	if(causer != NULL && causee != NULL)
	{
		// Causer has to be an action formula, or the law is not properly written.
		if(causer->hasConstants(ParseElement::MASK_ACTION) && !causer->hasConstants(ParseElement::MASK_FLUENT))
		{
			// Causer and assumingBody are going to end up together no matter what.
			if (causer) causer->parens(true);
			if (assumingBody) assumingBody->parens(true);
			SimpleBinaryOperator* tempPE = new SimpleBinaryOperator(causer, SimpleBinaryOperator::BOP_AND, assumingBody);

			// Where causer and ifBody end up in the basic form depend on what's in causee.
			if(!causee->hasConstants(ParseElement::MASK_FLUENT))
			{	// Causee is an action formula, this is "caused F if H assuming F & L where K".
				// Merge F into G & L.
				if (causee) causee->parens(true);
				SimpleBinaryOperator* tempPE2 = new SimpleBinaryOperator(causee, SimpleBinaryOperator::BOP_AND, tempPE);
				translateCausalLaw(causee, ifBody, tempPE2, NULL, NULL, whereBody);

				retVal = true;
				tempPE2->detachPostOp();
				tempPE2->detachPreOp();
				deallocateTempBinaryOp(tempPE2);
			}
			else if(!causee->hasConstants(ParseElement::MASK_ACTION))
			{	// Causee is a fluent formula, this is "caused F assuming F after G & H & L where K".
				// Merge H into G & L.
				if (ifBody) ifBody->parens(true);
				SimpleBinaryOperator* tempPE2 = new SimpleBinaryOperator(ifBody, SimpleBinaryOperator::BOP_AND, tempPE);
				translateCausalLaw(causee, NULL, causee, tempPE2, NULL, whereBody);

				retVal = true;
				tempPE2->detachPostOp();
				tempPE2->detachPreOp();
				deallocateTempBinaryOp(tempPE2);
			}
			else
			{	// Causee is mixed, that isn't allowed.
				warn("\"" + causee->fullName() + "\" must be a pure action formula or pure fluent formula in this law, it can't have both kinds of constants.", true, true);
			}

			tempPE->detachPostOp();
			tempPE->detachPreOp();
			deallocateTempBinaryOp(tempPE);
		}
		else
		{
			error("\"" + causer->fullName() + "\" is not an action formula, can't use it as G in a \"G may cause F\" style law.", true, true);
		}
	}
	return retVal;
}

// Transforms a causal law of the form "G causes F [if H] [assuming L] [where J]." to basic form, then calls the translator on it.
bool Translator::translateCausesLaw(
	ParseElement* causer,
	ParseElement* causee,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* unlessBody,
	ParseElement* whereBody
	)
{
	bool retVal = false; // Start pessimistic.
	if(causer != NULL && causee != NULL)
	{
		// Causer has to be an action formula, or the law is not properly written.
		if(causer->hasConstants(ParseElement::MASK_ACTION) && !causer->hasConstants(ParseElement::MASK_FLUENT))
		{
			// Causer and assumingBody are going to end up together no matter what.
			if (causer) causer->parens(true);
			if (assumingBody) assumingBody->parens(true);
			SimpleBinaryOperator* tempPE = new SimpleBinaryOperator(causer, SimpleBinaryOperator::BOP_AND, assumingBody);

			// Where causer and ifBody end up in the basic form depend on what's in causee.
			if((!causee->hasConstants(ParseElement::MASK_FLUENT)))
			{	// Causee is an action formula, this is "caused F if H assuming G & L".
				translateCausalLaw(causee, ifBody, tempPE, NULL, unlessBody, whereBody);
				retVal = true;
			}
			else if(!causee->hasConstants(ParseElement::MASK_ACTION))
			{	// Causee is a fluent formula, this is "caused F after G & L & H".
				if (ifBody) ifBody->parens(true);
				SimpleBinaryOperator* tempPE2 = new SimpleBinaryOperator(ifBody, SimpleBinaryOperator::BOP_AND, tempPE);

				translateCausalLaw(causee, NULL, NULL, tempPE2, unlessBody, whereBody);
				tempPE2->detachPostOp();
				tempPE2->detachPreOp();
				delete tempPE2;
				retVal = true;
			}
			else
			{	// Causee is mixed, that isn't allowed.
				error("\"" + causee->fullName() + "\" must be a pure action formula or pure fluent formula in this law, it can't have both kinds of constants.", true, true);
			}

			tempPE->detachPostOp();
			tempPE->detachPreOp();
			delete tempPE;
		}
		else
		{
			error("\"" + causer->fullName() + "\" is not an action formula, can't use it as G in a \"G causes F\" style law.", true, true);
		}
	}
	return retVal;
}

// Transforms an increment law of the form "A increments B by N [if H] [assuming L] [where K]." to basic form, then calls the translator on it.
bool Translator::translateIncrementLaw(
	ParseElement* causer,
	ParseElement* causee,
	ParseElement* increment,
	ParseElement* ifBody,
	ParseElement* assumingBody,
	ParseElement* unlessBody,
	ParseElement* whereBody,
	bool isIncrement
	)
{
	ParseElement* inc_expr = NULL;
	ConstantLikeElement* contrib = NULL;
	ParseElement* head, *newIf = NULL;

	std::list<SimpleBinaryOperator*> tmpBinOps;
	std::list<BaseLikeElement*> tmpBaseElems;

	bool malformed = false;

	// Some checking...

	if(!causer || !causee || !increment ) {
		error("An error occurred while parsing the incremental law.", true, true);
		return false;
	}

	/*********************************************************/
	/* Check the causer */
	/*********************************************************/

	if (causer->hasConstants(ParseElement::MASK_FLUENT)
		|| !causer->hasConstants(ParseElement::MASK_ACTION)
		|| causer->getType() != ParseElement::PELEM_CONSTLIKE)
	{
		error("\"" + causer->fullName() + "\" is not a boolean action, can't use it as A in a \"A increments B by N\" style law.", true, true);

		malformed = true;
	}
	else if (!((ConstantLikeElement*)causer)->ref()
		|| !((Constant const*)((ConstantLikeElement*)causer)->ref())->domain())
	{
		error("\"" + causer->fullName() + "\" does not appear to be properly declared, can't use it as A in a \"A increments B by N\" style law.", true, true);
		malformed = true;
	}
	else if (!((ConstantLikeElement*)causer)->isBoolean())
	{
		error("\"" + causer->fullName() + "\" is not a boolean action, can't use it as A in a \"A increments B by N\" style law.", true, true);
		malformed = true;
	}

	/*********************************************************/
	/* Check the causee */
	/*********************************************************/

	bool isAction = causee->hasConstants(ParseElement::MASK_ACTION);
	bool isFluent = causee->hasConstants(ParseElement::MASK_FLUENT);

	if ((!isAction && !isFluent) || (isAction && isFluent)
		|| causee->getType() != ParseElement::PELEM_CONSTLIKE)
	{
		error("\"" + causee->fullName() + "\" is not an additive constant, can't use it as B in a \"A increments B by N\" style law.", true, true);
		malformed = true;
	}
	else if (!((ConstantLikeElement*)causee)->ref()
		|| !((Constant const*)((ConstantLikeElement*)causee)->ref())->domain())
	{
		error("\"" + causee->fullName() + "\" does not appear to be properly declared, can't use it as B in a \"A increments B by N\" style law.", true, true);
		malformed = true;
	}

	else if (!((Constant const*)((ConstantLikeElement*)causee)->ref())->isAdditive())
	{
		error("\"" + causee->fullName() + "\" is not an additive constant, can't use it as B in a \"A increments B by N\" style law.", true, true);
		malformed = true;
	}

	/*********************************************************/
	/* Check the Increment Clause */
	/*********************************************************/

	if (!increment->isArithExpr()) {
		error( "\"" + increment->fullName() + "\" does not appear to be a valid arithmetic expression, can't use it as N in a \"A increments B by N\" style law.", true, true);
		malformed = true;
	}

	/**************************************************************/
	/* Done Checking */
	/**************************************************************/

	if (malformed) {
		return false;
	}

	// At this point, we know that the critical portions of the law appear to be
	// correct. We can procede with the translation.

	// Step one is to step up the atomic formula that will be in the head of our law.


	// Craft the LHS of the atomic formula
	ParseElementList tmpParams;
	tmpParams.push_back(causer);
	tmpParams.push_back(causee);
	tmpBaseElems.push_back(contrib = new ConstantLikeElement("contribution", getConstant("contribution", 2), &tmpParams));

	// Determine the RHS of the atomic formula.
	if (isIncrement) {
		inc_expr = increment;
	} else {
		tmpBaseElems.push_back(new ObjectLikeElement("-1", getOrCreateObject("-1")));
		if (increment) increment->parens(true);
		inc_expr = new SimpleBinaryOperator(tmpBaseElems.back(), SimpleBinaryOperator::BOP_TIMES, increment);
		tmpBinOps.push_back((SimpleBinaryOperator*) inc_expr);
	}

	// Craft the atomic formula

	//if (inc_expr) inc_expr->parens(true);
	head = new SimpleBinaryOperator(contrib, SimpleBinaryOperator::BOP_EQ, inc_expr);
	tmpBinOps.push_back((SimpleBinaryOperator*) head);

	// Step 2 is to craft the new if clause....
	// The new if body is the result of conjoining the old ifBody
	// with the boolean causer action.


	if (causer) causer->parens(true);
	tmpBaseElems.push_back(new ObjectLikeElement("true", getOrCreateObject("true")));
	tmpBinOps.push_back(new SimpleBinaryOperator(causer, SimpleBinaryOperator::BOP_EQ, tmpBaseElems.back()));

	if (ifBody || assumingBody) tmpBinOps.back()->parens(true);


	if (ifBody) {
		ifBody->parens(true);
		tmpBinOps.push_back(new SimpleBinaryOperator(tmpBinOps.back(), SimpleBinaryOperator::BOP_AND, ifBody));
	}

	if (assumingBody) {
		assumingBody->parens(true);
		tmpBinOps.push_back(new SimpleBinaryOperator(tmpBinOps.back(), SimpleBinaryOperator::BOP_AND, assumingBody));
	}

	newIf = tmpBinOps.back();		
	
	// Finally, translate!
	translateCausalLaw(head, newIf, NULL, NULL, unlessBody, whereBody);

	// Clean up!
	for (std::list<SimpleBinaryOperator*>::iterator it = tmpBinOps.begin(); it != tmpBinOps.end(); it++) {
		(*it)->detachPostOp();
		(*it)->detachPreOp();
		delete *it;
	}
	contrib->detachParams();

	for (std::list<BaseLikeElement*>::iterator it = tmpBaseElems.begin(); it != tmpBaseElems.end(); it++) {
		delete *it;
	}

	return true;
}



/* Class instance methods for general setup, etc. */

// Creates and adds a new object to the translator's data structures without translating or outputting it.
Object* Translator::createInternalObject(std::string const& newObjName, SortList* params, Sort* ofSort)
{
	Element* retVal = getSymbol(newObjName, (params) ? params->size() : 0);

	if (retVal && retVal->getElemType() != Element::ELEM_OBJ)
		error("Detected conflicting definition of \"" + retVal->baseName() + "/" + utils::to_string(retVal->arity()) + "\".", true);
		
	if (!retVal) {
		retVal = new Object(newObjName, Object::OBJ_NAME, true, params);
		if (addSymbol(retVal) != SymbolTable::ADDSYM_OK) {
			error("An error occurred while processing declaration of \"" + retVal->baseName() + "/" + utils::to_string(retVal->arity()) + "\".", true);
			delete retVal;
			retVal = NULL;
		}
	}

	if (retVal && ofSort) ofSort->addObject((Object*)retVal);

	return ((Object*)retVal);
}

// Creates and adds a new number range (child of Object) to the translator's data structures without translating or outputting it.
NumberRange* Translator::createInternalNumRange(std::string const& newObjName, Sort* ofSort)
{
	NumberRange* retVal = NULL;
	if(ofSort)
	{
		retVal = new NumberRange(newObjName);
		int addSymResult = this->addSymbol(retVal);
		if(addSymResult != SymbolTable::ADDSYM_OK)
		{	// Something went wrong adding the object, skip connecting & translating it.
			if(addSymResult == SymbolTable::ADDSYM_DUP)
			{	// Duplicate object. don't bother adding it.
				delete retVal;
				retVal = NULL;
			}
			else
			{	// A real object error. Skip adding it.
				delete retVal;
				retVal = NULL;
			}
		}
		else
		{	// Object add went okay, connect the sort to the object.
			ofSort->addObject(retVal);
		}
	}
	return retVal;
}


// Allocates a new Query instance and assigns it to tempQuery.
void Translator::allocateNewTempQuery(bool deallocateFirst)
{
	if(deallocateFirst && tempQuery)
	{
		delete tempQuery;
		tempQuery = NULL;
	}
	tempQuery = new Query();
}

// Sets a translator instance's output stream to the passed stream.
void Translator::setOutput(std::ostream &newOst)
{
	if(ostOutPtr)
	{
		ostOutPtr->rdbuf(newOst.rdbuf());
	}
}

// Sets whether a static translation should be outputted
void Translator::setStaticTranslation(bool staticTrans) {
	blnStaticTrans = staticTrans;

	// Make sure we update the timestamps.
	if (lang() == LANG_MVPF) {
		staticTimeStamp = "0";
		dynamicTimeStamp = "0";
	} else if (staticTrans) {
		staticTimeStamp = getSort("step")->varTransName();
		dynamicTimeStamp = getSort("astep")->varTransName();
	} else {
		staticTimeStamp = "maxstep";
		dynamicTimeStamp = "maxstep";
	}

}

// Sets a translator instance's error output stream to the passed stream.
void Translator::setErrorOutput(std::ostream &newOst)
{
	if(ostErrPtr)
	{
		ostErrPtr->rdbuf(newOst.rdbuf());
	}
}



// Outputs the contents of str to wherever the translator's output stream is aimed.
void Translator::output(std::string const& str, bool endWithNewline)
{
	(*ostOutPtr) << str;
	if(endWithNewline)
	{
		(*ostOutPtr) << std::endl;
	}
}

// Outputs the contents of str to wherever the translator's error output stream is aimed.
void Translator::error(std::string const& str, bool endWithNewline, bool location)
{
	outputToErr("% Error",str,endWithNewline,location);
	this->translatorNumErrors++;
}

// Outputs the contents of str to wherever the translator's error output stream is aimed.
void Translator::warn(std::string const& str, bool endWithNewline, bool location)
{
	outputToErr("% Warning",str,endWithNewline,location);
	this->translatorNumWarnings++;
}

// Outputs the contents of str to wherever the translator's error output stream is aimed.
void Translator::caution(std::string const& str, bool endWithNewline, bool location)
{
	outputToErr("% Caution",str,endWithNewline,location);
}

// Outputs the contents of str to wherever the translator's error output stream is aimed.
void Translator::pragma(std::string const& str, bool endWithNewline, bool location)
{
	outputToErr("% Pragma",str,endWithNewline,location);
}

void Translator::outputToErr(std::string const& hdr, std::string const& str, bool endWithNewline, bool location) {
	(*ostErrPtr) << hdr;
	if (location) {
		(*ostErrPtr) << " at "
			<< ltsyyGetFileName() << ":"
			<< ltsyyGetLoc().first_line
			<< "."
			<< ltsyyGetLoc().first_column;
	}
	(*ostErrPtr) << ": " << str;

	if(endWithNewline)
		(*ostErrPtr) << std::endl;
}

// Attempts to resolve dynamic declarations that weren't caught previously.
bool Translator::resolveDynamicDeclarations(BaseElementList& undefined, BaseElementList& reallyUndefined) {
	// Track the old size.
	size_t prevSize = reallyUndefined.size();	
	Constant* ref;
	
	// Move through each of the possibly undefined constants and attempt to find a definition for it.
	for (BaseElementList::iterator uIt = undefined.begin(); uIt != undefined.end(); uIt++) {

		if ((ref = getConstant((*uIt)->baseName(), (*uIt)->arity()))) {
			// found it
			(*uIt)->ref(ref);
		} else {
			// it really is undefined...
			reallyUndefined.push_back(*uIt);
		}
	}


	// Check if we've added anything to the 'really undefined list'.
	return reallyUndefined.size() == prevSize;
}

// helper to output clauses to a stream
std::ostream& Translator::outputClauses(std::ostream& out, ClauseList const& clauses, bool initConj) {
	if (initConj && !clauses.empty()) out << " & ";
	for (std::list<std::string>::const_iterator it = clauses.begin(); it != clauses.end(); ) {
		out << *it;
		if (++it != clauses.end()) out << " & ";
	}
	return out;
}

// helper to output a translated expression with a bound context.
std::ostream& Translator::bindAndTranslate(std::ostream& out, ParseElement const* expr, Context& context, bool upwardMobileClauses, bool suppressQuantifier) {

	if (!expr) return out;

	ClauseList localClauses, localVariables;
	Context localContext = context.mkBindClauses(&localClauses).mkBindVars(&localVariables);
	std::ostringstream tmp;

	expr->translate(tmp, localContext);

	// Existential quantification (if needed).
	if (!localVariables.empty() && !suppressQuantifier) {
		out << "?[";

		for (std::list<std::string>::const_iterator it = localVariables.begin(); it != localVariables.end(); ) {
				out << *it;
				if (++it != localVariables.end()) out << ", ";
		}
		out << "]:";
	}

	//if ((!localVariables.empty() && !suppressQuantifier)
	//		|| (!upwardMobileClauses && !localClauses.empty()))
	//{
		out << "(";
	//}

	out << tmp.str();

	if ((!localVariables.empty() && !suppressQuantifier) || (!localClauses.empty() && !upwardMobileClauses)) {
		Translator::outputClauses(out, localClauses, true);
		//out << ")";
	} else if (!localClauses.empty()){
		context.transferExtraClauses(localClauses);
	}
	out << ")";

	return out;
}

// @brief Converts the name of a language to its corresponding enum type.
bool Translator::strToLanguage(char const* str, Language& outLang) {
	if (!strcmp(str, "bc")) outLang = Language::LANG_BC;
	else if (!strcmp(str, "bc+")) outLang = Language::LANG_BCPLUS;
	else if (!strcmp(str, "c+")) outLang = Language::LANG_CPLUS;
	else if (!strcmp(str, "mvpf")) outLang = Language::LANG_MVPF;
	else return false;
	return true;
}

// Destructor. Deallocates all memory associated with the list attributes.
Translator::~Translator()
{
	if(this->tempQuery)
	{
		delete tempQuery;
		tempQuery = NULL;
	}

	for (std::list<Query*>::iterator it = queries.begin(); it != queries.end(); it++)
		delete *it;


	queries.clear();
	// Reset and deallocate the internal ostream pointers.
	if(ostNullPtr)
	{
		if(ostOutPtr)
		{
			ostOutPtr->rdbuf(ostNullPtr->rdbuf());
		}
		if(ostErrPtr)
		{
			ostErrPtr->rdbuf(ostNullPtr->rdbuf());
		}
	}
	delete ostOutPtr;
	delete ostErrPtr;
	delete ostNullPtr;
	// Close fNull so we don't leave an open file handle.
	fNull.close();
}

bool Translator::setIncrementalPart(IPart newIncPart) {
	if (!ostOutPtr) return false;
	if (newIncPart == mCurrentPart) return true;

	if (!blnStaticTrans) {
		switch (newIncPart) {
		case IPART_BASE:
			output("#base.",true);
			break;
		case IPART_CUMULATIVE:
			output("#cumulative " + dynamicTimeStamp + ".",true);
			break;
		case IPART_VOLATILE:
			output("#volatile " + dynamicTimeStamp + ".",true);
			break;
		case IPART_NONE:
			return true;
		default:
			return false;
		}
	}

	mCurrentPart = newIncPart;
	return true;

}

void Translator::output(std::string const& str, IPart ipart, bool endWithNewline) {
	if (mCurrentPart != ipart) setIncrementalPart(ipart);
	output(str, endWithNewline);
}

// helper to output statements to the output stream
void Translator::outputStmts(StmtList const& stmts) {
	for (StmtList::const_iterator it = stmts.begin(); it != stmts.end(); it++) {
		output(it->first, it->second, true);
	}
}

// Dumpts the symbol table to the output stream.
std::ostream& Translator::outputSymbolTable(std::ostream& out) const {
	using boost::property_tree::ptree;

	ptree table, *node, *node2;
	std::string loc;
	boost::property_tree::xml_writer_settings<char> settings('\t', 1);


	for (Element const* s : mSymbolTable) {
		// ignore all internal symbols
		if (s->internal()) continue;
	
		loc = "symbols." + Element::elemTypeToString(s->getElemType()) + "s";


		switch (s->getElemType()) {
		case Element::ELEM_CONST:
			loc += "." ;
			if (((Constant*)s)->isAction())
				loc += "actions.";
			else if (((Constant*)s)->isFluent())
				loc += "fluents.";
			else if (((Constant*)s)->isAbnormal())
				loc += "abnormalities.";
			else
				loc += "unknown.";
			loc += ((Constant*)s)->constTypeStr();

			node = &table.add(loc, "");

			node->add("<xmlattr>.name", s->baseName());
			node->add("<xmlattr>.arity", s->arity());
			node->add("<xmlattr>.trans_name", s->baseTransName());


			if (((Constant*)s)->constType() == Constant::CONST_ATTRIBUTE) {
				node->add("<xmlattr>.parent", ((Attribute*)s)->parent()->baseName()
					+ "/" + utils::to_string(((Attribute*)s)->parent()->arity()));
			}

			break;

		case Element::ELEM_VAR:
			loc += "." + Element::elemTypeToString(s->getElemType()); 
			node = &table.add(loc, "");

			node->add("<xmlattr>.name", s->baseName());
			node->add("<xmlattr>.trans_name", s->baseTransName());
			node->add("<xmlattr>.domain", ((Variable*)s)->domain()->baseName());
			break;
		case Element::ELEM_SORT:
			loc += "." + Element::elemTypeToString(s->getElemType()); 
			node = &table.add(loc, "");

			node->add("<xmlattr>.name", s->baseName());
			node->add("<xmlattr>.trans_name", s->baseTransName());


			for (Object const* obj : *(Sort*)s) {
				node2 = &node->add("object", "");
				node2->add("<xmlattr>.name", obj->baseName());
				node2->add("<xmlattr>.trans_name", obj->baseTransName());
				node2->add("<xmlattr>.arity", obj->arity());
			}

			for (SortList::const_iterator it = ((Sort*)s)->beginSubsorts(); it != ((Sort*)s)->endSubsorts(); it++) {
				node2 = &node->add("subsort.<xmlattr>.name", (*it)->baseName());
				

				for (Object const* obj : **it) {
					node2 = &node->add("object", "");
					node2->add("<xmlattr>.name", obj->baseName());
					node2->add("<xmlattr>.trans_name", obj->baseTransName());
					node2->add("<xmlattr>.arity", obj->arity());
				}
			}
			break;

		case Element::ELEM_OBJ:
			// handled by sorts.
			break;	
		}

	}

	for (Query const* q : queries) {
		node = &table.add("queries.query", "");
		node->add("<xmlattr>.label", q->label);
		node->add("<xmlattr>.steps", q->maxstep);
	}

	write_xml(out, table, settings);
	return out;
}
