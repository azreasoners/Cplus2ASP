%include {
			#include <cassert>
			#include <cstring>

			#include "babb/utils/memory.h"
			#include "as2transition/Predicate.h"
			#include "as2transition/PredElement.h"
			#include "as2transition/parser/PredicateParser.h"



			#define UNUSED void*

			using namespace babb::utils;
			using namespace as2transition;
			using namespace as2transition::parser;

			// Helper for deallocating things
			#define DEALLOC(x)		{ if (x) delete x; x = NULL; }


		}

%name 								transition_parser
%token_prefix 						T_
%extra_argument 					{ PredicateParser* parser					}


%nonassoc IDENTIFIER.     			// Any identifier starting with a letter or
                          			// with underscore(s) and a letter/number, & optionally
                           			// followed by letters/numbers/underscores,
%nonassoc INTEGER.					// Any integral value
%nonassoc PAREN_L.         			// (
%nonassoc PAREN_R.        			// )
%nonassoc COMMA.		   			// ,
%nonassoc TILDE.			   		// ~
%nonassoc STRING_LITERAL.			// a string
%nonassoc EOF.

%nonassoc ERR_UNKNOWN_SYMBOL.		// placeholder for an unknown lexical symbol error
%nonassoc ERR_UNTERMINATED_STRING.	// placeholder for an unterminated string literal error


%token_type 						{ ReferencedString const*					}
%token_destructor 					{ DEALLOC($$);								}


%syntax_error 						{ parser->_parse_error("Syntax error.");	}


/*************************************************************************************************/
/* Start Symbol */
/*************************************************************************************************/

%type 		solution				{ UNUSED									}
%destructor solution				{ /* Intentionally left blank */			}
%type 		pred_list				{ UNUSED									}
%destructor pred_list				{ /* Intentionally left blank */			}
%type 		predicate				{ Predicate*								}
%destructor predicate				{ DEALLOC($$);								}
%type		obj_list				{ PredElement::ElementList*					}
%destructor obj_list				{ DEALLOC($$);								}
%type		object					{ PredElement*								}
%destructor object					{ DEALLOC($$);								}

solution ::= pred_list EOF.
pred_list ::= .
pred_list ::= pred_list predicate(P).											{ parser->_add(P); }
pred_list ::= pred_list COMMA predicate(P).										{ parser->_add(P); }
pred_list ::= pred_list error.


predicate(P) ::= IDENTIFIER(ID).												{ P = new Predicate(ID); }
predicate(P) ::= IDENTIFIER(ID) PAREN_L obj_list(OL) PAREN_R.					{ P = new Predicate(ID, OL); }

predicate(P) ::= TILDE IDENTIFIER(ID).											{ P = new Predicate(ID, NULL, true); }
predicate(P) ::= TILDE IDENTIFIER(ID) PAREN_L obj_list(OL) PAREN_R.				{ P = new Predicate(ID, OL, true); }

obj_list(OL) ::= object(O).														{ OL = new Predicate::PredElement::ElementList(); OL->push_back(O); }
obj_list(OL) ::= obj_list(L) COMMA object(O).									{ OL = L; OL->push_back(O); }

object(P) ::= IDENTIFIER(ID).													{ P = new PredElement(PredElement::Type::OBJECT, ID); }
object(P) ::= IDENTIFIER(ID) PAREN_L obj_list(OL) PAREN_R.						{ P = new PredElement(PredElement::Type::OBJECT, ID, OL); }
object(P) ::= INTEGER(I).														{ P = new PredElement(PredElement::Type::INTEGER, I); }
object(P) ::= STRING_LITERAL(S).												{ P = new PredElement(PredElement::Type::STRING_LITERAL, S); }
