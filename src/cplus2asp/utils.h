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


/*
 * utils.h
 *
 *  Created on: Jan 24, 2013
 *      Author: joseph
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <sstream>


namespace utils {

/// A list of possible exit codes...
enum ExitCodes
{
	EXT_CODE_GOOD = 0x00,
	EXT_CODE_BAD_ARGS = 0x80,
	EXT_CODE_TOOLCHAIN_TRANS_ERR = 0xC0,
	EXT_CODE_TOOLCHAIN_PREPROC_ERR = 0xC1,
	EXT_CODE_TOOLCHAIN_GROUNDER_ERR = 0xC2,
	EXT_CODE_TOOLCHAIN_SOLVER_ERR = 0xC3,
	EXT_CODE_TOOLCHAIN_POSTPROC_ERR = 0xC4,
	EXT_CODE_TOOLCHAIN_REACTIVE_BRIDGE_ERR = 0xC5,
	EXT_CODE_UNDEFINED_CONSTANTS = 0xA0,
	EXT_CODE_INVALID_STEP_VALUE = 0xA1,
	EXT_CODE_INVALID_QUERY = 0xA2
};

/**
 * Checks if a c-string represents an integer or not.
 * The whole string must count as an integer, no spaces or other nonsense allowed. Number may start with + or -.
 * @param str - The string to test.
 * @return True if str is an integer, false otherwise.
 */
bool isInteger(const char *str);

/**
 * Attempts to transform a string into a numeric representation of itself.
 * For numbers, fmt should be dec, hex, or oct.
 * @param val - A reference to the recipient of the generated number.
 * @param s - The string to convert.
 * @param fmt - An optional parameter that specifies what kind of numeric conversion to attempt. If not given, will default to std::dec.
 * @return True if s could be converted into a number using fmt, false if there was a problem performing the conversion.
 */
template <class T>
bool from_string(T& val, const std::string& s, std::ios_base& (*fmt)(std::ios_base&)=std::dec)
{
	std::istringstream iss(s);
	return !((iss >> fmt >> val).fail());
}

/**
 * Transforms something (usually a number) into a string representation of itself.
 * For integers, fmt should be dec, hex, or oct. For floating-point numbers, fmt should be fixed or scientific.
 * Precision will help control the precision of floating-point numbers.
 * @param val - The number to transform into a string.
 * @param fmt - An optional parameter controlling what kind of number val should be treated as. Defaults to std::dec.
 * @param precision - The decimal precision to use with floating-point numbers. Defaults to -1.
 * @return The string version of val on success, a blank string on failure.
 */
template <class T>
std::string to_string(T val, std::ios_base& (*fmt)(std::ios_base&)=std::dec, int precision=-1)
{
	std::stringstream ss;
	if(precision > -1)
	{
		ss.precision(precision);
	}
	ss << fmt << val;
	return ss.str();
}

/**
 * A wrapper for exit() that can output an optional custom message before leaving.
 * Outputs to stdout if exitCode is 0, stderr otherwise.
 * @param exitCode - The exit code to pass to exit.
 * @param message - A message to output to standard out before exiting (terminating newline automatically added).
 */
void nice_exit(int exitCode, const char* message);


/**
 * Trims whitespace from both ends of a text string.
 * @param text - The text to trim.
 * @return The string stripped of whitespace on both sides.
 */
std::string trimWhitespace(std::string const& text);

/**
 * Starting from position index, finds the first character in text that isn't whitespace, returns its position.
 * @param text - The string to search.
 * @param index - Optional parameter specifying starting index of the search. Defaults to 0.
 * @return The index of the first character in text that isn't whitespace, or string::npos if it's all whitespace.
 */
inline size_t findFirstNotWhitespace(std::string const& text, size_t index=0)
{
	return text.find_first_not_of(" \n\f\r\t\v", index);
}

/**
 * Starting from position index, finds the first character in text that is whitespace, returns its position.
 * @param text - The string to search through.
 * @param index - Optional parameter specifying starting index of the search. Defaults to 0.
 * @return The index of the first character in text that is whitespace, or string::npos if none of it is whitespace.
 */
inline size_t findFirstWhitespace(std::string const& text, size_t index=0)
{
	return text.find_first_of(" \n\f\r\t\v", index);
}

/**
 * Generates a pragma message and sends it to the user via stdout.
 * @param fmt The formatting string for the message.
 * @param newline Whether to include a newline after the message.
 * @param ... Additional parameters for the format.
 */
void pragma(char const* fmt, bool newline, ...);

/**
 * Generates a warning message and sends it to the user via stderr.
 * @param fmt The formatting string for the message.
 * @param newline Whether to include a newline after the message.
 * @param ... Additional parameters for the format.
 */
void warning(char const* fmt, bool newline, ...);

/**
 * Generates an error message and sends it to the user via stderr. Exits if the provided exit code is non-zero.
 * @param fmt The formatting string for the message.
 * @param newline Whether to include a newline after the message.
 * @param exitCode The code to exit with. An exit code of 0 will prevent the program from exiting.
 * @param ... Additional parameters for the format.
 */
void error(char const* fmt, bool newline, unsigned char exitCode, ...);

/**
 * Generates and displays a debug message if the binary was compiled in debug mode. Otherwise does nothing.
 * @param fmt The formatting string for the message.
 * @param newline Whether to include a newline after the message.
 * @param ... Additional parameters for the format.
 */

#ifdef DEBUG
void debug(char const* fmt, bool newline, ...);
#else
inline void debug(char const* fmt, bool newline, ...) { /* Intentionally left blank */ }
#endif
};

#endif /* UTILS_H_ */
