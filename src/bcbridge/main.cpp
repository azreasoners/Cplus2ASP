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


/**
 * @file main.cpp
 * @brief Contains the primary driver for the acplusbridge which provides a CCalc like interface to oClingo's online progression.
 * @author Joseph Babb
 * @date 3/2013
 */

/* History:
 * v0.1 - Alpha development.
 */


#include "Driver.h"
#include "babb/utils/memory.h"
#include "as2transition/TransitionFormatter.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_REV 0

using namespace cplus2asp::bcbridge;

/**
 * @brief Displays the help dialog/
 * @param out The output stream to write the help dialog to.
 * @param the name of this application.
 */
void showHelp(std::ostream& out, char const* name);

/// Prints the version string to out.
void printVersion(std::ostream& out) {
	out << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REV;
}

int main(int argc, char const* argv[])
{

	Driver driver;

	// -------------------------------------------------- Option Parsing.

	for (int i = 1; i < argc; i++) {
		if (!driver.formatter().parseOption(argv[i], true)) {
			if (!strncmp(argv[i], "--port=", strlen("--port="))) {
				if (strlen(argv[i]) <= strlen("--port=") ||!driver.port(argv[i] + strlen("--port="))) { 
					std::cerr << "Error: '" << argv[i] << "' is not a valid port specification.\n";
					showHelp(std::cout, argv[0]);
					return 1;
				}
			} else if (!strcmp(argv[i], "-p")) {
				if (++i < argc) {
					if (!driver.port(argv[i])) { 
						std::cerr << "Error: '" << argv[i] << "' is not a valid port.\n";
						showHelp(std::cout, argv[0]);
						return 1;
					}
				} else {
					std::cerr << "Error: Expected a port number following '" << argv[i-1] << "'.\n";
					showHelp(std::cout, argv[0]);
					return 1;
				}
			} else if (!strncmp(argv[i], "--host=", strlen("--host="))) {
				if (strlen(argv[i]) <= strlen("--host=") ||!driver.host(argv[i] + strlen("--host="))) { 
					std::cerr << "Error: '" << argv[i] << "' is not a valid host specification.\n";
					showHelp(std::cout, argv[0]);
					return 1;
				}
			} else if (!strcmp(argv[i], "--time") || !strcmp(argv[i],"-t")) {
#ifdef BOOST_TIMER
				// display time
				driver.displayTime(true);
#else
				std::cerr << "Warning: The '" << argv[i] << "' flag requires that the Boost.Timer library is present at compile time. " << std::endl;
				std::cerr << "Warning: Ignoring '" << argv[i] << "'" << std::endl;
#endif
			} else if (!strcmp(argv[i], "-h")) {
				if (++i < argc) {
					if (!driver.host(argv[i])) { 
						std::cerr << "Error: '" << argv[i] << "' is not a host.\n";
						showHelp(std::cout, argv[0]);
						return 1;
					}
				} else {
					std::cerr << "Error: Expected a host name following '" << argv[i-1] << "'.\n";
					showHelp(std::cout, argv[0]);
					return 1;
				}
			} else if (!strcmp(argv[i],"-s")) {
				if (++i < argc) {
					if (!driver.symtab(argv[i])) {
						std::cerr << "Error: '" << argv[i] << "' is not a valid symbol table file.\n";
						showHelp(std::cout, argv[0]);
						return 1;
				
					}
				} else {
					std::cerr << "Error: Expected a file name following '" << argv[i-1] << "'.\n";
					showHelp(std::cout, argv[0]);
					return 1;
				}
			} else if (!strncmp(argv[i],"--symbol-table=", strlen("--symbol-table="))) {
				if (strlen(argv[i]) <= strlen("--symbol-table=") || !driver.symtab(argv[i] + strlen("--symbol-table="))) {
						std::cerr << "Error: '" << argv[i] << "' is not a valid symbol table file.\n";
						showHelp(std::cout, argv[0]);
						return 1;
				}
			} else if (!strcmp(argv[i], "--help") || !strcmp(argv[i],"-?") || !strcmp(argv[i],"?")) {
				showHelp(std::cout, argv[0]);
				return 0;
			} else {
				if (!driver.formatter().parseOption(argv[i], false)) {
					std::cerr << "Error: Unknown option: '" << argv[i] << "'.\n";
					return 1;
				}
			}
		}
	}


	// ----------------------------------------------------- The hard work.
	return driver.run();
}

// Displays the help dialog/
void showHelp(std::ostream& out, char const* name) {

	out << name << " version "; printVersion(out); out													<< std::endl
	<< "USAGE: " << name << " [OPTIONS]"																<< std::endl
																										<< std::endl
	<< "Available Options: "																			<< std::endl
	<< "  --port=PORT OR -p PORT "																		<< std::endl
	<< "        Specifies the port to connect to oClingo with. [Default: " << Driver::DEF_OCLINGO_PORT << " ]."	<< std::endl
	<< "  --host=HOST OR -h HOST "																		<< std::endl
	<< "        Specifies the host that oClingo is running on. [Default: " << Driver::DEF_OCLINGO_HOST << " ]." << std::endl
	<< "  --time OR -t "																				<< std::endl
	<< "        Prints the time oClingo took to respond after each iteration."							<< std::endl
#ifndef BOOST_TIMER
	<< "        NOTICE: Requires recompilation with the Boost.Timer library."							<< std::endl
#endif
	<< "  --symbol-table=SYMTAB OR -s SYMTAB "															<< std::endl
	<< "        Specifies a symbol table to load pre-defined BC+ symbols from."							<< std::endl
	<< "  --version OR -v "																				<< std::endl
	<< "        Displays the version of the program."													<< std::endl
	<< "  --help OR -? OR ?"																			<< std::endl
	<< "        Shows this dialog."																		<< std::endl
																										<< std::endl;

	out << "AS2Transition Options: " << std::endl;
	as2transition::TransitionFormatter::help(out, true);
}

