#include <string>
#include <iostream>

#include "babb/utils/memory.h"
#include "memwrappers.h"
#include "as2transition/parser/lemon_parser.h"
#include "as2transition/parser/PredicateParser.h"


namespace u = babb::utils;

// parser functions
void*       transition_parserAlloc(void* (*mallocProc)(size_t));
void*       transition_parserFree(void* yyp, void (*freeProc)(void*));
void        transition_parser(void* yyp, int tokentype, ReferencedString const* token, as2transition::parser::PredicateParser* parser);
int         transition_parserPreInject(void* yyp, int pop, ReferencedString const** token);
void		transition_parserAttemptReduce(void* yyp, as2transition::parser::PredicateParser* parser);
#ifndef NDEBUG
void 		transition_parserTrace(FILE *TraceFILE, char const*zTracePrompt);
char const*	transition_parserTokenName(int tcode);
#endif

namespace as2transition {
namespace parser {



PredicateParser::PredicateParser(std::istream& input, bool stopnewline)
	: _stat(Status::READY) {
	_scanner = new Scanner(input, stopnewline);
	_parser = transition_parserAlloc(malloc);
}

PredicateParser::~PredicateParser() {
	transition_parserFree(_parser, free);
}


PredicateParser::Status::type PredicateParser::parse() {
	ReferencedString* token;
	int tcode;

	if (_stat == Status::FINISHED) return _stat;


	// parse the list...
	do {
		tcode = _scanner->readToken(token);
		_last_token = *token;
//		std::cout << "Got token \"" << *token << "\" of type \"" << transition_parserTokenName(tcode) << "\"." << std::endl;
		

		switch (tcode) {

		case T_ERR_UNTERMINATED_STRING:
			_parse_error("Unexpected end of input. Unterminated string.");
			break;
		
		case T_EOF:
			transition_parser(_parser, tcode, token, this);
			transition_parserAttemptReduce(_parser, this);
			_stat = Status::FINISHED;
			break;
		case T_ERR_UNKNOWN_SYMBOL:
			_parse_error("Encountered an unexpected symbol \"" + *token + "\".");

		default:
			transition_parser(_parser, tcode, token, this);
			break;
		} 
	} while (_stat == Status::READY);

	return _stat;
}



void PredicateParser::_parse_error(std::string const& error) {
	std::cerr << "ERROR: \"" << error << "\"";
	if (_last_token.size()) std::cerr << " while parsing token \"" << _last_token << "\".";
	std::cerr << std::endl;
	_stat = Status::SYNTAX_ERR;
}

}}
