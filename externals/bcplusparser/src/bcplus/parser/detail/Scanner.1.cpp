/* Generated by re2c 0.13.5 on Thu Dec 17 18:02:06 2015 */
#line 1 "bcplus/parser/detail/Scanner.1.re2c"

#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "bcplus/parser/detail/Scanner.h"
#include "bcplus/parser/detail/lemon_parser.h"

#include "bcplus/Location.h"
#include "bcplus/parser/Token.h"

#include "bcplus/parser/detail/ScannerSource.h"
#include "bcplus/parser/detail/ScannerRawSource.h"
#include "bcplus/parser/detail/ScannerTokenSource.h"
#include "bcplus/parser/detail/ScannerFileSource.h"
#include "bcplus/parser/detail/ScannerBufferSource.h"

namespace u = babb::utils; 
namespace fs = boost::filesystem;

namespace bcplus {
namespace parser {
namespace detail {

Scanner::Scanner(Configuration const* config)
	: _cond(yycNORMAL), _config(config), _in_macro(false) {
	// Intentionally left blank
}

Scanner::~Scanner() {
	// Intentionally left blank
}

bool Scanner::push_front(fs::path const& file, bool squelch) {
	u::ref_ptr<ScannerFileSource> src = new ScannerFileSource(_config, file, squelch);
	if (!src->good()) return false;
	_sources.push_front(src.get());
	return true;
}

bool Scanner::push_back(fs::path const& file, bool squelch) {
	u::ref_ptr<ScannerFileSource> src = new ScannerFileSource(_config, file, squelch);
	if (!src->good()) return false;
	_sources.push_back(src.get());
	return true;
}

void Scanner::push_front(char const* buffer, Location const& loc, bool track_position) {
	u::ref_ptr<ScannerBufferSource> src = new ScannerBufferSource(_config, buffer, loc, track_position);
	_sources.push_front(src.get());
}

void Scanner::push_back(char const* buffer, Location const& loc, bool track_position) {
	u::ref_ptr<ScannerBufferSource> src = new ScannerBufferSource(_config, buffer, loc, track_position);
	_sources.push_back(src.get());
}

void Scanner::push_back(Token* token) {
	if (!_sources.size() || _sources.back()->type() != ScannerSource::Type::TOKENS) {
		_sources.push_back(new ScannerTokenSource(token));
	} else ((ScannerTokenSource*)_sources.back().get())->push_back(token);
}

void Scanner::push_front(Token* token) {
	if (!_sources.size() || _sources.front()->type() != ScannerSource::Type::TOKENS) {
		_sources.push_front(new ScannerTokenSource(token));
	} else ((ScannerTokenSource*)_sources.front().get())->push_front(token);
}

void Scanner::push_back(TokenList* tokens) {
	if (!_sources.size() || _sources.back()->type() != ScannerSource::Type::TOKENS) {
		_sources.push_back(new ScannerTokenSource(tokens));
	} else ((ScannerTokenSource*)_sources.back().get())->push_back(tokens);
}

void Scanner::push_front(TokenList* tokens) {
	if (!_sources.size() || _sources.front()->type() != ScannerSource::Type::TOKENS) {
		_sources.push_front(new ScannerTokenSource(tokens));
	} else ((ScannerTokenSource*)_sources.front().get())->push_front(tokens);
}


Token* Scanner::readToken() {
	u::ref_ptr<Token> token = NULL;
	if (!size()) {
		Location l = loc();
		token  = new Token(T_EOF, new ReferencedString("<EOF>"), l, l);
	} else {
		do {
			ScannerSource* src = _sources.front();
#ifndef NDEBUG
			_config->ostream(Verb::TRACE_SCANNER) << "TRACE: " << size() << " sources, reading from \"" 
				<< (src->loc().file() ? src->loc().file()->native() : std::string("<NULL>")) << "\"." << std::endl;
#endif
			switch(src->type()) {
			case ScannerSource::Type::RAW:
				token = tokenize((ScannerRawSource*)src);
				break;
			case ScannerSource::Type::TOKENS:
				token = ((ScannerTokenSource*)src)->next();
				break;
			}
	
			if (!token || token->type() == T_ERR_IO) {
#ifndef NDEBUG
				_config->ostream(Verb::TRACE_SCANNER) << "TRACE: Scanner encountered IO error." << std::endl;
#endif
				_sources.pop_front();
			} else if (token->type() == T_EOF) {
#ifndef NDEBUG
				_config->ostream(Verb::TRACE_SCANNER) << "TRACE: Scanner encountered end of input source." << std::endl;
#endif
				_sources.pop_front();
			}
		} while (token && token->type() == T_EOF && size());
	}
	
	if (!token) {
		Location l = loc();
#ifndef NDEBUG
		_config->ostream(Verb::TRACE_SCANNER) << "TRACE: Scanner got a null token." << std::endl;
#endif
		token  = new Token(T_ERR_IO, new ReferencedString("<IO ERROR>"), l, l);
	}

#ifndef NDEBUG
	_config->ostream(Verb::TRACE_SCANNER) << "TRACE: " << token->beginLoc() << ": (" << token->typeString() << "): \"" << *token << "\": Read from scanner." << std::endl;
#endif

	return token.release();
}

#define BLK_INIT \
	blk.str(""); \
	use_blk = true; \
	source->token() = source->cursor();

#define BLK_LINE \
	source->newline(); \
    blk.write(source->token(), (size_t)(source->cursor() - source->token())); \
	source->token() = source->cursor();		

#define BLK_FINISH(sz) \
	blk.write(source->token(), (size_t)(source->cursor() - source->token() - sz));

Token* Scanner::tokenize(ScannerRawSource* source) {
	if (source->eof()) return new Token(T_EOF, new ReferencedString("<EOF>"), source->loc(), source->loc());
	else if (!source->good()) return new Token(T_ERR_IO, new ReferencedString("<IO ERROR>"), source->loc(), source->loc());


	std::stringstream blk;
	int ret;
	Location begin;
	bool use_blk = false;
	size_t preskip = 0;
	size_t postskip = 0;

re2c_start:
	source->token() = source->cursor();
	begin = source->loc();

	if (_cond == yycNORMAL) {
	
#line 168 "<stdout>"
{
	char yych;
	if (_cond < 7) {
		if (_cond < 3) {
			if (_cond < 1) {
				goto yyc_NORMAL;
			} else {
				if (_cond < 2) {
					goto yyc_ARG;
				} else {
					goto yyc_ASP_CP;
				}
			}
		} else {
			if (_cond < 5) {
				if (_cond < 4) {
					goto yyc_ASP_GR;
				} else {
					goto yyc_DBL_STRING;
				}
			} else {
				if (_cond < 6) {
					goto yyc_F2LP_CP;
				} else {
					goto yyc_F2LP_GR;
				}
			}
		}
	} else {
		if (_cond < 10) {
			if (_cond < 8) {
				goto yyc_LUA_CP;
			} else {
				if (_cond < 9) {
					goto yyc_LUA_GR;
				} else {
					goto yyc_MACO;
				}
			}
		} else {
			if (_cond < 12) {
				if (_cond < 11) {
					goto yyc_PRE_ARG;
				} else {
					goto yyc_SG_STRING;
				}
			} else {
				if (_cond < 13) {
					goto yyc_COMMENT;
				} else {
					goto yyc_BLK_COMMENT;
				}
			}
		}
	}
/* *********************************** */
yyc_ARG:

	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_3;
yy1_3:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 236 "<stdout>"
/* *********************************** */
yyc_ASP_CP:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_8;
yy1_8:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 248 "<stdout>"
/* *********************************** */
yyc_ASP_GR:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_13;
yy1_13:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 260 "<stdout>"
/* *********************************** */
yyc_BLK_COMMENT:
	if ((source->limit() - source->cursor()) < 2) source->fill(2);
	yych = *source->cursor();
	if (yych <= '\n') {
		if (yych <= 0x00) goto yy1_17;
		if (yych <= '\t') goto yy1_19;
		goto yy1_22;
	} else {
		if (yych == '*') goto yy1_21;
		goto yy1_19;
	}
yy1_17:
	++source->cursor();
#line 222 "bcplus/parser/detail/Scanner.1.re2c"
	{ 
												  BLK_FINISH(1);
												  ret = T_ERR_UNTERMINATED_BLK_COMMENT;			
												  _cond = yycNORMAL;
												  source->cursor()--;
												  goto re2c_finish;					
												}
#line 283 "<stdout>"
yy1_19:
	++source->cursor();
yy1_20:
#line 229 "bcplus/parser/detail/Scanner.1.re2c"
	{												goto yyc_BLK_COMMENT;				}
#line 289 "<stdout>"
yy1_21:
	yych = *++source->cursor();
	if (yych == '/') goto yy1_24;
	goto yy1_20;
yy1_22:
	++source->cursor();
#line 211 "bcplus/parser/detail/Scanner.1.re2c"
	{ 
												  BLK_LINE;
												  blk << "% "; 
												  goto yyc_BLK_COMMENT;				
												}
#line 302 "<stdout>"
yy1_24:
	++source->cursor();
#line 216 "bcplus/parser/detail/Scanner.1.re2c"
	{ 
												  BLK_FINISH(2);
												  _cond = yycNORMAL; 
												  ret = T_COMMENT; 
												  goto re2c_finish;					
												}
#line 312 "<stdout>"
/* *********************************** */
yyc_COMMENT:
	{
		static const unsigned char yybm[] = {
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128,   0, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
			128, 128, 128, 128, 128, 128, 128, 128, 
		};
		if ((source->limit() - source->cursor()) < 2) source->fill(2);
		yych = *source->cursor();
		if (yych <= 0x00) goto yy1_28;
		if (yych == '\n') goto yy1_31;
		goto yy1_30;
yy1_28:
		++source->cursor();
		yych = *source->cursor();
		goto yy1_34;
yy1_29:
#line 203 "bcplus/parser/detail/Scanner.1.re2c"
		{ 												goto yyc_COMMENT;					}
#line 362 "<stdout>"
yy1_30:
		yych = *++source->cursor();
		goto yy1_34;
yy1_31:
		++source->cursor();
#line 197 "bcplus/parser/detail/Scanner.1.re2c"
		{ 
												  source->cursor()--; 
												  _cond = yycNORMAL; 
												  ret = T_COMMENT;			
												  goto re2c_finish;
												}
#line 375 "<stdout>"
yy1_33:
		++source->cursor();
		if (source->limit() <= source->cursor()) source->fill(1);
		yych = *source->cursor();
yy1_34:
		if (yybm[0+yych] & 128) {
			goto yy1_33;
		}
		goto yy1_29;
	}
/* *********************************** */
yyc_DBL_STRING:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_38;
yy1_38:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 397 "<stdout>"
/* *********************************** */
yyc_F2LP_CP:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_43;
yy1_43:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 409 "<stdout>"
/* *********************************** */
yyc_F2LP_GR:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_48;
yy1_48:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 421 "<stdout>"
/* *********************************** */
yyc_LUA_CP:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_53;
yy1_53:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 433 "<stdout>"
/* *********************************** */
yyc_LUA_GR:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_58;
yy1_58:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 445 "<stdout>"
/* *********************************** */
yyc_MACO:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_63;
yy1_63:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 457 "<stdout>"
/* *********************************** */
yyc_NORMAL:
	if ((source->limit() - source->cursor()) < 2) source->fill(2);
	yych = *source->cursor();
	if (yych <= 0x1F) {
		if (yych <= '\t') {
			if (yych <= 0x00) goto yy1_67;
			if (yych <= 0x08) goto yy1_69;
			goto yy1_71;
		} else {
			if (yych <= '\n') goto yy1_76;
			if (yych <= '\f') goto yy1_71;
			goto yy1_69;
		}
	} else {
		if (yych <= '%') {
			if (yych <= ' ') goto yy1_71;
			if (yych <= '$') goto yy1_69;
			goto yy1_73;
		} else {
			if (yych == '/') goto yy1_75;
			goto yy1_69;
		}
	}
yy1_67:
	++source->cursor();
#line 182 "bcplus/parser/detail/Scanner.1.re2c"
	{ ret = T_EOF; 							goto re2c_finish; 					}
#line 486 "<stdout>"
yy1_69:
	++source->cursor();
yy1_70:
#line 231 "bcplus/parser/detail/Scanner.1.re2c"
	{ goto re2c_notcmt; }
#line 492 "<stdout>"
yy1_71:
	++source->cursor();
#line 181 "bcplus/parser/detail/Scanner.1.re2c"
	{ 										goto re2c_start; 					}
#line 497 "<stdout>"
yy1_73:
	++source->cursor();
#line 184 "bcplus/parser/detail/Scanner.1.re2c"
	{ _cond = yycCOMMENT; 							goto yyc_COMMENT;	 				}
#line 502 "<stdout>"
yy1_75:
	yych = *++source->cursor();
	if (yych == '*') goto yy1_78;
	goto yy1_70;
yy1_76:
	++source->cursor();
#line 180 "bcplus/parser/detail/Scanner.1.re2c"
	{ source->newline();		 			goto re2c_start; 					}
#line 511 "<stdout>"
yy1_78:
	++source->cursor();
#line 185 "bcplus/parser/detail/Scanner.1.re2c"
	{ 
												  _cond = yycBLK_COMMENT; 
												  BLK_INIT;
												  blk << "% "; 
												  goto yyc_BLK_COMMENT; 				
												}
#line 521 "<stdout>"
/* *********************************** */
yyc_PRE_ARG:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_83;
yy1_83:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 533 "<stdout>"
/* *********************************** */
yyc_SG_STRING:
	if (source->limit() <= source->cursor()) source->fill(1);
	yych = *source->cursor();
	if (yych != '\n') goto yy1_88;
yy1_88:
	++source->cursor();
#line 193 "bcplus/parser/detail/Scanner.1.re2c"
	{
												  goto re2c_notcmt;
												}
#line 545 "<stdout>"
}
#line 232 "bcplus/parser/detail/Scanner.1.re2c"

	}
re2c_notcmt:
	source->cursor() = source->token();
	return tokenize_2(source);

re2c_finish:
	u::ref_ptr<ReferencedString> str = new ReferencedString();
	if (ret != T_EOF) {
		if (use_blk)
			str->assign(blk.str());
		else
			str->assign(source->token() + preskip, (size_t)(source->cursor() - source->token() - preskip - postskip));
	} else {
		str->assign("<EOF>");
	}

	return new Token(ret, str, begin, source->loc());
}

void Scanner::close() {
	_sources.clear();
}



}}}