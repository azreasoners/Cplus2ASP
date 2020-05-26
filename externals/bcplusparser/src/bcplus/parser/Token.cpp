
#include "bcplus/parser/detail/lemon_parser.h"
#include "bcplus/parser/detail/lemon_macro.h"
#include "bcplus/parser/Token.h"

char const* lemon_parserTokenName(int tok);

namespace bcplus {
namespace parser {

Token::Token(int type, ReferencedString const* string, Location const& begin, Location const& end)
	: _type(type), _string(string), _begin(begin), _end(end) {
	// Inentionally left blank
}

Token::~Token() {
	// Intentionally left blank
}

Token* Token::copy() const {
	return new Token(type(), str(), beginLoc(), endLoc());
}

char const* Token::typeString(int token) {
	return lemon_parserTokenName(token);
}

int Token::macroType() const {
	switch (type()) {
		case T_COMMA:							return M_COMMA;
		case T_PAREN_L:							return M_PAREN_L;
		case T_PAREN_R:							return M_PAREN_R;
		case T_ARG:								return M_ARG;
		case T_EOF:								return M_EOF;
		case T_IDENTIFIER:						return M_IDENTIFIER;
		case T_INTEGER:							return M_INTEGER;
		case T_INCLUDE:							return M_INCLUDE;
		case T_COLON_DASH:						return M_COLON_DASH;
		case T_PERIOD:							return M_PERIOD;
		case T_SEMICOLON:						return M_SEMICOLON;
		case T_STRING_LITERAL:					return M_STRING_LITERAL;
		default:								return M_OTHER;
	}
}

std::ostream& Token::output(std::ostream& out) const {
	out << *str();
	return out;
}

}}





