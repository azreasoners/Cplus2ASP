#include <string>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Configuration.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/blocks.h"
#include "bcplus/parser/detail/lemon_parser.h"
#include "bcplus/parser/BCParser.h"
#include "bcplus/parser/Token.h"
#include "bcplus/parser/detail/Scanner.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/SymbolTable.h"
#include "bcplus/symbols/NumberRangeSymbol.h"

namespace u = babb::utils;
namespace fs = boost::filesystem;

// parser functions
void*       lemon_parserAlloc(void* (*mallocProc)(size_t));
void*       lemon_parserFree(void* yyp, void (*freeProc)(void*));
void        lemon_parser(void* yyp, int tokentype, bcplus::parser::Token const* token, bcplus::parser::BCParser* parser);
int         lemon_parserPreInject(void* yyp, int pop, bcplus::parser::Token const** token);
void		lemon_parserAttemptReduce(void* yyp, bcplus::parser::BCParser* parser);
#ifndef NDEBUG
void 		lemon_parserTrace(FILE *TraceFILE, char const*zTracePrompt);
#endif
namespace bcplus {
namespace parser {



BCParser::BCParser(Configuration const* config, languages::Language const* lang, symbols::SymbolTable* symtab)
	: _config(config), _lang(lang), _rangeCount(0) {
	if (symtab) _symtab = symtab;
	else _symtab = new symbols::SymbolTable(config);
	_scanner = new detail::MacroParser(config, lang, _symtab);
	_parser = lemon_parserAlloc(malloc);
	_stat = Status::OK;

}

BCParser::~BCParser() {
	lemon_parserFree(_parser, free);
}

statements::Statement* BCParser::makeCommentStmt() {
	u::ref_ptr<const Token> token = _scanner->comment();
	if (token != NULL) {
		u::ref_ptr<statements::CommentBlock::ElementList> l = new statements::CommentBlock::ElementList();

		do {
			l->push_back(token);
	
			token = _scanner->comment();
		} while (token != NULL);

		return new statements::CommentBlock(l, l->front()->beginLoc(), l->back()->endLoc());
	} else return NULL;
}

BCParser::ParseType BCParser::parse() {
	u::ref_ptr<const Token> token;
	u::ref_ptr<statements::Statement> stmt;


	
	// output any comments
	if (stmt = makeCommentStmt()) return ParseType(Status::OK, stmt);

	// output existing statement
	if (_stmt) {
		stmt = _stmt;
		_stmt = NULL;
		return ParseType(Status::OK, stmt);
	}

	// if we're at the end of input don't bother progressing
	if (_stat == Status::END_INPUT) {
		return ParseType(Status::END_INPUT, NULL);
	}


	_stat = Status::OK;
	
#ifndef NDEBUG
	if (_config->parseTrace()) 
		lemon_parserTrace(stdout, "TRACE: Parser: ");
#endif
	// parse a new statement
	do {
		token = _scanner->next();

		int type = token->type();

		// track the soft end of file flag
		if (type != T_EOF) _soft_eof = false;

		switch (type) {

		case T_ERR_IO:
			// an IO error occurred
			_parse_error("An IO error occurred reading from \"" + token->beginLoc().filename() + "\".", &token->beginLoc());
			break;

		case T_ERR_UNTERMINATED_STRING:
			_parse_error("Unexpected end of input. Unterminated string.", &token->beginLoc());
			break;
		
		case T_ERR_UNTERMINATED_ASP:
			_parse_error("Unexpected end of input. Unterminated ASP code block.", &token->beginLoc());
			break;
		
		case T_ERR_UNTERMINATED_LUA:
			_parse_error("Unexpected end of input. Unterminated LUA code block.", &token->beginLoc());
			break;

		case T_ERR_UNTERMINATED_PYTHON:
			_parse_error("Unexpected end of input. Unterminated Python code block.", &token->beginLoc());
			break;
		
		case T_ERR_UNTERMINATED_F2LP:
			_parse_error("Unexpected end of input. Unterminated F2LP code block.", &token->beginLoc());
			break;
		
		case T_ERR_UNTERMINATED_BLK_COMMENT:
			_parse_error("Unexpected end of input. Unterminated block comment.", &token->beginLoc());
			break;

		case T_ERR_SYNTAX:
			_parse_error("Syntax error.", &token->beginLoc());
			break;

		case T_ERR_UNKNOWN_SYMBOL:
			_parse_error("Unknown symbol \"" + *token->str() + "\".", &token->beginLoc());
			break;

		case T_ERR_PAREN_MISMATCH:
			_parse_error("Detected umatched open parentheses.", &token->beginLoc());
			break;

		case T_EOF:
#ifndef NDEBUG
			_config->ostream(Verb::TRACE_PARSER) << "TRACE: Hard EOF." << std::endl;
#endif
			_last_token = token;
			lemon_parser(_parser, type, token.release(), this);
			lemon_parserAttemptReduce(_parser, this);
			_stat = Status::END_INPUT;
			break;

		default:
			_last_token = token;
			lemon_parser(_parser, type, token.release(), this);

			// This forces this parser to reduce each previous statement before
			// looking ahead. This allows the MacroParser to perform just-in-time
			// identifier binding to disambiguate our grammer at this level.
			if (type == T_PERIOD) {
				lemon_parserAttemptReduce(_parser, this);
			}

			break;
		} 
	} while (!_stmt && _stat == Status::OK);
	
#ifndef NDEBUG
	if (_config->parseTrace()) 
		lemon_parserTrace(NULL, NULL);
#endif	

	// Figure out exactly why we stopped
	if (_stat != Status::OK && _stat != Status::END_INPUT) {
		return ParseType(_stat, NULL);
//	} else if (stmt = makeCommentStmt()) {
//		return ParseType(Status::OK, stmt);
	} else {
		stmt = _stmt;
		_stmt = NULL;
		return ParseType(Status::OK, stmt);
	}
	
}


void BCParser::reset() {
	_scanner->reset();
	lemon_parserFree(_parser, free);
	_parser = lemon_parserAlloc(malloc);
	_stat = Status::OK;
	_stmt = NULL;	
	_soft_eof = false;
}



bool BCParser::push_back(fs::path const& file, bool squelch) {
	return _scanner->push_back(file, squelch);
}

void BCParser::push_back(char const* buffer, Location const& loc) {
	_scanner->push_back(buffer, loc);
}

void BCParser::_feature_error(languages::Language::Feature::type feature, Location const* loc) {
	std::ostream& out = _config->ostream(Verb::ERROR);
	out << "ERROR: ";
	if (loc) {
		out << *loc << ": ";
	} else {
		Token const* t = _last_token;
		if (t) {
			out << t->beginLoc() << ": \"" << *t << "\": ";
		}
	}
	out << lang()->featureDescription(feature) << " are not supported by language " << lang()->name() << "." << std::endl;
	_stat = Status::SYNTAX_ERR;
}

void BCParser::_parse_error(std::string const& error, Location const* loc) {
	std::ostream& out = _config->ostream(Verb::ERROR);
	out << "ERROR: ";
	if (loc) {
		out << *loc << ": ";
	} else {
		Token const* t = _last_token;
		if (t) {
			out << t->beginLoc() << ": \"" << *t << "\": ";
		}
	}
	out << error << std::endl;
	_stat = Status::SYNTAX_ERR;
}

void BCParser::_handle_stmt(statements::Statement* stmt) {
#ifndef NDEBUG
	_config->ostream(Verb::TRACE_PARSER) << "TRACE: Got statement of type \"" << stmt->typeString() << "\"." << std::endl;
#endif

	_stmt = stmt;
}

symbols::SortSymbol* BCParser::_newRange(elements::Term* min, elements::Term* max) {
	u::ref_ptr<elements::Term> minptr = min, maxptr = max;

	// create a new number range object
	u::ref_ptr<const symbols::NumberRangeSymbol> rsym = _newRangeSymbol(min,max);
	if (!rsym) return NULL;

	// create a new sort
	u::ref_ptr<symbols::SortSymbol> s = new symbols::SortSymbol(new ReferencedString("_rsort_" + boost::lexical_cast<std::string>(_rangeCount)));
	if (!symtab()->create(s)) {
		return NULL;
	}


	// add the range
	s->add(rsym);
	
	return s.release();
}

symbols::NumberRangeSymbol* BCParser::_newRangeSymbol(elements::Term* min, elements::Term* max) {
	u::ref_ptr<elements::Term> minptr = min, maxptr = max;

	// create a new number range object
	u::ref_ptr<symbols::NumberRangeSymbol> rsym = symtab()->resolveOrCreate(
		new symbols::NumberRangeSymbol(
			new ReferencedString("_range_" + boost::lexical_cast<std::string>(++_rangeCount)),
			min, max));

	return rsym.release();
}


}}
