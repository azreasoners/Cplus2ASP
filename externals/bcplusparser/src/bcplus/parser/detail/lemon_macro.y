%include {
			#include <cassert>
			#include <cstring>

			#include "babb/utils/memory.h"

			#include "bcplus/parser/Token.h"
			#include "bcplus/parser/detail/MacroParser.h"
			#include "bcplus/parser/detail/lemon_macro.h"
			#include "bcplus/parser/detail/lemon_parser.h"
			#include "bcplus/elements/Macro.h"
			#include "bcplus/statements/Statement.h"
			#include "bcplus/statements/declarations.h"
			#include "bcplus/symbols/Symbol.h"
			#include "bcplus/symbols/MacroSymbol.h"
			#define UNUSED void*

			using namespace babb::utils;
			using namespace bcplus::parser;
			using namespace bcplus::parser::detail;
			using namespace bcplus::statements;
			using namespace bcplus::elements;
			using namespace bcplus::languages;
			using namespace bcplus::symbols;

			// Helper for deallocating things
			#define DEALLOC(x)		{ if (x) delete x; x = NULL; }


		}

%name 								lemon_macro
%token_prefix 						M_
%extra_argument 					{ MacroParser* parser						}


%nonassoc IDENTIFIER.     			// Any identifier starting with a letter or
                          			// with underscore(s) and a letter/number, & optionally
                           			// followed by letters/numbers/underscores,
%nonassoc INTEGER.					// Any integral value
%nonassoc PAREN_L.         			// (
%nonassoc PAREN_R.        			// )
%nonassoc COMMA.		   			// ,
%nonassoc INCLUDE.					// "include"
%nonassoc COLON_DASH.				// ":-"
%nonassoc PERIOD.					// "."
%nonassoc SEMICOLON.				// ";"
%nonassoc STRING_LITERAL.			// a string
%nonassoc ARG.             			// argument placeholder
%nonassoc OTHER.		   			// anything else
%nonassoc EOF.


%token_type 						{ Token*	 								}
%token_destructor 					{ DEALLOC($$);								}



%syntax_error 						{ parser->_parse_error("Syntax error.");	}


/*************************************************************************************************/
/* Start Symbol */
/*************************************************************************************************/

%type 		start					{ UNUSED									}			// start symbol for file parsing
%destructor start					{ /* Intentionally left blank */			}
%type 		macro_lst				{ UNUSED									}			// list of BC+ statements
%destructor macro_lst				{ /* Intentionally left blank */			}
%type		incl_stmt				{ UNUSED									}
%destructor incl_stmt				{ /* Intentionally left blank */			}
%type		base_elem_pkgd			{ UNUSED									}			// pre-declared base elemtn
%destructor base_elem_pkgd			{ /* Intentionally left blank */			}
%type		other					{ UNUSED									}
%destructor other					{ /* Intentionally left blank */			}

start ::= macro_lst EOF.

macro_lst ::= .
macro_lst ::= macro_lst error.
macro_lst ::= macro_lst base_elem_pkgd.
macro_lst ::= macro_lst other.
macro_lst ::= macro_lst incl_stmt.

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

/*************************************************************************************************/
/* Include Statements */
/*************************************************************************************************/

%type       incl_lst                { IncludeStatement::ElementList*           }           // semicolon or comma separated file list
%destructor incl_lst                { DEALLOC($$);                              }			
%type       incl_lst_sc             { IncludeStatement::ElementList*           }           // semi-colon separated file list: '<file>;...;<file>.'
%destructor incl_lst_sc             { DEALLOC($$);                              }			
%type       incl_lst_c              { IncludeStatement::ElementList*           }           // comma separated file list: '<file>,...,<file>.'
%destructor incl_lst_c              { DEALLOC($$);                              }			
%type       incl_item               { Token const*                              }           // A single file name
%destructor incl_item               { DEALLOC($$);                              }

incl_stmt ::= COLON_DASH(cd) INCLUDE incl_lst(lst) PERIOD(p).
        {
	        ref_ptr<IncludeStatement::ElementList> lst_ptr = lst;
            
			if (!parser->lang()->support(Language::Feature::DECL_INCLUDE)) {
                parser->_feature_error(Language::Feature::DECL_INCLUDE, &cd->beginLoc());
                YYERROR;
            } else {
	        	ref_ptr<IncludeStatement> stmt = new IncludeStatement(lst, cd->beginLoc(), p->endLoc() );
				parser->_handle_include(stmt);
			}

            delete cd;
            delete p;

        }

incl_lst(new_lst) ::= incl_lst_sc(lst).
		{
			new_lst = lst;
		}

incl_lst(new_lst) ::= incl_lst_c(lst).
		{
			new_lst = lst;
		}

incl_lst_sc(lst) ::= incl_item(item).
        {
            lst = new IncludeStatement::ElementList();
            lst->push_back(item->str());
            delete item;
        }

incl_lst_sc(new_lst) ::= incl_lst_sc(lst) SEMICOLON incl_item(item).
        {
            new_lst = lst;
            new_lst->push_back(item->str());
            delete item;
        }

incl_lst_c(lst) ::= incl_item(item) COMMA incl_item(item2).
        {
            lst = new IncludeStatement::ElementList();
            lst->push_back(item->str());
            lst->push_back(item2->str());
            delete item;
            delete item2;
        }

incl_lst_c(new_lst) ::= incl_lst_c(lst) COMMA incl_item(item).
        {
            new_lst = lst;
            new_lst->push_back(item->str());
            delete item;
        }

incl_item(item) ::= STRING_LITERAL(str).
        {
            item = str;
        }
incl_item(item) ::= IDENTIFIER(id).
        {
            item = id;
        }
incl_item(item) ::= INTEGER(i).
        {
            item = i;
        }


/*************************************************************************************************/
/* Base Elements */
/*************************************************************************************************/

%include {
	typedef ReferencedList<ref_ptr<Token> >::type TokenList;
	typedef ReferencedWrapper<std::pair<ref_ptr<TokenList>, ref_ptr<Macro::ArgumentList> > > CommaArgumentList;
}

%type		arg_lst					{ CommaArgumentList*												}			// list of packaged arguments which haven't been parsed yet (with included commas)
%destructor arg_lst					{ DEALLOC($$);														}

base_elem_pkgd ::= IDENTIFIER(id).
		{
			ref_ptr<const Token> id_ptr = id;
			Symbol const* sym = parser->symtab()->resolve(Symbol::Type::M_TERM, *id->str());
			if (sym && sym->type() == Symbol::Type::MACRO) {
				if (parser->lang()->support(Language::Feature::MACRO)) {
					ref_ptr<Macro> elem = new Macro((MacroSymbol*)sym, NULL, id->beginLoc(), id->endLoc());
					parser->_handle_macro(elem);
				} else {
					parser->_feature_error(Language::Feature::MACRO, &id->beginLoc());
					YYERROR;
				}
			} else {
				// Change the identifier token depending on what type of symbol it is (if any)
				if (sym) {
					switch (sym->type()) {
					case Symbol::Type::CONSTANT:
						id->type(T_CONSTANT_ID);
						break;
					case Symbol::Type::OBJECT:
						id->type(T_OBJECT_ID);
						break;
					case Symbol::Type::VARIABLE:
						id->type(T_VARIABLE_ID);
						break;
					default:
						break;
					}
				}	
				parser->_token(id);
			}
		}

base_elem_pkgd ::= IDENTIFIER(id) PAREN_L(pl) arg_lst(args) PAREN_R(pr).
		{
			// this has a packed argument list to allow for macro expansion.
			ref_ptr<const Token> id_ptr = id;
			ref_ptr<const Token> pl_ptr = pl;
			ref_ptr<CommaArgumentList> args_ptr = args;
			ref_ptr<const Token> pr_ptr = pr;

			Symbol const* sym = parser->symtab()->resolve(Symbol::Type::M_TERM, *id->str(), args->second->size());
			if (sym && sym->type() == Symbol::Type::MACRO) {
				
				if (parser->lang()->support(Language::Feature::MACRO)) {
					ref_ptr<Macro> elem = new Macro((MacroSymbol*)sym, args->second, id->beginLoc(), pr->endLoc());
					parser->_handle_macro(elem);
				} else {
					parser->_feature_error(Language::Feature::MACRO, &id->beginLoc());
					YYERROR;
				}

			} else {	
				// It's not a macro. Just go ahead and unpack it and re-add it for reading
				parser->_push_front(pr);
				TokenList::reverse_iterator commas = args->first->rbegin();
				for (Macro::ArgumentList::reverse_iterator it = args->second->rbegin(); it != args->second->rend(); ) {
					parser->_push_front((*it)->str()->c_str(), (*it)->beginLoc(), true);

					it++;
					if (it != args->second->rend()) {
						parser->_push_front((*commas));
						commas++;
					}
				}

				// Change the identifier token depending on what type of symbol it is (if any)
				if (sym) {
					switch (sym->type()) {
					case Symbol::Type::CONSTANT:
						id->type(T_CONSTANT_ID);
						break;
					case Symbol::Type::OBJECT:
						id->type(T_OBJECT_ID);
						break;
					case Symbol::Type::VARIABLE:
						id->type(T_VARIABLE_ID);
						break;
					default:
						break;
					}
				}	
				
				parser->_token(id);
				parser->_token(pl);
			}
		}

arg_lst(lst) ::= ARG(arg).
		{
			lst = new CommaArgumentList();
			lst->first = new TokenList();
			lst->second = new Macro::ArgumentList();
			lst->second->push_back(arg);
		}

arg_lst(new_lst) ::= arg_lst(lst) COMMA(c) ARG(arg).
		{
			new_lst = lst;
			lst->first->push_back(c);
			lst->second->push_back(arg);
		}

/*************************************************************************************************/
/* other */
/*************************************************************************************************/

other ::= INTEGER(o).				{ parser->_token(o); }
other ::= PAREN_L(o).				{ parser->_token(o); }
other ::= PAREN_R(o).				{ parser->_token(o); }
other ::= COMMA(o).					{ parser->_token(o); }
other ::= COLON_DASH(o).			{ parser->_token(o); }
other ::= SEMICOLON(o).				{ parser->_token(o); }
other ::= STRING_LITERAL(o).		{ parser->_token(o); }
other ::= OTHER(o).					{ parser->_token(o); }
other ::= PERIOD(o).				{ parser->_token(o); }

