#include <string>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Configuration.h"
#include "bcplus/parser/detail/lemon_macro.h"
#include "bcplus/parser/detail/lemon_parser.h"
#include "bcplus/parser/Token.h"
#include "bcplus/parser/detail/Scanner.h"
#include "bcplus/parser/detail/MacroParser.h"
#include "bcplus/symbols/SymbolTable.h"

namespace u = babb::utils;
namespace fs = boost::filesystem;

// parser functions
void*       lemon_macroAlloc(void* (*mallocProc)(size_t));
void*       lemon_macroFree(void* yyp, void (*freeProc)(void*));
void        lemon_macro(void* yyp, int tokentype, bcplus::parser::Token* token, bcplus::parser::detail::MacroParser* parser);
int         lemon_macroPreInject(void* yyp, int pop, bcplus::parser::Token** token);
void		lemon_macroAttemptReduce(void* yyp, bcplus::parser::detail::MacroParser* parser);

namespace bcplus {
namespace parser {
namespace detail {

MacroParser::MacroParser(Configuration const* config, languages::Language const* lang, symbols::SymbolTable* symtab)
	: _config(config), _lang(lang) {
	if (symtab) _symtab = symtab;
	else _symtab = new symbols::SymbolTable(config);
	_scanner = new detail::Scanner(config);
	_parser = lemon_macroAlloc(malloc);
	_stat = Status::OK;
}

MacroParser::~MacroParser() {
	lemon_macroFree(_parser, free);
}


Token const* MacroParser::next() {


	u::ref_ptr<Token> token;

	// pick up any statement that was generated simultaneously with an error (just in case)
	if (_tokens.size()) {
		token = _tokens.front();
		_tokens.pop_front();
	} else if (_stat == Status::END_INPUT) {
		token = new Token(T_EOF, new ReferencedString("<EOF>"), _scanner->loc(), _scanner->loc());
	} else {

		_stat = Status::OK;

		bool soft_eof = false;
		do {

			token = _scanner->readToken();
				
			int macro_type = token->macroType();
			switch (token->type()) {
			case T_EOF:
				// only fed it to the parser if it's a hard EOF (i.e. we already encoutnered a soft EOF)
				if (soft_eof) {
#ifndef NDEBUG
					_config->ostream(Verb::TRACE_MACRO_PARSER) << "TRACE: Macro parser hard EOF." << std::endl;
#endif
					_last_token = token;
					lemon_macro(_parser, macro_type, token.release(), this);
					_stat = Status::END_INPUT;
				} else {
#ifndef NDEBUG
					_config->ostream(Verb::TRACE_MACRO_PARSER) << "TRACE: Macro parser soft EOF." << std::endl;
#endif
					soft_eof = true;
					lemon_macroAttemptReduce(_parser, this);
				}
				break;
			case T_COMMENT:
				_comments.push_back(token);
				break;
			default:
				_last_token = token;
				lemon_macro(_parser, macro_type, token.release(), this);

				// if it was a close paren try to reduce the parser stack right away.
				// this is to avoid having the argument expansion process gum up the
				// scanner conditions.
				if (macro_type == M_PAREN_R) lemon_macroAttemptReduce(_parser, this);
				break;
			}
		} while (!_tokens.size() && _stat == Status::OK);


		if (_stat == Status::END_INPUT && !_tokens.size()) {
			token = new Token(T_EOF, new ReferencedString("<EOF>"), _scanner->loc(), _scanner->loc());
		} else if (_stat == Status::END_INPUT || _stat == Status::OK) {
			token = _tokens.front();
			_tokens.pop_front();
		} else {
			token = new Token(T_ERR_SYNTAX, new ReferencedString("<ERR_SYNTAX>"), _scanner->loc(), _scanner->loc());
		}
	}
#ifndef NDEBUG
	_config->ostream(Verb::TRACE_MACRO_PARSER) << "TRACE: " << token->beginLoc() << ": (" << token->typeString() << "): \"" << *token << "\": Read from macro parser." << std::endl;
#endif
	return token.release();
}

Token const* MacroParser::comment() {
	if (_comments.size()) {
		Token const* t = _comments.front().release();
		_comments.pop_front();
		return t;
	} else return NULL;
}


void MacroParser::reset() {
	_scanner = new detail::Scanner(_config);
	lemon_macroFree(_parser, free);
	_parser = lemon_macroAlloc(malloc);
	_stat = Status::OK;
	_tokens.clear();
}

bool MacroParser::push_back(fs::path const& file, bool squelch) {
	return _scanner->push_back(file, squelch);
}

void MacroParser::push_back(char const* buffer, Location const& loc, bool track) {
	_scanner->push_back(buffer, loc, track);
}

void MacroParser::push_back(Token* token) {
	_scanner->push_back(token);
}


void MacroParser::_feature_error(languages::Language::Feature::type feature, Location const* loc) {
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
	_stat = Status::ERR_SYNTAX;
}

void MacroParser::_parse_error(std::string const& error, Location const* loc) {
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
	_stat = Status::ERR_SYNTAX;
}

void MacroParser::_token(Token* t) {
	_tokens.push_back(t);
}

void MacroParser::_handle_include(statements::IncludeStatement const* stmt) {
#ifndef NDEBUG
	_config->ostream(Verb::DETAIL) << "TRACE: Got include statement!" << std::endl;
#endif

	Location l = stmt->beginLoc();
	
	BOOST_FOREACH(ReferencedString const* name, *stmt) {
		fs::path p = fs::path(*name);
		try {
			// Try to resolve the name
			fs::path fullpath;

			// try to resolve the path of the file

			if (p.is_absolute() || !l.file()) {
				fullpath = p;
			} else if (!l.file()) {
				fullpath = fs::current_path() / p;
			} else {
				// Try a path relative to the current file we're in first
				// Then try resolving with our default path.
				fullpath = l.file()->parent_path() / p;
				if (!fs::exists(fullpath) || fs::is_directory(fullpath)) {
					fullpath = fs::current_path() / p;
				}
			}

			if (!fs::exists(fullpath)) {
				// We were unable to resolve the file..
				std::ostream& out = _config->ostream(Verb::ERROR);
				out << "ERROR: " << l << ": Could not open file \"" << p.native() << "\". File does not exist." << std::endl;

				_stat = Status::ERR_SYNTAX;
				break;
			}

			if (fs::is_directory(fullpath)) {
				// We were unable to resolve the file..
				std::ostream& out = _config->ostream(Verb::ERROR);
				out << "ERROR: " << l << ": Could not open file \"" << p.native() << "\". The file is a directory." << std::endl;

				_stat = Status::ERR_SYNTAX;
				break;
				// We can't open a directory.
			}

			// The file appears to be good.
			if (!_push_front(fullpath, false)) {
				std::ostream& out = _config->ostream(Verb::ERROR);
				out << "ERROR: " << l << ": An error occurred openning file \"" << p.native() << "\"." << std::endl;
				_stat = Status::ERR_IO;
				break;
			}

		} catch (fs::filesystem_error& err) {
			std::ostream& out = _config->ostream(Verb::ERROR);
			out << "ERROR: " << l << ": An error occurred openning file \"" << p.native() << "\"." << std::endl;
			_stat = Status::ERR_IO;
			break;
		}
	}
}

bool MacroParser::_push_front(fs::path const& file, bool squelch) {
	preInjectPrep();
	return _scanner->push_front(file, squelch);
} 

void MacroParser::_push_front(char const* buffer, Location const& loc, bool track) {
	preInjectPrep();
	_scanner->push_front(buffer, loc, track);
}

void MacroParser::_push_front(Token* token) {
	preInjectPrep();
	_scanner->push_front(token);	
}

void MacroParser::preInjectPrep(bool pop_stack) {
	Token* tok = NULL;
	int t = lemon_macroPreInject(_parser, pop_stack, &tok);
	if (t) {
#ifndef NDEBUG
		_config->ostream(Verb::TRACE_MACRO_PARSER) << "TRACE: " << tok->beginLoc() << ": Retracted (" << tok->typeString() << ") \"" << *tok << "\"." << std::endl;
#endif
		_scanner->push_front(tok);
	} 
#ifndef NDEBUG
	else {
		_config->ostream(Verb::TRACE_MACRO_PARSER) << "TRACE: No token to retract from the parser." << std::endl;
	}
#endif
}

void MacroParser::_handle_macro(elements::Macro const* macro) {
	u::ref_ptr<const elements::Macro> macro_ptr = macro;
	// Expand it!
	_push_front(macro->expand()->c_str(), macro->beginLoc(), false);
}


}}}
