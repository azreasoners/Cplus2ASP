#include <fstream>
#include <streambuf>
#include <string>
#include <cerrno>
#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/program_options.hpp>

// #define DEBUG 0
#define COMMENT "%"
#define PAREN_OPEN "("
#define PAREN_CLOSE ")"
#define COMMA ","
#define COLON_DASH ":-" 
#define DOT "."
#define DOTCHAR '.'
#define WHITESPACE "\t \n"
#define HASHTAG "#"
#define NEWLINE "\n"
#define OUTPUT_EXTN ".cl\0"

#define HIDE "#hide"
#define ABS "#abs"
#define SUM "#sum"
#define PIPE "|"

// Constants for incremental parts of the program

#define QUERY "query"
#define IBASE "#base"
#define ICUMULATIVE "#cumulative"
#define IVOLATILE "#volatile"
#define PRGBASE "#program base"
#define PRGCUMULATIVE "#program step"
#define PRGVOLATILE "#program check"
#define PRGQUERY "query"


namespace ba = boost::algorithm;