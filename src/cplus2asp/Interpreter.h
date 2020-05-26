
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


#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <string>
#include <iostream>

/**
 * @brief A static helper class to assist with interpreting interactive input.
 */
class Interpreter {
public:

	/***********************************************************/
	/* Types */
	/***********************************************************/
	/**
 	 * @brief The user actions recognized by the interpreter.
	 */
	typedef enum {
		SHOW_QUERIES,
		SHOW_CONFIG,
		SET_QUERY,
		SET_NUMSOLN,
		SET_MAXSTEP,
		SET_MINSTEP,
		EXIT,
		SHOW_HELP,
		INVALID
	} Action;


	/***********************************************************/
	/* Static Methods */
	/***********************************************************/
	
	/**
	 * @brief Prints a help dialog to the provided output stream detaling the options accepted by the interpreter.
	 * @param output The output stream to print to.
	 */
	static void showHelp(std::ostream& output);


	/**
	 * @brief Interprets the provided command.
	 * @param command THe command to interpret.
	 * @param[out] outVal A string containing any value accompanying the command, or empty if not applicable.
	 * @return The action requested by the user, or INVALID if the command wasn't recognized.
	 */
	static Action interpret(std::string const& command, std::string& outVal);


private:

	/***********************************************************/
	/* Static Private Methods */
	/***********************************************************/

	/**
	 * @brief Determines the action requested by the user.
	 * @brief The action's name.
	 * @return The action specified by the name, or INVALID.
	 */
	static Action lex(std::string const& commandName);

};


#endif

