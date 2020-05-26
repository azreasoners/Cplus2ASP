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


/** @file main.cpp
 *  @brief Transforms answer sets from answer set solvers into CCalc-style models.
 *  @author Joseph Babb (v1.X: Michael Casolary)
 *  @date 03/2013
 */

/* History:
 * v3.1 - Secondary restructure of parsing mechanism.
		- Predicate structure is now completely parsed.
		- now supports non-reified predicates of the form c_<type>_<arity>(<arg0>, <arg1>, ..., <argn>, <value>, <time>)
		- now supports non-reified external predicates of the form e_<type>_<arity>(<arg0>, <arg1>, ..., <argn>, <value>, <time>)
		- Other compatibility changes for cplus2asp version 3
 * v3.0 - Major program restructure. 
 *      - Now exposes an API to allow for parsing answer sets at the program level. 
 *      - Rewrite of the parser driver to increase stability and allow for complete oClingo compatibility.
 * v2.2 - Added nicer handling of timeless predicates of the forms p, eql(p,v), and eq(p,v) to allow for non-transitional answer sets to be displayed nicely.
 * v2.1 - Added the '--none-alias' command line option to allow for aliased values of none. 
 * v2.0 - Extended to allow for abnormality constants (staticAbnormality / dynamicAbnormality).
 * 		- Now uses four different top-level predicates to classify constant types.
 * 			- h/2 		- fluents
 * 			- occ/2 	- actions
 * 			- ab_h/2 	- static abnormalities
 * 			- ab_occ/2 	- dynamic abnormalities
 * 		- No longer requires fluent/1, actions/1, and rigid/1 predicates within the answer set.
 * v1.8 - Now exits with code 100 if no solutions are found, 101 if solutions are found, 1 if an error occurs, or 0 if there's nothing to do.
 * v1.7.3 - Fixed a bug where an unknown solver's output would be (erroneously) flagged as not having solutions due to bad text processing.
 * v1.7.2 - Optimized solution display routine, fixed extra space in solutions with no "timeless" predicates output, fixed bug where a solution with only actions wouldn't display properly.
 * v1.7.1 - Fixed a bug where bad time stamps could appear with "timeless" predicates.
 * v1.7 - Overhauled predicate detection, enabled support for new rigid and h-style predicate constructs, added option to show negated predicates.
 * v1.6.1 - Fixed bug where a time step with all false predicates would not be displayed.
 * v1.6 - Changed name to as2transition, added auto-hiding of "contribution" predicates  (and option to show them anyway).
 * v1.5 - Added capability to read output from cmodels and smodels in addition to clasp (& family), along with rudimentary ability to try to read an unknown/unidentified solver's output. Reduced memory usage and optimized handling of multiple solutions.
 * v1.4.1 - Fixed a bug with the program failing to recognize predicates beginning with "x_" inside of eq-style predicates.
 * v1.4 - Handling of eq-style predicates implemented, boolean predicates simplified, actions separated from fluents, option to print out solutions one predicate per line added.
 * v1.3.5 - (Internal) Split TimeStepInfo class off to its own file, created Predicate, Constant, and StringUtils classes, switched to using makefile to build program.
 * v1.3.1 - Program now outputs solutions as soon as they are ready to output instead of waiting until all solutions have been processed.
 * v1.3 - Added check for "x_" h-style predicates, and hide them by default. Added option to show these predicates anyway.
 * v1.2 - Added option controlling whether h-style predicates are added to solutions as a whole element or if their inner predicate is extracted and added alone.
 * v1.1 - Added support for newer clasp versions outputting "[UN]SATISFIABLE" at the end of the answer section, changed program version to explicitly store major, minor, revision.
 * v1.01 - Added version info.
 * v1.00 - First working version.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib> // for exit()
#include <cstring>
#include <algorithm>
#include <cctype>

#include "babb/utils/memory.h"

#include "as2transition/TransitionFormatter.h"

namespace u = babb::utils;
using namespace as2transition;


// Program version.
#define VERSION_MAJOR 3
#define VERSION_MINOR 0
#define VERSION_REV 0

/**
 * A wrapper for exit() that can output an optional custom message before leaving.
 * Outputs message to stdout if exitCode is 0, stderr otherwise. Doesn't output anything if message is NULL.
 * @param exitCode - The exit code to pass to exit.
 * @param message - A message to output to standard out before exiting (terminating newline automatically added).
 */
void nice_exit(int exitCode, const char* message)
{
	if(exitCode != 0 && message != NULL)
	{
		cerr << message << endl;
	}
	if(exitCode == 0 && message != NULL)
	{
		cout << message << endl;
	}
	exit(exitCode);
}

/// Skips the current line of an input stream.
std::istream& skipline(std::istream& in) {
	std::string tmp;
	getline(in, tmp);
	return in;
}

/// Skips the current line of an input stream.
std::ostream& printline(std::istream& in, std::ostream& out) {
	std::string tmp;
	getline(in, tmp);
	out << tmp << std::endl;
	return out;
}

// The codes used to tell calling programs / the shell if we found solutions or not.
enum ExitCodes { EXIT_NO_SOLUTIONS = 100, EXIT_SOLUTIONS = 101, EXIT_ERROR = 1 };


int main(int argc, char** argv)
{
	ifstream ifsFileIn;
	ofstream ofsFileOut;
	istream istIn(cin.rdbuf()); // Gets given a different buffer later.
	ostream ostOut(cout.rdbuf()); // Gets given a different buffer later.
	ExitCodes intExitCode = EXIT_ERROR; // What to exit with on successful program completion. 100 = No solutions, 101 = Solution(s) found.

	string strInFile, strOutFile, strTempString;

	u::ref_ptr<TransitionFormatter> formatter = new TransitionFormatter();

	size_t solutionNum = 0; // The number of the solution (answer set) currently being processed. 1-based (first solution is #1).
	bool blnShowHelp = false, blnBadArgs = false;

	
	for(int i = 1; i < argc && !blnBadArgs && !blnShowHelp; i++)
	{
		if(!formatter->parseOption(argv[i], true)) {
			if(strcmp(argv[i], "-f") == 0)
			{	// Input file being specified.  Verify they actually gave us a file argument (and they haven't already given this option).
				if(i < argc-1 && strInFile == "") {
					i++;
					strInFile = argv[i];
				} else {
					blnBadArgs = true;
				}
			}
			else if(strcmp(argv[i], "-o") == 0)
			{	// Output file being specified.  Verify they actually gave us a file argument (and they haven't already given this option).
				if(i < argc-1 && strOutFile == "") {
					i++;
					strOutFile = argv[i];
				} else {
					blnBadArgs = true;
				}
			}
			else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
			{	// Show version information and quit.
				cout << argv[0] << " version " << VERSION_MAJOR;
				cout << "." << VERSION_MINOR;
				if(VERSION_REV > 0) { cout << "." << VERSION_REV; }
				cout << endl;
				nice_exit(0, NULL);
			}
			else if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-?")) {
				// They've asked for help
				blnShowHelp = true;
			}
			else {
				// Unrecognized option. Hopefully this is a file.
	
				if (argv[i][0] != '-'
						&& strInFile == "") {
					strInFile = argv[i];
				} else {
					blnBadArgs = true;
				}
			}
		}
	}

	// If the user needs help, provide basic usage info and then exit.
	if(blnBadArgs || blnShowHelp)
	{
		if (blnBadArgs) std::cerr << "Error parsing command options.\n";
		cout 
			<< "Usage: " << argv[0] << " [OPTIONS] [-f IN_FILE] [IN_FILE] [-o OUT_FILE]" << endl

			<< "Transforms answer sets from answer set solvers into CCalc-style model lists." << endl
			<< "If given a value for IN_FILE, will parse that file, otherwise" << endl
			<< "will parse standard in (to support piping input)." << endl
			<< "If given a value for OUT_FILE, will output results to OUT_FILE," << endl
			<< "otherwise will output to standard out." << endl << endl;

		TransitionFormatter::help(std::cout, true);
					
		cout 
			<< "Other Options:" << endl
		  	<< "  --help shows this usage message." << endl
		  	<< "  -v  (or --version) outputs version information." << endl << endl
		  	<< "  Example: gringo some_asp_file.lp | clasp | " << argv[0] << endl
		  	<< "           would pass the answer sets generated by clasp directly" << endl
		  	<< "           to " << argv[0] << ", which would output the models to standard out." << endl
		  	<< "  Example: " << argv[0] << " -f answer_sets.txt -o models.txt" << endl
		  	<< "           would process the answer_sets.txt file and output the" << endl
		  	<< "           results to the file models.txt." << endl << endl << endl;

		if (blnBadArgs) nice_exit(1, NULL);
		else nice_exit(0, NULL);
	}

	
	// Now we have the input & output files (if given), let's process the input!
	
	// Figure out which input source we'll be using.
	if(strInFile != "")
	{
		ifsFileIn.open(strInFile.c_str());
		if(!ifsFileIn)
		{
			strTempString = "Error: Unable to open input file \"";
			strTempString += strInFile;
			strTempString += "\".";
			nice_exit(1, strTempString.c_str());
		}
		istIn.rdbuf(ifsFileIn.rdbuf());
	}
	else
	{
		istIn.rdbuf(cin.rdbuf());
	}
	
	// Figure out which output source we'll be using.
	if(strOutFile != "")
	{
		ofsFileOut.open(strOutFile.c_str());
		if(!ofsFileOut)
		{
			strTempString = "Error: Unable to open output file \"";
			strTempString += strOutFile;
			strTempString += "\".";
			nice_exit(1, strTempString.c_str());
		}
		ostOut.rdbuf(ofsFileOut.rdbuf());
	}
	else
	{
		ostOut.rdbuf(cout.rdbuf());
	}
	
	// In a loop, go looking for an answer set from the input.
	// get the first token of the line
	istIn >> strTempString;
	while(istIn.good())
	{
		// skip the header	

		// clasp
		if (strTempString == "clasp") 		{ skipline(istIn); istIn >> strTempString; continue; }
		if (strTempString == "Reading") 	{ skipline(istIn); istIn >> strTempString; continue; } 	// 'Reading from'
		if (strTempString == "Solving")		{ skipline(istIn); istIn >> strTempString; continue; }	// 'Solving...'

		// cmodels
		if (strTempString == "cmodels")		{ skipline(istIn); istIn >> strTempString; continue; }
		if (strTempString == "Program")		{ skipline(istIn); istIn >> strTempString; continue; }	// 'Prograim is tight'
		if (strTempString == "Calling")		{ skipline(istIn); istIn >> strTempString; continue; }	// 'Calling SAT solver'

		// smodels
		if (strTempString == "smodels")		{ skipline(istIn); istIn >> strTempString; continue; }

		// oclingo
		// no header

		// ---------------------------------------------------------------------------

		// clasp
/*
		if (strTempString == "SATISFIABLE" || strTempString == "UNSATISFIABLE") {
			// skip it.
			skipline(istIn);
			continue;
		}
*/		
		// cmodels
		// no footer

		// smodels
/*
		if (strTempString == "True" || strTempString == "False") {
			// skip it.
			skipline(istIn);
			continue;
		}
*/		
		// oclingo
		// no footer
	
		// ---------------------------------------------------------------------------

		// Show the answer sets, which are in lines following "Answer:"
		if (strTempString == "Answer:") {
			// print the line first
			// printline(istIn, ostOut, oldpos)
			solutionNum++;
			ostOut << "Solution:";
			printline(istIn, ostOut);
			// show the answer set
			formatter->format(istIn, ostOut, false);
			istIn >> strTempString;
			continue;
		} else if (strTempString == "Step:") {
			// oClingo compatibility: Prefaced with 'Step:'
			skipline(istIn);
			solutionNum++;
			formatter->format(istIn, ostOut, false);
			istIn >> strTempString;
			continue;
		}
	
		// If we are still here we have no idea what this output is. Show it.
		ostOut << strTempString;
		printline(istIn, ostOut);
		istIn >> strTempString; 
	
	}

	if (solutionNum) intExitCode = EXIT_SOLUTIONS;

	// We broke out of that loop either because we hit a footer or we don't have any
	// more to read. Just output whatever is left over.
	while (istIn.good()) {
		if (getline(istIn, strTempString))
			ostOut << strTempString << "\n";
	}
	
	// If we were using a file as the input source, close it now that we're done with input.
	if(ifsFileIn)
	{
		ifsFileIn.close();
	}
	
	// Did we get any solutions? If not, output a message indicating so.
	if(intExitCode != EXIT_SOLUTIONS)
	{
		ostOut << "No solution." << endl << endl;
		intExitCode = EXIT_NO_SOLUTIONS; // We didn't find any answer sets, so return the "no solutions" exit code.
	}
	
	
	// If we were using a file as the output destination, close it now that we're done with output.
	if(ofsFileOut)
	{
		ofsFileOut.close();
	}
	
	return intExitCode;
}

