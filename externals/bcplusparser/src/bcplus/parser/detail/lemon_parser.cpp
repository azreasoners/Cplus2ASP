/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "bcplus/parser/detail/lemon_parser.y"

			#include <cassert>
			#include <cstring>

			#include <boost/foreach.hpp>

			#include "babb/utils/memory.h"

			#include "bcplus/Location.h"
			#include "bcplus/parser/BCParser.h"
			#include "bcplus/parser/Token.h"
			#include "bcplus/parser/detail/lemon_parser.h"
			#include "bcplus/parser/Number.h"
			#include "bcplus/parser/detail/NumberRange.h"
			#include "bcplus/parser/detail/NumberRangeEval.h"
			#include "bcplus/statements/Statement.h"
			#include "bcplus/statements/declarations.h"
			#include "bcplus/statements/QueryStatement.h"
			#include "bcplus/statements/blocks.h"
			#include "bcplus/statements/laws.h"
			#include "bcplus/elements/Element.h"
			#include "bcplus/elements/terms.h"
			#include "bcplus/elements/formulas.h"
			#include "bcplus/symbols/Symbol.h"
			#include "bcplus/symbols/MacroSymbol.h"
			#include "bcplus/symbols/ConstantSymbol.h"
			#include "bcplus/symbols/AttributeSymbol.h"
			#include "bcplus/symbols/NumberRangeSymbol.h"

			#define UNUSED void*

			using namespace bcplus;
			using namespace babb::utils;
			using namespace bcplus::parser;
			using namespace bcplus::statements;
			using namespace bcplus::elements;
			using namespace bcplus::languages;
			using namespace bcplus::symbols;
			using namespace bcplus::parser::detail;
			

			/// A list of terms
			typedef ReferencedVector<ref_ptr<const Term> >::type TermList;

			/// A list of sorts
			typedef ReferencedVector<ref_ptr<const SortSymbol> >::type SortList;


			typedef ReferencedVector<ref_ptr<const Token> >::type TokenList;

			/// Helper for deallocating things
			#define DEALLOC(x)	{ if (x) delete x; x = NULL; }

			/// A list of name sortlist pairs for declaring identifiers
			typedef std::pair<ref_ptr<const Token>, ref_ptr<SortList> > IdentifierDecl;
			typedef ReferencedVector<IdentifierDecl>::type IdentifierDeclList;

		
#line 332 "bcplus/parser/detail/lemon_parser.y"

	#define BASE_ELEM_DEF(elem, id, lparen, args, rparen, symtype, class, symclass)											\
		BASE_ELEM_DEF9(elem, id, lparen, args, rparen, symtype, class, symclass, false)

	#define BASE_ELEM_DEF9(elem, id, lparen, args, rparen, symtype, class, symclass, dynamic)								\
		elem = NULL;																										\
		ref_ptr<const Token> id_ptr = id;																					\
		ref_ptr<const Token> lparen_ptr = lparen;																			\
		ref_ptr<TermList> args_ptr = args;																					\
		ref_ptr<const Token> rparen_ptr = rparen;																			\
		size_t arity = (args_ptr ? args_ptr->size() : 0);																	\
																															\
		/* Check to see if we have constants in any of the terms */															\
		bool good = true;																									\
		if (!parser->lang()->support(Language::Feature::FORMULA_CONSTANT_ARGS) && arity) {									\
			int cargs = 0;																									\
			/*BOOST_FOREACH(Element const* elem, *args_ptr) { */															\
			for (TermList::const_iterator it = args_ptr->begin(); it != args_ptr->end(); it++) {							\
				cargs |=(*it)->cmask();																						\
			}																												\
			if (cargs) {																									\
				parser->_feature_error(Language::Feature::FORMULA_CONSTANT_ARGS, &id->beginLoc());							\
				good = false;																								\
			}																												\
		}																													\
																															\
		if (good) {																											\
			Symbol const* sym = parser->symtab()->resolve(symtype, *id_ptr->str(), arity);									\
																															\
			if (!sym && dynamic) {																							\
				/* Dynamic declarations are allowed. Attempt to create a new symbol */										\
				ref_ptr<ConstantSymbol::SortList> sorts = new ConstantSymbol::SortList();									\
				int argnum = 0;																								\
				if (args) {																									\
					BOOST_FOREACH(Term const* t, *args_ptr) {																\
							argnum++;																						\
						switch (t->subType()) {																				\
						case Term::Type::VARIABLE:																			\
							sorts->push_back(((Variable const*)t)->symbol()->sort());										\
							break;																							\
						case Term::Type::CONSTANT:																			\
							sorts->push_back(((Constant const*)t)->symbol()->sort());										\
							break;																							\
						case Term::Type::ANON_VAR:																			\
						case Term::Type::NULLARY:																			\
						case Term::Type::LUA:																				\
						case Term::Type::PYTHON:																			\
						case Term::Type::OBJECT:																			\
						case Term::Type::BINARY:																			\
						case Term::Type::UNARY:																				\
						case Term::Type::BINDING:																			\
							parser->_parse_error("Unable to dynamically declare abAction \"" + Symbol::genName(*id_ptr->str(), (args_ptr ? args_ptr->size() : 0))\
							+ "\". Could not deduce the sort of argument #" + boost::lexical_cast<std::string>(argnum)		\
							+ " as it isn't a constant or variable. This problem can be fixed by explicitly declaring the abAction" \
							" prior to this rule.", &id_ptr->beginLoc());													\
							good = false;																					\
							break;																							\
						}																									\
					}																										\
				}																											\
				if  (good) {																								\
					ref_ptr<ConstantSymbol> cs = new ConstantSymbol(ConstantSymbol::Type::ABACTION, id_ptr->str(), parser->symtab()->bsort(SymbolTable::BuiltinSort::BOOLEAN), sorts);\
					/* add the sort to the symbol table */																	\
					if (!parser->symtab()->create(cs)) {																	\
						/* It seems there was a problem. */																	\
						parser->_parse_error("An error occurred while declaring \"" + Symbol::genName(*id_ptr->str(), (args_ptr ? args_ptr->size() : 0)) + "\".");\
						good = false;																						\
						break;																								\
					} else sym = cs;																						\
				}																											\
			}																												\
																															\
			if (!good) {																									\
				YYERROR;																									\
			} else if (!sym || sym->type() != symtype) {																	\
				/* The preprocessor indicated this was a constant and it's not... oops. */									\
				parser->_parse_error(std::string("INTERNAL ERROR: Could not locate symbol table entry for ")				\
					+ Symbol::Type::cstr(symtype) + " \"" + Symbol::genName(*id_ptr->str(), arity)		 					\
					+ "\".", &id_ptr->beginLoc());																			\
				YYERROR;																									\
			} else {																										\
				elem = new class((symclass*)sym, args, id_ptr->beginLoc(), (arity ? rparen_ptr->endLoc() : id_ptr->endLoc()));	\
			} 																												\
		}																													\
	
	#define BASE_ELEM_BARE_DEF(elem, id, symtype, class, symclass)															\
		elem = NULL;																										\
		ref_ptr<const Token> id_ptr = id;																					\
																															\
		Symbol const* sym = parser->symtab()->resolve(symtype, *id_ptr->str());												\
		if (sym && sym->type() == symtype) {																				\
			elem = new class((symclass*)sym, id_ptr->beginLoc(), id_ptr->endLoc());											\
		} else {																											\
			/* The preprocessor indicated this was a constant and it's not... oops. */										\
			parser->_parse_error(std::string("INTERNAL ERROR: Could not locate symbol table entry for ")					\
				+ Symbol::Type::cstr(symtype) + " \"" + Symbol::genName(*id_ptr->str(), 0)			 						\
				+ "\".", &id_ptr->beginLoc());																				\
			YYERROR;																										\
		}	
		
	#define BASE_LUA_ELEM(elem, id, lparen, args, rparen)																	\
		ref_ptr<const Token> id_ptr = id;																					\
		ref_ptr<const Token> lparen_ptr = lparen;																			\
		ref_ptr<TermList> args_ptr = args;																					\
		ref_ptr<const Token> rparen_ptr = rparen;																			\
		elem = new LuaTerm(id_ptr->str(), args, id_ptr->beginLoc(), (args ? rparen_ptr->endLoc() : id_ptr->endLoc()));

	#define BASE_PYTHON_ELEM(elem, id, lparen, args, rparen)																	\
		ref_ptr<const Token> id_ptr = id;																					\
		ref_ptr<const Token> lparen_ptr = lparen;																			\
		ref_ptr<TermList> args_ptr = args;																					\
		ref_ptr<const Token> rparen_ptr = rparen;																			\
		elem = new PyTerm(id_ptr->str(), args, id_ptr->beginLoc(), (args ? rparen_ptr->endLoc() : id_ptr->endLoc()));

	#define UNDECLARED(elem, id, args)																						\
		elem = NULL;																										\
		ref_ptr<const Token> id_ptr = id;																					\
		ref_ptr<TermList> args_ptr = args;																					\
		parser->_parse_error("Encountered undeclared identifier \"" 														\
			+ Symbol::genName(*id->str(), (args_ptr ? args_ptr->size() : 0)) + "\".", &id_ptr->beginLoc());					\
		YYERROR;		

#line 532 "bcplus/parser/detail/lemon_parser.y"

	#define BASIC_TERM(term, id)																							\
		term = NULL;																										\
		ref_ptr<const Token> id_ptr = id;																					\
		ObjectSymbol const* sym = parser->symtab()->resolveOrCreate(new ObjectSymbol(id->str()));							\
		if (!sym) {																											\
			parser->_parse_error("An error occurred creating symbol \"" + *(id->str()) + "/0\".", &id->beginLoc());			\
			YYERROR;																										\
		} else term = new Object(sym, NULL, id->beginLoc(), id->endLoc());

	#define TERM_PARENS(term, lparen, sub, rparen)																			\
		ref_ptr<const Token> lparen_ptr = lparen;																			\
		ref_ptr<const Token> rparen_ptr = rparen;																			\
		term = sub;																											\
		term->parens(true);																									\
		term->beginLoc(lparen->beginLoc());																					\
		term->endLoc(rparen->endLoc());


	#define UNARY_ARITH(term, op, sub, operator)																			\
		term = NULL;																										\
		ref_ptr<const Token> op_ptr = op;																					\
		ref_ptr<Term> sub_ptr = sub;																						\
																															\
		if (sub->domainType() != DomainType::INTEGRAL && sub->domainType() != DomainType::UNKNOWN) {						\
			parser->_parse_error("Malformed arithmetic operation. Operand is not numeric.", &sub->beginLoc());				\
			YYERROR;																										\
		} else term = new UnaryTerm(operator, sub, sub->beginLoc(), sub->endLoc());

	#define BINARY_ARITH(term, lhs, op, rhs, operator)																		\
		term = NULL;																										\
		ref_ptr<Term> lhs_ptr = lhs;																						\
		ref_ptr<Term> rhs_ptr = rhs;																						\
		ref_ptr<const Token> op_ptr = op;																					\
																															\
		bool good = true;																									\
		if (lhs->domainType() != DomainType::INTEGRAL && lhs->domainType() != DomainType::UNKNOWN) {						\
			good = false;																									\
			parser->_parse_error("Malformed arithmetic operation. Left operand is not numeric.", &op->beginLoc());			\
			YYERROR;																										\
		}																													\
		if (rhs->domainType() != DomainType::INTEGRAL && rhs->domainType() != DomainType::UNKNOWN) {						\
			good = false;																									\
			parser->_parse_error("Malformed arithmetic operation. Right operand is not numeric.", &rhs->beginLoc());		\
			YYERROR;																										\
		}																													\
		if (good) term = new BinaryTerm(operator, lhs, rhs, lhs->beginLoc(), rhs->endLoc());

	#define NULLARY_TERM(term, op, feature, operator)																		\
		term = NULL;																										\
		ref_ptr<const Token> op_ptr = op;																					\
																															\
		if (!parser->lang()->support(feature)) {																			\
			parser->_feature_error(feature, &op->beginLoc());																\
		} else {																											\
			term = new NullaryTerm(operator, op->beginLoc(), op->endLoc());													\
		}

#line 801 "bcplus/parser/detail/lemon_parser.y"

	#define NUM_UOP(t_new, t, val) \
		ref_ptr<const Referenced> t_ptr = t; \
		t_new = new Number(val, t->beginLoc(), t->endLoc());


	#define NUM_BOP(t_new, l, r, val) \
		ref_ptr<const Referenced> l_ptr = l, r_ptr = r; \
		t_new = new Number(val, l->beginLoc(), r->endLoc());

#line 847 "bcplus/parser/detail/lemon_parser.y"

	#define NESTED_BOP(new_f, lhs, op, rhs, operator)															\
		new_f = NULL;																							\
		ref_ptr<Formula> lhs_ptr = lhs;																			\
		ref_ptr<const Token> op_ptr = op;																		\
		ref_ptr<Formula> rhs_ptr = rhs;																			\
																												\
		if (!parser->lang()->support(Language::Feature::FORMULA_NESTED)) {										\
			parser->_feature_error(Language::Feature::FORMULA_NESTED, &op->beginLoc());							\
			YYERROR;																							\
		}																										\
		new_f = new BinaryFormula(operator, lhs, rhs, lhs->beginLoc(), rhs->endLoc());  						\

	#define NESTED_UOP(new_f, op, rhs, operator, feature)														\
		new_f = NULL;																							\
		ref_ptr<const Token> op_ptr = op;																		\
		ref_ptr<Formula> rhs_ptr = rhs;																			\
																												\
		/* Check to ensure that the operator is supported */													\
		if (!parser->lang()->support(feature)) {																\
			parser->_feature_error(feature);																	\
			YYERROR;																							\
		} else if (!parser->lang()->support(Language::Feature::FORMULA_NESTED)) {								\
			/* Check to ensure that this isn't nested */														\
			if (rhs->subType() == Formula::Type::BINARY || rhs->subType() == Formula::Type::UNARY) {			\
				parser->_feature_error(Language::Feature::FORMULA_NESTED, &rhs->beginLoc());					\
				YYERROR;																						\
			}																									\
		}																										\
		else new_f = new UnaryFormula (UnaryFormula:: Operator::NOT, rhs, op->beginLoc(), rhs->endLoc());

#line 928 "bcplus/parser/detail/lemon_parser.y"


	#define ATOMIC_FORMULA(af, constant, value) 														\
		af = NULL;																						\
		ref_ptr<Constant> c_ptr = constant;																\
																										\
																										\
		ref_ptr<const ObjectSymbol> t =																	\
			(value ? parser->symtab()->bobj(SymbolTable::BuiltinObject::TRUE) 							\
				: parser->symtab()->bobj(SymbolTable::BuiltinObject::FALSE));							\
																										\
																										\
		/* If this is a boolean constant we can interpret it as a shortcut for c=true */				\
		if (!constant->symbol()->sort()->contains(t)) {													\
			parser->_parse_error("\"" + *constant->symbol()->name() 									\
				+ "\" is not a boolean valued constant and therefore "									\
				"cannot be used in a bare atomic formula.", &constant->beginLoc());						\
			YYERROR;																					\
		} else {																						\
			af = new AtomicFormula(constant,															\
					new Object(t, NULL, constant->beginLoc(), constant->endLoc()), 						\
					constant->beginLoc(), constant->endLoc());											\
		}
	


#line 1064 "bcplus/parser/detail/lemon_parser.y"

	#define CARD_FORMULA(card, min, lbrack, vars, af, rbrack, max)																	\
		card = NULL;																												\
		ref_ptr<const Referenced> vars_ptr = vars, af_ptr = af;																	\
		ref_ptr<const Term> min_ptr = min, max_ptr = max;																		\
		ref_ptr<const Token> lbrack_ptr = lbrack, rbrack_ptr = rbrack;															\
																																	\
		bool good = true;																										\
		if (min && min_ptr->domainType() != DomainType::INTEGRAL && min_ptr->domainType() != DomainType::UNKNOWN) {				\
			parser->_parse_error("Invalid lower cardinality bound. Expected an integral expression.", &min_ptr->beginLoc());		\
			good = false;																										\
			YYERROR;																											\
		}																														\
																																	\
		if (max && max_ptr->domainType() != DomainType::INTEGRAL && max_ptr->domainType() != DomainType::UNKNOWN) {				\
			parser->_parse_error("Invalid upper cardinality bound. Expected an integral expression.", &max_ptr->beginLoc());		\
			good = false;																										\
			YYERROR;																											\
		}																														\
																																\
		if (good) {																												\
			/* hopefully good to go. */																							\
			card = new CardinalityFormula(vars, af, min, max, 																	\
				(min ? min_ptr->beginLoc() : lbrack_ptr->beginLoc()), 															\
				(max ? max_ptr->endLoc() : rbrack_ptr->endLoc()));																\
		}																														\



#line 1133 "bcplus/parser/detail/lemon_parser.y"

	#define BINDING(new_f, lhs, op, rhs, class)																	\
		new_f = NULL;																							\
		ref_ptr<const Element> lhs_ptr = lhs, rhs_ptr;															\
		ref_ptr<const Token> op_ptr = op;																		\
																												\
		if (!parser->lang()->support(Language::Feature::QUERY_BIND_STEP)) {										\
			parser->_feature_error(Language::Feature::QUERY_BIND_STEP, &op->beginLoc());						\
			YYERROR;																							\
		} else {																								\
			new_f = new class(lhs, rhs, lhs->beginLoc(), rhs->endLoc());										\
		}

#line 1548 "bcplus/parser/detail/lemon_parser.y"

		
	#define DYNAMIC_SORT_PLUS(new_s, s, op, feature, o)																												\
		new_s = NULL;																																				\
		ref_ptr<const Referenced> s_ptr = s, op_ptr = op;																											\
																																									\
																																									\
		if (!parser->lang()->support(feature)) {																													\
			parser->_feature_error(feature, &op->beginLoc());																										\
			YYERROR;																																				\
		} else {																																					\
			new_s = parser->symtab()->plus(s, o);																													\
		}
#line 1676 "bcplus/parser/detail/lemon_parser.y"

	#define CONSTANT_DECL(c, loc)																							\
		if (!parser->symtab()->create(c)) {																					\
			/* Check if it's a duplicate */																					\
			ConstantSymbol* c2 = (ConstantSymbol*)parser->symtab()->resolve(Symbol::Type::CONSTANT, *c->base(), c->arity());\
			if (!c2 || c2 != c) {																							\
				parser->_parse_error("Detected conflicting definition of symbol \"" + *c->name() + "\".", &loc);			\
			} else {																										\
				parser->_parse_error("Detected a duplicate definition of symbol \"" + *c->name() + "\".", &loc);			\
			}																												\
		}
#line 2423 "bcplus/parser/detail/lemon_parser.y"

	#define NC_STATEMENT(stmt, kw, period, feature, class)											\
		stmt = NULL;																				\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = period;											\
																									\
		if (!parser->lang()->support(feature)) {													\
			parser->_feature_error(feature, &kw->beginLoc());										\
			YYERROR;																				\
		} else {																					\
			stmt = new class(kw->beginLoc(), period->endLoc());										\
		}																							

#line 2446 "bcplus/parser/detail/lemon_parser.y"

	#define VALUE_DECL(stmt, cd, kw, val_obj, p, feature, class)									\
		stmt = NULL;																				\
		ref_ptr<const Referenced> cd_ptr = cd, kw_ptr = kw, val_ptr = val_obj, p_ptr = p;			\
																									\
		if (!parser->lang()->support(feature)) {													\
			parser->_feature_error(feature, &kw->beginLoc());										\
			YYERROR;																				\
		} else { 																					\
			int value = val_obj->val();																\
			if (value < 0) {																		\
				parser->_parse_error("ERROR: Expected a positive integer.", &val_obj->beginLoc());	\
			} else {																				\
				stmt = new class(value, cd->beginLoc(), p->endLoc());								\
			}																						\
		}
#line 2475 "bcplus/parser/detail/lemon_parser.y"

	struct QueryData {
		QueryStatement::FormulaList* l;
		NumberRangeEval* maxstep;
		Token const* label;
	};

#line 2582 "bcplus/parser/detail/lemon_parser.y"

	#define QUERY_DECL(decl, kw, val, feature)																\
		decl = NULL;																						\
		ref_ptr<const Token> kw_ptr = kw, val_ptr = val;													\
																											\
		if (!parser->lang()->support(feature)) {															\
			parser->_feature_error(feature, &kw->beginLoc());												\
			YYERROR;																						\
		} else {																							\
			decl = val_ptr.release();																		\
		}

#line 2654 "bcplus/parser/detail/lemon_parser.y"

	#define CLAUSE(elem, kw, f, feature) 														\
		ref_ptr<const Token> kw_ptr = kw;														\
		elem = f;																				\
		if (!parser->lang()->support(feature)) {												\
			parser->_feature_error(feature, &kw->beginLoc());									\
			YYERROR;																			\
		}
#line 2743 "bcplus/parser/detail/lemon_parser.y"

	#define LAW_BASIC_FORM(law, kw, head, ifbody, ifcons, after, unless, where, p, static, dynamic, class)											\
		law = NULL;																																	\
		ref_ptr<Element> head_ptr = head, if_ptr = ifbody, ifcons_ptr = ifcons, after_ptr = after, unless_ptr = unless, where_ptr = where;		\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = p;																								\
		Language::Feature::type feature = ((after) ? (dynamic) : (static));																			\
																																					\
		if (!parser->lang()->support(feature)) {																									\
			parser->_feature_error(feature, ((kw) ? &(kw_ptr)->beginLoc() : &(head_ptr)->beginLoc()));												\
			YYERROR;																																\
		} else {																																	\
			law = new class(head, ifbody, ifcons, after, unless, where, ((kw) ? (kw_ptr)->beginLoc() : (head_ptr)->beginLoc()), (p_ptr)->endLoc());	\
		}																																			

	#define LAW_IMPL_FORM(law, head, kw, body, where, p, feature, class)																			\
		law = NULL;																																	\
		ref_ptr<Element> body_ptr = body, where_ptr = where;																						\
		ref_ptr<Formula> head_ptr = head;																											\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = p;																								\
																																					\
		if (!head) head_ptr = new NullaryFormula(NullaryFormula::Operator::FALSE, kw->beginLoc(), kw->beginLoc());									\
		if (!parser->lang()->support(feature)) {																									\
			parser->_feature_error(feature, &kw->beginLoc());																						\
			YYERROR;																																\
		} else {																																	\
			law = new class(head_ptr, body, where, head_ptr->beginLoc(), p->endLoc());																\
		}


	#define LAW_DYNAMIC_FORM(law, body, kw, head, ifbody, unless, where, p, feature, class)															\
		law = NULL;																																	\
		ref_ptr<Element> body_ptr = body, head_ptr = head, if_ptr = ifbody, unless_ptr = unless, where_ptr = where;								\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = p;																								\
																																					\
		if (!parser->lang()->support(feature)) {																									\
			parser->_feature_error(feature, &kw->beginLoc());																						\
			YYERROR;																																\
		} else {																																	\
			law = new class(body, head, ifbody, unless, where, body->beginLoc(), p->endLoc());														\
		}
	
	#define LAW_INCREMENTAL_FORM(law, body, kw, head, by, ifbody, unless, where, p, feature, class)													\
		law = NULL;																																	\
		ref_ptr<Element> body_ptr = body, head_ptr = head, if_ptr = ifbody, unless_ptr = unless, where_ptr = where;								\
		ref_ptr<Element> by_ptr = by;																											\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = p;																								\
																																					\
		if (!parser->lang()->support(feature)) {																									\
			parser->_feature_error(feature, &kw->beginLoc());																						\
			YYERROR;																																\
		} else {																																	\
			law = new class(body, head, by, ifbody, unless, where, body->beginLoc(), p->endLoc());													\
		}

		
	#define LAW_CONSTRAINT_FORM(law, kw, body, after, unless, where, p, static, dynamic, class)														\
		law = NULL;																																	\
		ref_ptr<Element> body_ptr = body, after_ptr = after, unless_ptr = unless, where_ptr = where;												\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = p;																								\
																																					\
		Language::Feature::type feature = (after ? dynamic : static);																				\
																																					\
		if (!parser->lang()->support(feature)) {																									\
			parser->_feature_error(feature, (kw ? &kw_ptr->beginLoc() : &body_ptr->beginLoc()));													\
			YYERROR;																																\
		} else {																																	\
			law = new class(body, after, unless, where, (kw ? kw_ptr->beginLoc() : body_ptr->beginLoc()), p_ptr->endLoc());							\
		}

	#define LAW_DYNAMIC_CONSTRAINT_FORM(law, kw, body, ifbody, unless, where, p, feature, class)													\
		law = NULL;																																	\
		ref_ptr<Element> body_ptr = body, if_ptr = ifbody, unless_ptr = unless, where_ptr = where;													\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = p;																								\
																																					\
		if (!parser->lang()->support(feature)) {																									\
			parser->_feature_error(feature, (kw ? &kw_ptr->beginLoc() : &body_ptr->beginLoc()));													\
			YYERROR;																																\
		} else {																																	\
			law = new class(body, ifbody, unless, where, (kw ? kw_ptr->beginLoc() : body_ptr->beginLoc()), p_ptr->endLoc());						\
		}
	
	#define LAW_SIMPLE_FORM(law, kw, head, where, p, feature, class)																				\
		law = NULL;																																	\
		ref_ptr<Element> head_ptr = head;																		\
		ref_ptr<const Referenced> where_ptr = where;															\
		ref_ptr<const Token> kw_ptr = kw, p_ptr = p;															\
																												\
		if (!parser->lang()->support(feature)) {																\
			parser->_feature_error(feature, (kw ? &kw_ptr->beginLoc() : &head_ptr->beginLoc()));				\
			YYERROR;																							\
		} else {																								\
			law = new class(head, where, (kw ? kw_ptr->beginLoc() : head_ptr->beginLoc()), p_ptr->endLoc());	\
		}
		

#line 2936 "bcplus/parser/detail/lemon_parser.y"

	#define CODE_BLK(stmt, code, feature, type) 												\
		ref_ptr<const Token> code_ptr = code;													\
		if (!parser->lang()->support(feature)) {												\
			stmt = NULL;																		\
			parser->_feature_error(feature, &code->beginLoc());									\
			YYERROR;																			\
		} else {																				\
			stmt = new type(code, code->beginLoc(), code->endLoc());							\
		}
#line 556 "bcplus/parser/detail/lemon_parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    lemon_parserTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is lemon_parserTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    lemon_parserARG_SDECL     A static variable declaration for the %extra_argument
**    lemon_parserARG_PDECL     A parameter declaration for the %extra_argument
**    lemon_parserARG_STORE     Code to store %extra_argument into yypParser
**    lemon_parserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned short int
#define YYNOCODE 259
#define YYACTIONTYPE unsigned short int
#define lemon_parserTOKENTYPE  Token const* 								
typedef union {
  int yyinit;
  lemon_parserTOKENTYPE yy0;
  Variable* yy1;
  ConstantDeclaration::ElementList* yy37;
  MacroDeclaration* yy39;
  VariableDeclaration* yy40;
  QuantifierFormula* yy77;
  CardinalityFormula* yy81;
  VariableDeclaration::ElementList* yy89;
  SortDeclaration* yy93;
  NumberRangeEval* yy97;
  TokenList* yy112;
  ConstantDeclaration* yy151;
  Token const* yy167;
  Object* yy186;
  CardinalityFormula::VariableList* yy187;
  Formula* yy201;
  Term* yy223;
  SortSymbol* yy229;
  SortDeclaration::ElementList* yy247;
  Number* yy248;
  ConstantSymbol::SortList* yy255;
  SortSymbol const* yy256;
  ObjectDeclaration::ElementList* yy262;
  ObjectDeclaration::Element::ObjectList* yy285;
  QueryData yy289;
  Constant* yy301;
  ObjectDeclaration::Element* yy302;
  ConstantSymbol::Type::type yy313;
  ShowStatement::ElementList* yy323;
  UNUSED yy361;
  StrongNCStatement* yy370;
  AtomicFormula* yy371;
  MacroSymbol::ArgumentList* yy374;
  Statement* yy376;
  IdentifierDeclList* yy414;
  MacroSymbol* yy427;
  NCStatement* yy441;
  QuantifierFormula::QuantifierList* yy449;
  LuaTerm* yy457;
  MacroDeclaration::ElementList* yy458;
  QueryStatement* yy482;
  TermList* yy491;
  ObjectDeclaration* yy492;
  NumberRange* yy513;
  QuantifierFormula::Operator::type yy514;
  int yy517;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define lemon_parserARG_SDECL  BCParser* parser						;
#define lemon_parserARG_PDECL , BCParser* parser						
#define lemon_parserARG_FETCH  BCParser* parser						 = yypParser->parser						
#define lemon_parserARG_STORE yypParser->parser						 = parser						
#define YYNSTATE 870
#define YYNRULE 459
#define YYERRORSYMBOL 138
#define YYERRSYMDT yy517
#define YYFALLBACK 1
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (3500)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   869,   64,  870,  868,  867,  866,  865,  864,  863,  862,
 /*    10 */   861,  860,  859,  858,  857,  856,  855,  854,  697,  826,
 /*    20 */   323,  853,  846,  852,  851,  845,  149,  147,  145,  144,
 /*    30 */   143,  848,  319,  324,  826,  323,  853,  846,  480,  851,
 /*    40 */   845,  785,  422,  117,  115,  114,  847,  318,  324,  699,
 /*    50 */   768,  363,  246,   63,  337,  777,  774,  773,  772,  771,
 /*    60 */   699,  122,  363,  248,  784,  783,  684,   18,  482,  826,
 /*    70 */   323,  853,  846,  852,  851,  845,  122,   23,   22,   21,
 /*    80 */    25,   24,  318,  324,   26,  472,  473,  700,  701,  368,
 /*    90 */   777,  774,  773,  772,  771,  471,  519,  473,  700,  701,
 /*   100 */   246,  595,  594,  593,  592,  591,  590,  589,  588,  587,
 /*   110 */   586,  585,  584,  583,  582,  581,  580,  579,  578,  577,
 /*   120 */   825,  549,  655,   62,  550,  824,  561,  560,  557,  556,
 /*   130 */   555,  554,  559,  558,  121,  119,  117,  115,  114,  204,
 /*   140 */   644,  539,  853,  846,  852,  851,  845,   59,   30,   12,
 /*   150 */   169,  167,  166,   44,   61,   10,   11,  101,  192,  675,
 /*   160 */   269,  363,  779,  613,  525,  263,  215,  214,  213,  798,
 /*   170 */   797,  799,    9,  502,  501,    8,  313,   43,  255,  327,
 /*   180 */   617,  614,  612,  611,  781,  782,  719,  698,  648,  325,
 /*   190 */   185,   47,   48,  730,  243,  675,  310,  363,  766,  765,
 /*   200 */    13,   23,   22,   21,   25,   24,   20,  677,   26,  695,
 /*   210 */   694,  693,  692,  293,  643,  549,  165,  517,  550,  642,
 /*   220 */   242,  240,  238,  237,  236,  691,  125,  689,  241,  721,
 /*   230 */   802,   98,  688,  239,  311,  714,  713,  715,  100,  553,
 /*   240 */   466,  673,  465,  674,  687,  685,  686,  690,  663,  210,
 /*   250 */   716,  717,  552,  844,  551,  665,  191,   60,  220,   46,
 /*   260 */  1167,  503,  298,  621,  620,  504,  643,  549,  193,  301,
 /*   270 */   550,  642,  699,   45,  363, 1167, 1167,  194,  609,  610,
 /*   280 */    99,  199,  201,  202,    6,  699,   42,  363,  702,  703,
 /*   290 */   310,   30,  218,  201,  202, 1167, 1167,  216,  680,  757,
 /*   300 */   747,  853,  846,  852,  851,  845, 1167, 1167,  518,  473,
 /*   310 */   700,  701,   73, 1167,  321,  621,  620,  622,  184,   40,
 /*   320 */    41,  470,  473,  700,  701,  131,  365,  744,  741,   15,
 /*   330 */   609,  610,  126,  199,   72, 1167,    6,   26,   42,   71,
 /*   340 */   825,  549,  310,    7,  550,  824,  552,  844,  551,   16,
 /*   350 */    54,   47,  547,   30,   14,  138,  137,  136,  516,  135,
 /*   360 */   134,  133,  132,   23,   22,   21,   25,   24,  766,  765,
 /*   370 */    26,   40,   41,  661,   25,   24,   33,  131,   26,  151,
 /*   380 */   142,  141,  140,  139,  539, 1230,   74,  547,  231,  798,
 /*   390 */   797,  799,  825,  549,  454,  538,  550,  824,  552,  844,
 /*   400 */   551,  475,  681,  474,  769,  770,  759,  206,  661,  758,
 /*   410 */     3, 1230,   29,   47,  432,  658,  310,   23,   22,   21,
 /*   420 */    25,   24,  705,   67,   26,  826,  323,  853,  846,  852,
 /*   430 */   851,  845,  467,  513,   22,   21,   25,   24,  316,  324,
 /*   440 */    26,  798,  797,  799,  547,   27,   28,  785,  780,  431,
 /*   450 */   658,   98,   50,   49,   53,   52,  781,  782,   54,  212,
 /*   460 */   601,  600,  185, 1091,   48,  661,  243, 1091,  310,  481,
 /*   470 */   784,  783,  552,  844,  551,  219,  217,  215,  214,  213,
 /*   480 */   292,  714,  713,  715,  238,  237,  236,  736,  826,  323,
 /*   490 */   853,  846,  480,  851,  845,  735,  716,  717,  125,  734,
 /*   500 */   241,  318,  324,   98,  220,  239,  190,  662,  339,  777,
 /*   510 */   774,  773,  772,  771,  826,  326,  853,  846,  852,  851,
 /*   520 */   845,   19,  482,  843,  552,  844,  551,  796,  407,  644,
 /*   530 */   539,  853,  846,  852,  851,  845,  839,  549,  218,   66,
 /*   540 */   550,  838,  733,  216,  707,   53,   52,  637,  706,   54,
 /*   550 */    38,   37,  613,  122,   39,   64,  644,  539,  853,  846,
 /*   560 */   480,  851,  845,  841,  819,  314,  725,  726,  344,  617,
 /*   570 */   614,  612,  611,  756,  549,  478,  722,  550,  755,  613,
 /*   580 */   149,  147,  145,  144,  143,  833,  832,  834,   65,   31,
 /*   590 */   482,  761,  314,  122,  174,  329,  617,  614,  612,  611,
 /*   600 */   835,  836,  453,  599,  598,  463,  668,  462,  120,  842,
 /*   610 */   145,  144,  143,  527,  826,  406,  853,  846,  852,  851,
 /*   620 */   845,  122,  749,  748,  750,  312,  672,  369,  407,  757,
 /*   630 */   747,  853,  846,  852,  851,  845,  235,  739,  740,  667,
 /*   640 */   732,  244,  118,  251,  317,   57,  209,  116,  731,  644,
 /*   650 */   539,  853,  846,  480,  851,  845,  334,  744,  741,  203,
 /*   660 */   826,  794,  853,  846,  852,  851,  845,  837,  552,  844,
 /*   670 */   551,  728,  613,  796,  407,  461,  457,  510,   55,   56,
 /*   680 */   696,  548,   32,  482,  152,  314,  539,  179,  331,  617,
 /*   690 */   614,  612,  611,  644,  539,  853,  846,  852,  851,  845,
 /*   700 */   121,  119,  117,  115,  114,  552,  844,  551,  780,    7,
 /*   710 */   182,  657,  641,  511,  817,  549,  613,   30,  550,  816,
 /*   720 */    14,  138,  137,  136,  246,  135,  134,  133,  132,  314,
 /*   730 */   274,  193,  343,  617,  614,  612,  611,  540,  639,  549,
 /*   740 */   194,  547,  550,  638,  526,  151,  142,  141,  140,  139,
 /*   750 */   839,  549, 1268,  233,  550,  838,  547,  186,  234,  723,
 /*   760 */   823,  310,  546,  811,  810,  812,  183,  603,  602,  108,
 /*   770 */   107,  106, 1268,  105,  104,  103,  102,  661,  813,  814,
 /*   780 */    70,  121,  119,  117,  115,  114,  172,  633,  632,  634,
 /*   790 */   521,  275,  547,  113,  112,  111,  110,  109,  787,  833,
 /*   800 */   832,  834,  635,  636,   23,   22,   21,   25,   24,  760,
 /*   810 */   150,   26,  205,  544,  835,  836,  520,  552,  754,  660,
 /*   820 */   170,  699,  120,  363,  312,  168,  178,  159,  158,  157,
 /*   830 */   604,  156,  155,  154,  153,  540,  840,  849,  850,  853,
 /*   840 */   846,  852,  851,  845,  148,  420,  552,  844,  551,  146,
 /*   850 */     5,  164,  163,  162,  161,  160,  118,  524,  473,  700,
 /*   860 */   701,  116,  664,  539,  523,  522,  720,  474,  552,  508,
 /*   870 */   552,  844,  551,  826,  323,  853,  846,  852,  851,  845,
 /*   880 */   548,  844,  552,  844,  551,  780,  315,  324,  173,  171,
 /*   890 */   169,  167,  166,  335,  777,  774,  773,  772,  771,  515,
 /*   900 */   826,  322,  853,  846,  852,  851,  845,  488,  671,  462,
 /*   910 */    69,  247,  308,  778,  324,  683,  181,  312,  670,  815,
 /*   920 */   763,  777,  774,  773,  772,  771,  826,  323,  853,  846,
 /*   930 */   852,  851,  845,  200,   23,   22,   21,   25,   24,  318,
 /*   940 */   324,   26,  766,  765,  177,  182,  776,  777,  774,  773,
 /*   950 */   772,  771,  173,  171,  169,  167,  166,  826,  323,  853,
 /*   960 */   846,  852,  851,  845,  476,  724,  552,   68,  460,  510,
 /*   970 */   318,  324,  682,  548,  699,  176,  363,  775,  777,  774,
 /*   980 */   773,  772,  771,  826,  323,  853,  846,  852,  851,  845,
 /*   990 */   175,   23,   22,   21,   25,   24,  318,  324,   26,  605,
 /*  1000 */   762,  345,  182,  542,  777,  774,  773,  772,  771,  469,
 /*  1010 */   519,  473,  700,  701,  826,  323,  853,  846,  852,  851,
 /*  1020 */   845,  608,   23,   22,   21,   25,   24,  318,  324,   26,
 /*  1030 */   659,  699,  509,  363,  541,  777,  774,  773,  772,  771,
 /*  1040 */   826,  323,  853,  846,  852,  851,  845,  666,   36,   35,
 /*  1050 */    34,   38,   37,  318,  324,   39,  656,  737,  459,  510,
 /*  1060 */   389,  777,  774,  773,  772,  771,  468,  519,  473,  700,
 /*  1070 */   701,  826,  323,  853,  846,  852,  851,  845,   39,   51,
 /*  1080 */    50,   49,   53,   52,  318,  324,   54,  198,  699,  232,
 /*  1090 */   363,  441,  777,  774,  773,  772,  771,  826,  323,  853,
 /*  1100 */   846,  852,  851,  845,  452,  242,  240,  238,  237,  236,
 /*  1110 */   318,  324,  130,  458,  510,  676,  230,  440,  777,  774,
 /*  1120 */   773,  772,  771,  464,  519,  473,  700,  701,  826,  323,
 /*  1130 */   853,  846,  852,  851,  845,  597,   36,   35,   34,   38,
 /*  1140 */    37,  318,  324,   39,  596,  182,  576,  823,  340,  777,
 /*  1150 */   774,  773,  772,  771,  826,  323,  853,  846,  852,  851,
 /*  1160 */   845,  575,   92,   91,   90,   89,   88,  318,  324,  129,
 /*  1170 */   433,  510,  245,  308,  338,  777,  774,  773,  772,  771,
 /*  1180 */   113,  112,  111,  110,  109,  826,  323,  853,  846,  852,
 /*  1190 */   851,  845,  548,   36,   35,   34,   38,   37,  318,  324,
 /*  1200 */    39, 1330,    1,  574,  718,  336,  777,  774,  773,  772,
 /*  1210 */   771,  826,  323,  853,  846,  852,  851,  845,  803,   97,
 /*  1220 */    96,   95,   94,   93,  318,  324,  128,  573,  572,  801,
 /*  1230 */   570,  367,  777,  774,  773,  772,  771,  219,  217,  215,
 /*  1240 */   214,  213,  826,  323,  853,  846,  852,  851,  845,  569,
 /*  1250 */    36,   35,   34,   38,   37,  318,  324,   39,  567,  566,
 /*  1260 */   565,  654,  366,  777,  774,  773,  772,  771,  826,  323,
 /*  1270 */   853,  846,  852,  851,  845,  564,  219,  217,  215,  214,
 /*  1280 */   213,  318,  324,  127,  563,  844,  767,  787,  227,  777,
 /*  1290 */   774,  773,  772,  771,  242,  240,  238,  237,  236,  826,
 /*  1300 */   323,  853,  846,  852,  851,  845,  552,   36,   35,   34,
 /*  1310 */    38,   37,  320,  324,   39,  306,  699,  764,  363,  357,
 /*  1320 */   777,  774,  773,  772,  771,  826,  323,  853,  846,  852,
 /*  1330 */   851,  845,   17,   23,   22,   21,   25,   24,  318,  324,
 /*  1340 */    26,  305,  606,   16,  304,  226,  777,  774,  773,  772,
 /*  1350 */   771,  507,  512,  473,  700,  701,  826,  323,  853,  846,
 /*  1360 */   852,  851,  845,  189,   36,   35,   34,   38,   37,  318,
 /*  1370 */   324,   39,  535,   58,  302,  532,  225,  777,  774,  773,
 /*  1380 */   772,  771,  826,  323,  853,  846,  852,  851,  845,   58,
 /*  1390 */    51,   50,   49,   53,   52,  318,  324,   54,  533,  312,
 /*  1400 */   299,  530,  221,  777,  774,  773,  772,  771,  242,  240,
 /*  1410 */   238,  237,  236,  826,  323,  853,  846,  852,  851,  845,
 /*  1420 */   531,   23,   22,   21,   25,   24,  318,  324,   26,  296,
 /*  1430 */   290,  529,  294,  224,  777,  774,  773,  772,  771,  826,
 /*  1440 */   323,  853,  846,  852,  851,  845,  647,   36,   35,   34,
 /*  1450 */    38,   37,  318,  324,   39,  548,  528,  729,  477,  223,
 /*  1460 */   777,  774,  773,  772,  771,  678,  698,  289,  288,  286,
 /*  1470 */   826,  323,  853,  846,  852,  851,  845,  500,  242,  240,
 /*  1480 */   238,  237,  236,  318,  324,    4,  285,  284,  823,  568,
 /*  1490 */   222,  777,  774,  773,  772,  771,  499,  108,  107,  106,
 /*  1500 */    16,  105,  104,  103,  102,  826,  323,  853,  846,  852,
 /*  1510 */   851,  845,  282,  280,   35,   34,   38,   37,  319,  324,
 /*  1520 */    39,  113,  112,  111,  110,  109,  278,  785,  780,  497,
 /*  1530 */   496,  495,  276,  273,  826,  323,  853,  846,  852,  851,
 /*  1540 */   845,  494,   30,  272,  493,  271,  268,  319,  324,  786,
 /*  1550 */   784,  783,  267,  270,  264,  266,  785,  780,  265,  492,
 /*  1560 */   826,  323,  853,  846,  852,  851,  845,  262,   23,   22,
 /*  1570 */    21,   25,   24,  319,  324,   26,  261,  260,  253,  784,
 /*  1580 */   783,  259,  785,  780,  258,  826,  323,  853,  846,  852,
 /*  1590 */   851,  845,  491,  256,  254,  490,  489,  303,  319,  324,
 /*  1600 */   295,  727,  291,  287,  252,  784,  783,  785,  780,  307,
 /*  1610 */   826,  323,  853,  846,  852,  851,  845,  704,  309,  361,
 /*  1620 */   675,  362,  363,  319,  324,  429,  646,  653,  430,  250,
 /*  1630 */   784,  783,  785,  780,  644,  539,  853,  846,  852,  851,
 /*  1640 */   845,  652,  644,  539,  853,  846,  852,  851,  845,  651,
 /*  1650 */   650,  649,  360,  300,  249,  784,  783,  613,  242,  240,
 /*  1660 */   238,  237,  236,  359,  358,  613,  679,  465,  674,  372,
 /*  1670 */   619,  342,  341,  607,  617,  614,  612,  611,  314,  534,
 /*  1680 */   297,  616,  617,  614,  612,  611,  644,  539,  853,  846,
 /*  1690 */   852,  851,  845,  498,  644,  539,  853,  846,  852,  851,
 /*  1700 */   845,  283,  279,  277,  257,  364,  281,  434,  712,  613,
 /*  1710 */   435,  711,  514,  710,  709,  708,  375,  613,  669, 1331,
 /*  1720 */  1331,  645,  314, 1331, 1331,  615,  617,  614,  612,  611,
 /*  1730 */   314, 1331, 1331,  506,  617,  614,  612,  611,  644,  539,
 /*  1740 */   853,  846,  852,  851,  845, 1331,  644,  539,  853,  846,
 /*  1750 */   852,  851,  845,  242,  240,  238,  237,  236, 1331, 1331,
 /*  1760 */  1331,  613,  818,  809,  853,  846,  852,  851,  845,  613,
 /*  1770 */  1331, 1331,  483,  411,  314, 1331, 1331,  505,  617,  614,
 /*  1780 */   612,  611,  314, 1331, 1331,  373,  617,  614,  612,  611,
 /*  1790 */   644,  539,  853,  846,  852,  851,  845, 1331,  644,  539,
 /*  1800 */   853,  846,  852,  851,  845,  840,  849,  850,  853,  846,
 /*  1810 */   852,  851,  845,  613,  450, 1331, 1331, 1331, 1331, 1331,
 /*  1820 */  1331,  613, 1331, 1331, 1331, 1331,  314, 1331, 1331,  424,
 /*  1830 */   617,  614,  612,  611,  314, 1331, 1331,  423,  617,  614,
 /*  1840 */   612,  611,  644,  539,  853,  846,  852,  851,  845, 1331,
 /*  1850 */   644,  539,  853,  846,  852,  851,  845,  840,  849,  850,
 /*  1860 */   853,  846,  852,  851,  845,  613,  371, 1331, 1331, 1331,
 /*  1870 */  1331, 1331, 1331,  613, 1331, 1331, 1331, 1331,  314, 1331,
 /*  1880 */  1331,  332,  617,  614,  612,  611,  314, 1331, 1331,  330,
 /*  1890 */   617,  614,  612,  611,  644,  539,  853,  846,  852,  851,
 /*  1900 */   845, 1331,  186, 1331,  825,  549, 1331, 1331,  550,  824,
 /*  1910 */     4, 1331, 1331, 1331,  108,  107,  106,  613,  105,  104,
 /*  1920 */   103,  102,  108,  107,  106, 1331,  105,  104,  103,  102,
 /*  1930 */   314, 1331, 1331,  328,  617,  614,  612,  611,  113,  112,
 /*  1940 */   111,  110,  109, 1331, 1331, 1331,  113,  112,  111,  110,
 /*  1950 */   109, 1331, 1331,  798,  797,  799, 1331,   87,   86,   85,
 /*  1960 */  1331,   84,   83,   82,   81,   74,   80,   79, 1331,   78,
 /*  1970 */    77,   76,   75,  938,  938,  938,  124,  938,  938,  938,
 /*  1980 */   938,   92,   91,   90,   89,   88, 1331, 1331, 1331,   97,
 /*  1990 */    96,   95,   94,   93, 1331, 1331, 1331,  938,  938,  938,
 /*  2000 */   938,  938,  159,  158,  157, 1331,  156,  155,  154,  153,
 /*  2010 */   123, 1331, 1331, 1331, 1331,   98, 1331,  108,  107,  106,
 /*  2020 */  1331,  105,  104,  103,  102, 1331,  164,  163,  162,  161,
 /*  2030 */   160, 1331, 1331,  571, 1331, 1331,  552,  844,  551, 1331,
 /*  2040 */  1331,  113,  112,  111,  110,  109, 1331, 1331,  757,  747,
 /*  2050 */   853,  846,  852,  851,  845,  757,  747,  853,  846,  852,
 /*  2060 */   851,  845, 1331,  746, 1331,  242,  240,  238,  237,  236,
 /*  2070 */   321, 1331, 1331, 1331, 1331,  738,  744,  741, 1331, 1331,
 /*  2080 */  1331, 1331,  743,  744,  741,  757,  747,  853,  846,  852,
 /*  2090 */   851,  845,  757,  747,  853,  846,  852,  851,  845, 1331,
 /*  2100 */   321, 1331, 1331, 1331, 1331, 1331,  562,  321, 1331, 1331,
 /*  2110 */  1331, 1331,  742,  744,  741, 1331, 1331, 1331, 1331,  537,
 /*  2120 */   744,  741,  757,  747,  853,  846,  852,  851,  845,  757,
 /*  2130 */   747,  853,  846,  852,  851,  845, 1331,  321,  242,  240,
 /*  2140 */   238,  237,  236, 1331,  321, 1331, 1331, 1331, 1331,  536,
 /*  2150 */   744,  741,  719,  698, 1331, 1331,  376,  744,  741,  757,
 /*  2160 */   747,  853,  846,  852,  851,  845,  757,  747,  853,  846,
 /*  2170 */   852,  851,  845,  197,  321,  211,  180,    2,  188,  195,
 /*  2180 */   196,  321, 1331, 1331, 1331, 1331,  437,  744,  741, 1331,
 /*  2190 */  1331, 1331, 1331,  436,  744,  741, 1331, 1331, 1331, 1331,
 /*  2200 */  1331,  714,  713,  715, 1331, 1331, 1331,  455,  456,  818,
 /*  2210 */   809,  853,  846,  852,  851,  845,  716,  717, 1331, 1331,
 /*  2220 */   412, 1331, 1331, 1331,  220, 1331, 1331,  840,  849,  850,
 /*  2230 */   853,  846,  852,  851,  845,  487,  421, 1331, 1331,  840,
 /*  2240 */   849,  850,  853,  846,  852,  851,  845,  486,  421, 1331,
 /*  2250 */   840,  849,  850,  853,  846,  852,  851,  845,  218,  451,
 /*  2260 */   719, 1331, 1331,  216,  840,  849,  850,  853,  846,  852,
 /*  2270 */   851,  845,  484,  421, 1331, 1331,  840,  849,  850,  853,
 /*  2280 */   846,  852,  851,  845,  479,  421, 1331, 1331, 1331, 1331,
 /*  2290 */  1331, 1331, 1331,  187,  840,  849,  850,  853,  846,  852,
 /*  2300 */   851,  845,  485,  421, 1331, 1331, 1331, 1331, 1331,  714,
 /*  2310 */   713,  715, 1331, 1331,  840,  849,  850,  853,  846,  852,
 /*  2320 */   851,  845, 1331,  831,  716,  717, 1331, 1331, 1331, 1331,
 /*  2330 */  1331, 1331,  220,  840,  849,  850,  853,  846,  852,  851,
 /*  2340 */   845, 1331,  827,  840,  849,  850,  853,  846,  852,  851,
 /*  2350 */   845, 1331,  830, 1331, 1331, 1331, 1331, 1331,  840,  849,
 /*  2360 */   850,  853,  846,  852,  851,  845,  218,  829, 1331, 1331,
 /*  2370 */  1331,  216, 1331,  840,  849,  850,  853,  846,  852,  851,
 /*  2380 */   845, 1331,  828, 1331,  840,  849,  850,  853,  846,  852,
 /*  2390 */   851,  845, 1331,  449, 1331, 1331,  840,  849,  850,  853,
 /*  2400 */   846,  852,  851,  845, 1331,  448, 1331,  840,  849,  850,
 /*  2410 */   853,  846,  852,  851,  845, 1331,  822,  840,  849,  850,
 /*  2420 */   853,  846,  852,  851,  845, 1331,  821,  840,  849,  850,
 /*  2430 */   853,  846,  852,  851,  845, 1331,  820,  840,  849,  850,
 /*  2440 */   853,  846,  852,  851,  845, 1331,  419, 1331, 1331, 1331,
 /*  2450 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  418,
 /*  2460 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  417,
 /*  2470 */  1331,  840,  849,  850,  853,  846,  852,  851,  845, 1331,
 /*  2480 */   416,  840,  849,  850,  853,  846,  852,  851,  845, 1331,
 /*  2490 */   415,  840,  849,  850,  853,  846,  852,  851,  845, 1331,
 /*  2500 */   414,  840,  849,  850,  853,  846,  852,  851,  845, 1331,
 /*  2510 */   413, 1331, 1331, 1331, 1331,  840,  849,  850,  853,  846,
 /*  2520 */   852,  851,  845, 1331,  409, 1331, 1331, 1331,  840,  849,
 /*  2530 */   850,  853,  846,  852,  851,  845, 1331,  408, 1331,  840,
 /*  2540 */   849,  850,  853,  846,  852,  851,  845, 1331,  795, 1331,
 /*  2550 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  445,
 /*  2560 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  444,
 /*  2570 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  793,
 /*  2580 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  792,
 /*  2590 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  791,
 /*  2600 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  443,
 /*  2610 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  442,
 /*  2620 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  790,
 /*  2630 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  789,
 /*  2640 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  788,
 /*  2650 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  405,
 /*  2660 */   840,  849,  850,  853,  846,  852,  851,  845, 1331,  404,
 /*  2670 */  1331, 1331,  840,  849,  850,  853,  846,  852,  851,  845,
 /*  2680 */  1331,  403, 1331, 1331, 1331,  840,  849,  850,  853,  846,
 /*  2690 */   852,  851,  845, 1331,  402, 1331,  840,  849,  850,  853,
 /*  2700 */   846,  852,  851,  845, 1331,  401, 1331,  840,  849,  850,
 /*  2710 */   853,  846,  852,  851,  845, 1331,  400,  840,  849,  850,
 /*  2720 */   853,  846,  852,  851,  845, 1331,  399,  840,  849,  850,
 /*  2730 */   853,  846,  852,  851,  845, 1331,  398,  840,  849,  850,
 /*  2740 */   853,  846,  852,  851,  845, 1331,  397,  840,  849,  850,
 /*  2750 */   853,  846,  852,  851,  845, 1331,  396,  840,  849,  850,
 /*  2760 */   853,  846,  852,  851,  845, 1331,  395,  840,  849,  850,
 /*  2770 */   853,  846,  852,  851,  845, 1331,  394,  840,  849,  850,
 /*  2780 */   853,  846,  852,  851,  845, 1331,  393,  840,  849,  850,
 /*  2790 */   853,  846,  852,  851,  845, 1331,  392,  840,  849,  850,
 /*  2800 */   853,  846,  852,  851,  845, 1331,  391,  840,  849,  850,
 /*  2810 */   853,  846,  852,  851,  845, 1331,  390,  840,  849,  850,
 /*  2820 */   853,  846,  852,  851,  845, 1331,  388, 1331, 1331,  840,
 /*  2830 */   849,  850,  853,  846,  852,  851,  845, 1331,  387, 1331,
 /*  2840 */  1331, 1331,  840,  849,  850,  853,  846,  852,  851,  845,
 /*  2850 */  1331,  386, 1331,  840,  849,  850,  853,  846,  852,  851,
 /*  2860 */   845, 1331,  385, 1331,  840,  849,  850,  853,  846,  852,
 /*  2870 */   851,  845, 1331,  384,  840,  849,  850,  853,  846,  852,
 /*  2880 */   851,  845, 1331,  229,  840,  849,  850,  853,  846,  852,
 /*  2890 */   851,  845, 1331,  228,  840,  849,  850,  853,  846,  852,
 /*  2900 */   851,  845, 1331,  374,  818,  809,  853,  846,  852,  851,
 /*  2910 */   845, 1331, 1331, 1331, 1331,  446,  818,  809,  853,  846,
 /*  2920 */   852,  851,  845, 1331, 1331, 1331, 1331,  370,  818,  809,
 /*  2930 */   853,  846,  852,  851,  845, 1331, 1331, 1331, 1331,  447,
 /*  2940 */   818,  809,  853,  846,  852,  851,  845, 1331, 1331, 1331,
 /*  2950 */  1331,  808,  818,  809,  853,  846,  852,  851,  845, 1331,
 /*  2960 */  1331, 1331, 1331,  804,  818,  809,  853,  846,  852,  851,
 /*  2970 */   845, 1331, 1331, 1331, 1331,  807,  818,  809,  853,  846,
 /*  2980 */   852,  851,  845, 1331, 1331, 1331, 1331,  806,  818,  809,
 /*  2990 */   853,  846,  852,  851,  845, 1331, 1331, 1331, 1331,  805,
 /*  3000 */   818,  809,  853,  846,  852,  851,  845, 1331, 1331, 1331,
 /*  3010 */  1331,  410,  818,  809,  853,  846,  852,  851,  845, 1331,
 /*  3020 */  1331, 1331, 1331,  439,  818,  809,  853,  846,  852,  851,
 /*  3030 */   845, 1331, 1331, 1331, 1331,  438,  818,  809,  853,  846,
 /*  3040 */   852,  851,  845, 1331, 1331, 1331, 1331,  753,  818,  809,
 /*  3050 */   853,  846,  852,  851,  845, 1331, 1331, 1331, 1331,  752,
 /*  3060 */  1331,  818,  809,  853,  846,  852,  851,  845, 1331, 1331,
 /*  3070 */  1331, 1331,  751,  818,  809,  853,  846,  852,  851,  845,
 /*  3080 */  1331, 1331, 1331, 1331,  383,  818,  809,  853,  846,  852,
 /*  3090 */   851,  845, 1331, 1331, 1331, 1331,  382,  818,  809,  853,
 /*  3100 */   846,  852,  851,  845, 1331, 1331, 1331, 1331,  381,  818,
 /*  3110 */   809,  853,  846,  852,  851,  845, 1331, 1331, 1331, 1331,
 /*  3120 */   380,  818,  809,  853,  846,  852,  851,  845, 1331, 1331,
 /*  3130 */  1331, 1331,  379,  818,  809,  853,  846,  852,  851,  845,
 /*  3140 */  1331, 1331, 1331, 1331,  378,  818,  809,  853,  846,  852,
 /*  3150 */   851,  845, 1331, 1331, 1331, 1331,  377,  818,  809,  853,
 /*  3160 */   846,  852,  851,  845, 1331, 1331, 1331, 1331,  745,  640,
 /*  3170 */   631,  853,  846,  852,  851,  845,  640,  631,  853,  846,
 /*  3180 */   852,  851,  845,  640,  631,  853,  846,  852,  851,  845,
 /*  3190 */  1331, 1331, 1331,  640,  631,  853,  846,  852,  851,  845,
 /*  3200 */  1331, 1331, 1331, 1331,  426, 1331, 1331, 1331, 1331, 1331,
 /*  3210 */  1331,  333, 1331, 1331, 1331, 1331, 1331, 1331,  427,  640,
 /*  3220 */   631,  853,  846,  852,  851,  845, 1331, 1331,  630, 1331,
 /*  3230 */   640,  631,  853,  846,  852,  851,  845,  640,  631,  853,
 /*  3240 */   846,  852,  851,  845,  640,  631,  853,  846,  852,  851,
 /*  3250 */   845, 1331, 1331, 1331,  428,  640,  631,  853,  846,  852,
 /*  3260 */   851,  845, 1331, 1331, 1331,  629, 1331, 1331, 1331, 1331,
 /*  3270 */  1331, 1331,  628, 1331, 1331, 1331, 1331, 1331, 1331,  627,
 /*  3280 */   640,  631,  853,  846,  852,  851,  845, 1331, 1331, 1331,
 /*  3290 */   626,  640,  631,  853,  846,  852,  851,  845, 1331, 1331,
 /*  3300 */  1331, 1331,  640,  631,  853,  846,  852,  851,  845, 1331,
 /*  3310 */  1331, 1331, 1331, 1331, 1331,  425,  640,  631,  853,  846,
 /*  3320 */   852,  851,  845, 1331, 1331, 1331,  625,  640,  631,  853,
 /*  3330 */   846,  852,  851,  845, 1331, 1331, 1331,  624,  640,  631,
 /*  3340 */   853,  846,  852,  851,  845,  800, 1331, 1331, 1331, 1331,
 /*  3350 */   547,  623,  640,  631,  853,  846,  852,  851,  845, 1331,
 /*  3360 */  1331, 1331,  356, 1331, 1331, 1331, 1331, 1331, 1331, 1331,
 /*  3370 */  1331,  545, 1331,  355,  208,  482,  640,  631,  853,  846,
 /*  3380 */   852,  851,  845, 1331, 1331, 1331, 1331,  354,  640,  631,
 /*  3390 */   853,  846,  852,  851,  845,  640,  631,  853,  846,  852,
 /*  3400 */   851,  845,  640,  631,  853,  846,  852,  851,  845, 1331,
 /*  3410 */  1331,  353,  640,  631,  853,  846,  852,  851,  845, 1331,
 /*  3420 */  1331, 1331, 1331,  352, 1331, 1331, 1331, 1331, 1331, 1331,
 /*  3430 */   351, 1331, 1331, 1331, 1331, 1331, 1331,  350,  640,  631,
 /*  3440 */   853,  846,  852,  851,  845, 1331, 1331,  618,  640,  631,
 /*  3450 */   853,  846,  852,  851,  845,  640,  631,  853,  846,  852,
 /*  3460 */   851,  845,  640,  631,  853,  846,  852,  851,  845,  800,
 /*  3470 */  1331, 1331, 1331,  349,  547, 1331, 1331, 1331, 1331, 1331,
 /*  3480 */  1331, 1331, 1331,  348, 1331, 1331, 1331, 1331, 1331, 1331,
 /*  3490 */   347, 1331, 1331, 1331, 1331,  543, 1331,  346,  207,  482,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */   138,   82,    0,  141,  142,  143,  144,  145,  146,  147,
 /*    10 */   148,  149,  150,  151,  152,  153,  154,  155,   74,  157,
 /*    20 */   158,  159,  160,  161,  162,  163,  107,  108,  109,  110,
 /*    30 */   111,   74,  170,  171,  157,  158,  159,  160,  161,  162,
 /*    40 */   163,  179,  180,  109,  110,  111,   74,  170,  171,  173,
 /*    50 */    69,  175,  108,   73,  177,  178,  179,  180,  181,  182,
 /*    60 */   173,  104,  175,  201,  202,  203,   74,  190,  191,  157,
 /*    70 */   158,  159,  160,  161,  162,  163,  104,   96,   97,   98,
 /*    80 */    99,  100,  170,  171,  103,  209,  210,  211,  212,  177,
 /*    90 */   178,  179,  180,  181,  182,  208,  209,  210,  211,  212,
 /*   100 */   108,  239,  240,  241,  242,  243,  244,  245,  246,  247,
 /*   110 */   248,  249,  250,  251,  252,  253,  254,  255,  256,  257,
 /*   120 */     1,    2,    1,   73,    5,    6,    7,    8,    9,   10,
 /*   130 */    11,   12,   13,   14,  107,  108,  109,  110,  111,   73,
 /*   140 */   157,  158,  159,  160,  161,  162,  163,   73,   43,   30,
 /*   150 */   109,  110,  111,   34,   73,   36,   37,   73,   39,  173,
 /*   160 */    41,  175,   74,  180,   98,   46,  109,  110,  111,   50,
 /*   170 */    51,   52,   53,   54,   55,   56,  193,   58,   59,  196,
 /*   180 */   197,  198,  199,  200,   65,   66,    1,    2,   75,   70,
 /*   190 */    71,  103,   73,   75,   73,  173,   77,  175,   92,   93,
 /*   200 */    81,   96,   97,   98,   99,  100,  100,  221,  103,   24,
 /*   210 */    25,   26,   27,   95,    1,    2,   83,   32,    5,    6,
 /*   220 */   107,  108,  109,  110,  111,   40,  107,   42,  107,   75,
 /*   230 */   100,  112,   47,  112,  104,   50,   51,   52,   72,  120,
 /*   240 */   218,  219,  220,  221,   59,   60,   61,   62,   75,   95,
 /*   250 */    65,   66,  133,  134,  135,   74,  137,   73,   73,   35,
 /*   260 */    28,   48,   38,   50,   51,   52,    1,    2,   95,   45,
 /*   270 */     5,    6,  173,   49,  175,   43,   44,  104,   65,   66,
 /*   280 */    72,   68,  101,  102,   71,  173,   73,  175,    1,    2,
 /*   290 */    77,   43,  107,  101,  102,   63,   64,  112,   75,  157,
 /*   300 */   158,  159,  160,  161,  162,  163,   74,   75,  209,  210,
 /*   310 */   211,  212,   72,   81,  172,   50,   51,   52,   95,  106,
 /*   320 */   107,  209,  210,  211,  212,  112,  184,  185,  186,   81,
 /*   330 */    65,   66,   78,   68,   72,  103,   71,  103,   73,   72,
 /*   340 */     1,    2,   77,   71,    5,    6,  133,  134,  135,   28,
 /*   350 */   103,  103,  166,   43,   82,   83,   84,   85,  104,   87,
 /*   360 */    88,   89,   90,   96,   97,   98,   99,  100,   92,   93,
 /*   370 */   103,  106,  107,  187,   99,  100,  100,  112,  103,  107,
 /*   380 */   108,  109,  110,  111,  158,   78,   83,  166,   67,   50,
 /*   390 */    51,   52,    1,    2,    1,  169,    5,    6,  133,  134,
 /*   400 */   135,  213,  214,  215,   65,   66,  180,   68,  187,  183,
 /*   410 */    71,  104,   73,  103,  228,  229,   77,   96,   97,   98,
 /*   420 */    99,  100,  135,   83,  103,  157,  158,  159,  160,  161,
 /*   430 */   162,  163,    1,    2,   97,   98,   99,  100,  170,  171,
 /*   440 */   103,   50,   51,   52,  166,  106,  107,  179,  180,  228,
 /*   450 */   229,  112,   97,   98,   99,  100,   65,   66,  103,   91,
 /*   460 */     1,    2,   71,  100,   73,  187,   73,  104,   77,  201,
 /*   470 */   202,  203,  133,  134,  135,  107,  108,  109,  110,  111,
 /*   480 */    78,   50,   51,   52,  109,  110,  111,   75,  157,  158,
 /*   490 */   159,  160,  161,  162,  163,   75,   65,   66,  107,   75,
 /*   500 */   107,  170,  171,  112,   73,  112,  104,  229,  177,  178,
 /*   510 */   179,  180,  181,  182,  157,  158,  159,  160,  161,  162,
 /*   520 */   163,  190,  191,   74,  133,  134,  135,  170,  171,  157,
 /*   530 */   158,  159,  160,  161,  162,  163,    1,    2,  107,   33,
 /*   540 */     5,    6,   75,  112,  109,   99,  100,   74,  113,  103,
 /*   550 */    99,  100,  180,  104,  103,   82,  157,  158,  159,  160,
 /*   560 */   161,  162,  163,   74,   74,  193,    3,    4,  196,  197,
 /*   570 */   198,  199,  200,    1,    2,  204,  205,    5,    6,  180,
 /*   580 */   107,  108,  109,  110,  111,   50,   51,   52,   33,  190,
 /*   590 */   191,   74,  193,  104,  104,  196,  197,  198,  199,  200,
 /*   600 */    65,   66,  230,  231,  232,  222,  223,  224,   73,   74,
 /*   610 */   109,  110,  111,   74,  157,  158,  159,  160,  161,  162,
 /*   620 */   163,  104,   50,   51,   52,   77,   75,  170,  171,  157,
 /*   630 */   158,  159,  160,  161,  162,  163,   78,   65,   66,   75,
 /*   640 */    75,   83,  107,  104,  172,   73,   95,  112,   75,  157,
 /*   650 */   158,  159,  160,  161,  162,  163,  184,  185,  186,   95,
 /*   660 */   157,  158,  159,  160,  161,  162,  163,   74,  133,  134,
 /*   670 */   135,   76,  180,  170,  171,  225,  226,  227,  106,  107,
 /*   680 */    74,  133,  190,  191,  112,  193,  158,   78,  196,  197,
 /*   690 */   198,  199,  200,  157,  158,  159,  160,  161,  162,  163,
 /*   700 */   107,  108,  109,  110,  111,  133,  134,  135,  180,   71,
 /*   710 */   104,   75,   74,  104,    1,    2,  180,   43,    5,    6,
 /*   720 */    82,   83,   84,   85,  108,   87,   88,   89,   90,  193,
 /*   730 */   202,   95,  196,  197,  198,  199,  200,    2,    1,    2,
 /*   740 */   104,  166,    5,    6,   98,  107,  108,  109,  110,  111,
 /*   750 */     1,    2,   75,   78,    5,    6,  166,   71,   83,   76,
 /*   760 */    74,   77,  187,   50,   51,   52,   73,  231,  232,   83,
 /*   770 */    84,   85,   95,   87,   88,   89,   90,  187,   65,   66,
 /*   780 */    72,  107,  108,  109,  110,  111,   73,   50,   51,   52,
 /*   790 */    57,  107,  166,  107,  108,  109,  110,  111,  158,   50,
 /*   800 */    51,   52,   65,   66,   96,   97,   98,   99,  100,  169,
 /*   810 */    73,  103,   77,  187,   65,   66,    2,  133,   74,  229,
 /*   820 */   107,  173,   73,  175,   77,  112,   73,   83,   84,   85,
 /*   830 */    75,   87,   88,   89,   90,    2,  156,  157,  158,  159,
 /*   840 */   160,  161,  162,  163,  107,  165,  133,  134,  135,  112,
 /*   850 */    95,  107,  108,  109,  110,  111,  107,  209,  210,  211,
 /*   860 */   212,  112,    2,  158,  216,  217,  214,  215,  133,   29,
 /*   870 */   133,  134,  135,  157,  158,  159,  160,  161,  162,  163,
 /*   880 */   133,  134,  133,  134,  135,  180,  170,  171,  107,  108,
 /*   890 */   109,  110,  111,  177,  178,  179,  180,  181,  182,    2,
 /*   900 */   157,  158,  159,  160,  161,  162,  163,  202,  223,  224,
 /*   910 */    72,  188,  189,  170,  171,   74,   73,   77,    2,   74,
 /*   920 */   177,  178,  179,  180,  181,  182,  157,  158,  159,  160,
 /*   930 */   161,  162,  163,   73,   96,   97,   98,   99,  100,  170,
 /*   940 */   171,  103,   92,   93,   73,  104,  177,  178,  179,  180,
 /*   950 */   181,  182,  107,  108,  109,  110,  111,  157,  158,  159,
 /*   960 */   160,  161,  162,  163,  206,  207,  133,   72,  226,  227,
 /*   970 */   170,  171,   74,  133,  173,   73,  175,  177,  178,  179,
 /*   980 */   180,  181,  182,  157,  158,  159,  160,  161,  162,  163,
 /*   990 */    73,   96,   97,   98,   99,  100,  170,  171,  103,  174,
 /*  1000 */    74,  176,  104,  177,  178,  179,  180,  181,  182,  208,
 /*  1010 */   209,  210,  211,  212,  157,  158,  159,  160,  161,  162,
 /*  1020 */   163,   69,   96,   97,   98,   99,  100,  170,  171,  103,
 /*  1030 */    75,  173,  104,  175,  177,  178,  179,  180,  181,  182,
 /*  1040 */   157,  158,  159,  160,  161,  162,  163,    2,   96,   97,
 /*  1050 */    98,   99,  100,  170,  171,  103,   75,   74,  226,  227,
 /*  1060 */   177,  178,  179,  180,  181,  182,  208,  209,  210,  211,
 /*  1070 */   212,  157,  158,  159,  160,  161,  162,  163,  103,   96,
 /*  1080 */    97,   98,   99,  100,  170,  171,  103,   78,  173,   91,
 /*  1090 */   175,  177,  178,  179,  180,  181,  182,  157,  158,  159,
 /*  1100 */   160,  161,  162,  163,   78,  107,  108,  109,  110,  111,
 /*  1110 */   170,  171,   72,  226,  227,   74,   67,  177,  178,  179,
 /*  1120 */   180,  181,  182,  208,  209,  210,  211,  212,  157,  158,
 /*  1130 */   159,  160,  161,  162,  163,   75,   96,   97,   98,   99,
 /*  1140 */   100,  170,  171,  103,   75,  104,   75,   74,  177,  178,
 /*  1150 */   179,  180,  181,  182,  157,  158,  159,  160,  161,  162,
 /*  1160 */   163,   75,  107,  108,  109,  110,  111,  170,  171,   72,
 /*  1170 */   226,  227,  188,  189,  177,  178,  179,  180,  181,  182,
 /*  1180 */   107,  108,  109,  110,  111,  157,  158,  159,  160,  161,
 /*  1190 */   162,  163,  133,   96,   97,   98,   99,  100,  170,  171,
 /*  1200 */   103,  139,  140,   75,   74,  177,  178,  179,  180,  181,
 /*  1210 */   182,  157,  158,  159,  160,  161,  162,  163,  166,  107,
 /*  1220 */   108,  109,  110,  111,  170,  171,   72,   75,   75,  161,
 /*  1230 */    75,  177,  178,  179,  180,  181,  182,  107,  108,  109,
 /*  1240 */   110,  111,  157,  158,  159,  160,  161,  162,  163,   75,
 /*  1250 */    96,   97,   98,   99,  100,  170,  171,  103,   75,   75,
 /*  1260 */    75,   74,  177,  178,  179,  180,  181,  182,  157,  158,
 /*  1270 */   159,  160,  161,  162,  163,   75,  107,  108,  109,  110,
 /*  1280 */   111,  170,  171,   72,   75,  134,  161,  158,  177,  178,
 /*  1290 */   179,  180,  181,  182,  107,  108,  109,  110,  111,  157,
 /*  1300 */   158,  159,  160,  161,  162,  163,  133,   96,   97,   98,
 /*  1310 */    99,  100,  170,  171,  103,  236,  173,  161,  175,  177,
 /*  1320 */   178,  179,  180,  181,  182,  157,  158,  159,  160,  161,
 /*  1330 */   162,  163,   44,   96,   97,   98,   99,  100,  170,  171,
 /*  1340 */   103,  235,   74,   28,  237,  177,  178,  179,  180,  181,
 /*  1350 */   182,   29,  209,  210,  211,  212,  157,  158,  159,  160,
 /*  1360 */   161,  162,  163,   63,   96,   97,   98,   99,  100,  170,
 /*  1370 */   171,  103,  238,   64,  237,  158,  177,  178,  179,  180,
 /*  1380 */   181,  182,  157,  158,  159,  160,  161,  162,  163,   64,
 /*  1390 */    96,   97,   98,   99,  100,  170,  171,  103,  238,   77,
 /*  1400 */   237,  158,  177,  178,  179,  180,  181,  182,  107,  108,
 /*  1410 */   109,  110,  111,  157,  158,  159,  160,  161,  162,  163,
 /*  1420 */   238,   96,   97,   98,   99,  100,  170,  171,  103,  237,
 /*  1430 */   236,  238,  237,  177,  178,  179,  180,  181,  182,  157,
 /*  1440 */   158,  159,  160,  161,  162,  163,   75,   96,   97,   98,
 /*  1450 */    99,  100,  170,  171,  103,  133,  238,  205,    2,  177,
 /*  1460 */   178,  179,  180,  181,  182,  212,    2,  235,  237,  236,
 /*  1470 */   157,  158,  159,  160,  161,  162,  163,  238,  107,  108,
 /*  1480 */   109,  110,  111,  170,  171,   71,  235,  237,   74,  158,
 /*  1490 */   177,  178,  179,  180,  181,  182,  238,   83,   84,   85,
 /*  1500 */    28,   87,   88,   89,   90,  157,  158,  159,  160,  161,
 /*  1510 */   162,  163,  237,  237,   97,   98,   99,  100,  170,  171,
 /*  1520 */   103,  107,  108,  109,  110,  111,  237,  179,  180,  238,
 /*  1530 */   238,  238,  237,  234,  157,  158,  159,  160,  161,  162,
 /*  1540 */   163,  238,   43,  236,  238,  235,  158,  170,  171,  201,
 /*  1550 */   202,  203,  234,  237,  237,  236,  179,  180,  235,  238,
 /*  1560 */   157,  158,  159,  160,  161,  162,  163,  158,   96,   97,
 /*  1570 */    98,   99,  100,  170,  171,  103,  234,  236,  201,  202,
 /*  1580 */   203,  235,  179,  180,  237,  157,  158,  159,  160,  161,
 /*  1590 */   162,  163,  238,  237,  158,  238,  238,  234,  170,  171,
 /*  1600 */   234,  207,  234,  234,  201,  202,  203,  179,  180,  234,
 /*  1610 */   157,  158,  159,  160,  161,  162,  163,  160,  189,  176,
 /*  1620 */   173,  176,  175,  170,  171,  176,   75,  176,  176,  201,
 /*  1630 */   202,  203,  179,  180,  157,  158,  159,  160,  161,  162,
 /*  1640 */   163,  176,  157,  158,  159,  160,  161,  162,  163,  176,
 /*  1650 */   176,  176,  176,  234,  201,  202,  203,  180,  107,  108,
 /*  1660 */   109,  110,  111,  176,  176,  180,  219,  220,  221,  176,
 /*  1670 */   193,  176,  176,  196,  197,  198,  199,  200,  193,  238,
 /*  1680 */   234,  196,  197,  198,  199,  200,  157,  158,  159,  160,
 /*  1690 */   161,  162,  163,  238,  157,  158,  159,  160,  161,  162,
 /*  1700 */   163,  235,  235,  235,  234,  175,  235,  175,  175,  180,
 /*  1710 */   175,  175,    2,  175,  175,  175,  175,  180,    2,  258,
 /*  1720 */   258,   75,  193,  258,  258,  196,  197,  198,  199,  200,
 /*  1730 */   193,  258,  258,  196,  197,  198,  199,  200,  157,  158,
 /*  1740 */   159,  160,  161,  162,  163,  258,  157,  158,  159,  160,
 /*  1750 */   161,  162,  163,  107,  108,  109,  110,  111,  258,  258,
 /*  1760 */   258,  180,  157,  158,  159,  160,  161,  162,  163,  180,
 /*  1770 */   258,  258,  167,  168,  193,  258,  258,  196,  197,  198,
 /*  1780 */   199,  200,  193,  258,  258,  196,  197,  198,  199,  200,
 /*  1790 */   157,  158,  159,  160,  161,  162,  163,  258,  157,  158,
 /*  1800 */   159,  160,  161,  162,  163,  156,  157,  158,  159,  160,
 /*  1810 */   161,  162,  163,  180,  165,  258,  258,  258,  258,  258,
 /*  1820 */   258,  180,  258,  258,  258,  258,  193,  258,  258,  196,
 /*  1830 */   197,  198,  199,  200,  193,  258,  258,  196,  197,  198,
 /*  1840 */   199,  200,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  1850 */   157,  158,  159,  160,  161,  162,  163,  156,  157,  158,
 /*  1860 */   159,  160,  161,  162,  163,  180,  165,  258,  258,  258,
 /*  1870 */   258,  258,  258,  180,  258,  258,  258,  258,  193,  258,
 /*  1880 */   258,  196,  197,  198,  199,  200,  193,  258,  258,  196,
 /*  1890 */   197,  198,  199,  200,  157,  158,  159,  160,  161,  162,
 /*  1900 */   163,  258,   71,  258,    1,    2,  258,  258,    5,    6,
 /*  1910 */    71,  258,  258,  258,   83,   84,   85,  180,   87,   88,
 /*  1920 */    89,   90,   83,   84,   85,  258,   87,   88,   89,   90,
 /*  1930 */   193,  258,  258,  196,  197,  198,  199,  200,  107,  108,
 /*  1940 */   109,  110,  111,  258,  258,  258,  107,  108,  109,  110,
 /*  1950 */   111,  258,  258,   50,   51,   52,  258,   83,   84,   85,
 /*  1960 */   258,   87,   88,   89,   90,   83,   84,   85,  258,   87,
 /*  1970 */    88,   89,   90,   83,   84,   85,   73,   87,   88,   89,
 /*  1980 */    90,  107,  108,  109,  110,  111,  258,  258,  258,  107,
 /*  1990 */   108,  109,  110,  111,  258,  258,  258,  107,  108,  109,
 /*  2000 */   110,  111,   83,   84,   85,  258,   87,   88,   89,   90,
 /*  2010 */   107,  258,  258,  258,  258,  112,  258,   83,   84,   85,
 /*  2020 */   258,   87,   88,   89,   90,  258,  107,  108,  109,  110,
 /*  2030 */   111,  258,  258,   75,  258,  258,  133,  134,  135,  258,
 /*  2040 */   258,  107,  108,  109,  110,  111,  258,  258,  157,  158,
 /*  2050 */   159,  160,  161,  162,  163,  157,  158,  159,  160,  161,
 /*  2060 */   162,  163,  258,  172,  258,  107,  108,  109,  110,  111,
 /*  2070 */   172,  258,  258,  258,  258,  184,  185,  186,  258,  258,
 /*  2080 */   258,  258,  184,  185,  186,  157,  158,  159,  160,  161,
 /*  2090 */   162,  163,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  2100 */   172,  258,  258,  258,  258,  258,   75,  172,  258,  258,
 /*  2110 */   258,  258,  184,  185,  186,  258,  258,  258,  258,  184,
 /*  2120 */   185,  186,  157,  158,  159,  160,  161,  162,  163,  157,
 /*  2130 */   158,  159,  160,  161,  162,  163,  258,  172,  107,  108,
 /*  2140 */   109,  110,  111,  258,  172,  258,  258,  258,  258,  184,
 /*  2150 */   185,  186,    1,    2,  258,  258,  184,  185,  186,  157,
 /*  2160 */   158,  159,  160,  161,  162,  163,  157,  158,  159,  160,
 /*  2170 */   161,  162,  163,   16,  172,   18,   19,   20,   21,   22,
 /*  2180 */    23,  172,  258,  258,  258,  258,  184,  185,  186,  258,
 /*  2190 */   258,  258,  258,  184,  185,  186,  258,  258,  258,  258,
 /*  2200 */   258,   50,   51,   52,  258,  258,  258,   50,   51,  157,
 /*  2210 */   158,  159,  160,  161,  162,  163,   65,   66,  258,  258,
 /*  2220 */   168,  258,  258,  258,   73,  258,  258,  156,  157,  158,
 /*  2230 */   159,  160,  161,  162,  163,  164,  165,  258,  258,  156,
 /*  2240 */   157,  158,  159,  160,  161,  162,  163,  164,  165,  258,
 /*  2250 */   156,  157,  158,  159,  160,  161,  162,  163,  107,  165,
 /*  2260 */     1,  258,  258,  112,  156,  157,  158,  159,  160,  161,
 /*  2270 */   162,  163,  164,  165,  258,  258,  156,  157,  158,  159,
 /*  2280 */   160,  161,  162,  163,  164,  165,  258,  258,  258,  258,
 /*  2290 */   258,  258,  258,  136,  156,  157,  158,  159,  160,  161,
 /*  2300 */   162,  163,  164,  165,  258,  258,  258,  258,  258,   50,
 /*  2310 */    51,   52,  258,  258,  156,  157,  158,  159,  160,  161,
 /*  2320 */   162,  163,  258,  165,   65,   66,  258,  258,  258,  258,
 /*  2330 */   258,  258,   73,  156,  157,  158,  159,  160,  161,  162,
 /*  2340 */   163,  258,  165,  156,  157,  158,  159,  160,  161,  162,
 /*  2350 */   163,  258,  165,  258,  258,  258,  258,  258,  156,  157,
 /*  2360 */   158,  159,  160,  161,  162,  163,  107,  165,  258,  258,
 /*  2370 */   258,  112,  258,  156,  157,  158,  159,  160,  161,  162,
 /*  2380 */   163,  258,  165,  258,  156,  157,  158,  159,  160,  161,
 /*  2390 */   162,  163,  258,  165,  258,  258,  156,  157,  158,  159,
 /*  2400 */   160,  161,  162,  163,  258,  165,  258,  156,  157,  158,
 /*  2410 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2420 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2430 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2440 */   159,  160,  161,  162,  163,  258,  165,  258,  258,  258,
 /*  2450 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2460 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2470 */   258,  156,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  2480 */   165,  156,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  2490 */   165,  156,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  2500 */   165,  156,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  2510 */   165,  258,  258,  258,  258,  156,  157,  158,  159,  160,
 /*  2520 */   161,  162,  163,  258,  165,  258,  258,  258,  156,  157,
 /*  2530 */   158,  159,  160,  161,  162,  163,  258,  165,  258,  156,
 /*  2540 */   157,  158,  159,  160,  161,  162,  163,  258,  165,  258,
 /*  2550 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2560 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2570 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2580 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2590 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2600 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2610 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2620 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2630 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2640 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2650 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2660 */   156,  157,  158,  159,  160,  161,  162,  163,  258,  165,
 /*  2670 */   258,  258,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  2680 */   258,  165,  258,  258,  258,  156,  157,  158,  159,  160,
 /*  2690 */   161,  162,  163,  258,  165,  258,  156,  157,  158,  159,
 /*  2700 */   160,  161,  162,  163,  258,  165,  258,  156,  157,  158,
 /*  2710 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2720 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2730 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2740 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2750 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2760 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2770 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2780 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2790 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2800 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2810 */   159,  160,  161,  162,  163,  258,  165,  156,  157,  158,
 /*  2820 */   159,  160,  161,  162,  163,  258,  165,  258,  258,  156,
 /*  2830 */   157,  158,  159,  160,  161,  162,  163,  258,  165,  258,
 /*  2840 */   258,  258,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  2850 */   258,  165,  258,  156,  157,  158,  159,  160,  161,  162,
 /*  2860 */   163,  258,  165,  258,  156,  157,  158,  159,  160,  161,
 /*  2870 */   162,  163,  258,  165,  156,  157,  158,  159,  160,  161,
 /*  2880 */   162,  163,  258,  165,  156,  157,  158,  159,  160,  161,
 /*  2890 */   162,  163,  258,  165,  156,  157,  158,  159,  160,  161,
 /*  2900 */   162,  163,  258,  165,  157,  158,  159,  160,  161,  162,
 /*  2910 */   163,  258,  258,  258,  258,  168,  157,  158,  159,  160,
 /*  2920 */   161,  162,  163,  258,  258,  258,  258,  168,  157,  158,
 /*  2930 */   159,  160,  161,  162,  163,  258,  258,  258,  258,  168,
 /*  2940 */   157,  158,  159,  160,  161,  162,  163,  258,  258,  258,
 /*  2950 */   258,  168,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  2960 */   258,  258,  258,  168,  157,  158,  159,  160,  161,  162,
 /*  2970 */   163,  258,  258,  258,  258,  168,  157,  158,  159,  160,
 /*  2980 */   161,  162,  163,  258,  258,  258,  258,  168,  157,  158,
 /*  2990 */   159,  160,  161,  162,  163,  258,  258,  258,  258,  168,
 /*  3000 */   157,  158,  159,  160,  161,  162,  163,  258,  258,  258,
 /*  3010 */   258,  168,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  3020 */   258,  258,  258,  168,  157,  158,  159,  160,  161,  162,
 /*  3030 */   163,  258,  258,  258,  258,  168,  157,  158,  159,  160,
 /*  3040 */   161,  162,  163,  258,  258,  258,  258,  168,  157,  158,
 /*  3050 */   159,  160,  161,  162,  163,  258,  258,  258,  258,  168,
 /*  3060 */   258,  157,  158,  159,  160,  161,  162,  163,  258,  258,
 /*  3070 */   258,  258,  168,  157,  158,  159,  160,  161,  162,  163,
 /*  3080 */   258,  258,  258,  258,  168,  157,  158,  159,  160,  161,
 /*  3090 */   162,  163,  258,  258,  258,  258,  168,  157,  158,  159,
 /*  3100 */   160,  161,  162,  163,  258,  258,  258,  258,  168,  157,
 /*  3110 */   158,  159,  160,  161,  162,  163,  258,  258,  258,  258,
 /*  3120 */   168,  157,  158,  159,  160,  161,  162,  163,  258,  258,
 /*  3130 */   258,  258,  168,  157,  158,  159,  160,  161,  162,  163,
 /*  3140 */   258,  258,  258,  258,  168,  157,  158,  159,  160,  161,
 /*  3150 */   162,  163,  258,  258,  258,  258,  168,  157,  158,  159,
 /*  3160 */   160,  161,  162,  163,  258,  258,  258,  258,  168,  157,
 /*  3170 */   158,  159,  160,  161,  162,  163,  157,  158,  159,  160,
 /*  3180 */   161,  162,  163,  157,  158,  159,  160,  161,  162,  163,
 /*  3190 */   258,  258,  258,  157,  158,  159,  160,  161,  162,  163,
 /*  3200 */   258,  258,  258,  258,  192,  258,  258,  258,  258,  258,
 /*  3210 */   258,  192,  258,  258,  258,  258,  258,  258,  192,  157,
 /*  3220 */   158,  159,  160,  161,  162,  163,  258,  258,  192,  258,
 /*  3230 */   157,  158,  159,  160,  161,  162,  163,  157,  158,  159,
 /*  3240 */   160,  161,  162,  163,  157,  158,  159,  160,  161,  162,
 /*  3250 */   163,  258,  258,  258,  192,  157,  158,  159,  160,  161,
 /*  3260 */   162,  163,  258,  258,  258,  192,  258,  258,  258,  258,
 /*  3270 */   258,  258,  192,  258,  258,  258,  258,  258,  258,  192,
 /*  3280 */   157,  158,  159,  160,  161,  162,  163,  258,  258,  258,
 /*  3290 */   192,  157,  158,  159,  160,  161,  162,  163,  258,  258,
 /*  3300 */   258,  258,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  3310 */   258,  258,  258,  258,  258,  192,  157,  158,  159,  160,
 /*  3320 */   161,  162,  163,  258,  258,  258,  192,  157,  158,  159,
 /*  3330 */   160,  161,  162,  163,  258,  258,  258,  192,  157,  158,
 /*  3340 */   159,  160,  161,  162,  163,  161,  258,  258,  258,  258,
 /*  3350 */   166,  192,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  3360 */   258,  258,  192,  258,  258,  258,  258,  258,  258,  258,
 /*  3370 */   258,  187,  258,  192,  190,  191,  157,  158,  159,  160,
 /*  3380 */   161,  162,  163,  258,  258,  258,  258,  192,  157,  158,
 /*  3390 */   159,  160,  161,  162,  163,  157,  158,  159,  160,  161,
 /*  3400 */   162,  163,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  3410 */   258,  192,  157,  158,  159,  160,  161,  162,  163,  258,
 /*  3420 */   258,  258,  258,  192,  258,  258,  258,  258,  258,  258,
 /*  3430 */   192,  258,  258,  258,  258,  258,  258,  192,  157,  158,
 /*  3440 */   159,  160,  161,  162,  163,  258,  258,  192,  157,  158,
 /*  3450 */   159,  160,  161,  162,  163,  157,  158,  159,  160,  161,
 /*  3460 */   162,  163,  157,  158,  159,  160,  161,  162,  163,  161,
 /*  3470 */   258,  258,  258,  192,  166,  258,  258,  258,  258,  258,
 /*  3480 */   258,  258,  258,  192,  258,  258,  258,  258,  258,  258,
 /*  3490 */   192,  258,  258,  258,  258,  187,  258,  192,  190,  191,
};
#define YY_SHIFT_USE_DFLT (-82)
#define YY_SHIFT_COUNT (553)
#define YY_SHIFT_MIN   (-81)
#define YY_SHIFT_MAX   (2259)
static const short yy_shift_ofst[] = {
 /*     0 */   -82,  119,  213,  339,  339,  213,  265,  265,  339,  339,
 /*    10 */   339,  339,  339,  339,  339,  339,  339,  339,  339,  339,
 /*    20 */   339,  339,  339,  339,  339,  339,  339,  339,  339,  339,
 /*    30 */   339,  265,  265,  265,  265,  265,  265,  265,  265,  265,
 /*    40 */   265,  265,  265,  391,  391,  391,  391,  391,  391,  572,
 /*    50 */   572,  572,  572,  572,  572,  572,  572,  572,  572,  535,
 /*    60 */   749,  749,  749,  749,  749,  749,  749,  749,  749,  749,
 /*    70 */   749,  749,  749,  749,  749,  749,  749,  749,  749,  749,
 /*    80 */   749,  749,  749,  749,  749,  749,  749,  749,  749,  749,
 /*    90 */   749,  749,  749,  749,  749,  749,  749,  749,  749,  749,
 /*   100 */   749,  713,  749,  749,  749,  749,  749,  749,  749,  749,
 /*   110 */   749,  749,  749,  749,  749,  749,  749,  749,  749,  749,
 /*   120 */   749,  749,  749, 1903, 1903, 1903,  185,  737,  737,  737,
 /*   130 */   737,  737,  737,  737,  737,  737,  737,  737,  737,  737,
 /*   140 */   737,  737,  737,  737,  737,  737,  737,  737,  737,  737,
 /*   150 */   737,  737,  713,  713,  713,  713,  713,  713,  713,  713,
 /*   160 */   713,  713,  713,  713,  713,  713,  713,  713,  713,  713,
 /*   170 */   713,  713,  713,  713,  713, 2151, 2151, 2151, 2151, 2151,
 /*   180 */   431, 2151, 2151, 2151,  431,  747,  747, 1322,  840,  735,
 /*   190 */   431,  684,  684,  548,  548,  860, 1716, 1710,  393,  850,
 /*   200 */   860,  860,  860,  860,  563,  833,  850,  548,  548, 1716,
 /*   210 */  1710, 1456, 2259, 2259, 2259, 2259, 2259, 2259, 2259, 2259,
 /*   220 */  2259,  321,  105, 1472, 1472, 1472, 1325, 1325,  674,  674,
 /*   230 */   121,  121,  121,  121,  121,  121,  121,  121,  121,  121,
 /*   240 */   121,  121,  121,  121,  121,  276,  287,  106,  248,  310,
 /*   250 */   310,  563,  310,  310, 1309, 1173, 1309, 1300, 1309, 1300,
 /*   260 */  1315, 1288, 1499, 1173, 1309, 1300, 1315, 1288, 1499, 1173,
 /*   270 */  1309, 1300, 1315, 1288, 1499, 1173, 1309, 1300, 1309, 1300,
 /*   280 */  1309, 1300, 1309, 1300, 1309, 1300, 1315, 1288, 1309, 1300,
 /*   290 */  1315, 1288, 1464, 1456, 1309, 1300, 1309, 1300, 1173, 1309,
 /*   300 */  1300, 1173, 1309, 1300, 1309, 1300, 1315, 1288, 1151, 1151,
 /*   310 */  1173, 1151, 1059,  638,  272, 1414,  686,  744, 1839, 1831,
 /*   320 */  1934, 1919, 1890, 1882, 1874, 2157,  232, 1268, 1211, 1154,
 /*   330 */  1097, 1040,  952,  473,  983,  926,  895,  838,  708,  267,
 /*   340 */   -19, 2031, 1958, 1351, 1351,  998,  -81,  -81,  -81,  -81,
 /*   350 */   -81,  -81,  -81,  -81,  -81,  -81,  -81, 1237, 1646, 1551,
 /*   360 */  1371,  113, 1187,  368, 1130, 1294, 1237, 1237, 1237, 1073,
 /*   370 */   845,  593, 1301, 1417,   27, 1169,  355,  781,  781,  781,
 /*   380 */   781,  781,  781,  781,   27,   27,   27,   27,   27,  337,
 /*   390 */    27,   27,   27,   27,   27,   27,   27,   27,   27,   27,
 /*   400 */    27,   27,   27,   27,   27,   27, 1112, 1055,   27,   27,
 /*   410 */   781,  781,  781,   27,   27,   27,   27,   27,   27,   27,
 /*   420 */    27,   27,  224,  451,  451,  501,  501,  501,  501,  375,
 /*   430 */   375,  636,  173,  181,   57,   57,  446,  446,   41,   41,
 /*   440 */   275,  275,  -66,  -66,  -66,  -66,   41,   41,  -66,  -66,
 /*   450 */   -66,  -66,  459,  755,  677,  675,  558,  192,  192,  192,
 /*   460 */   192,  564,  609,  551, 1041,  402,  223,  307,  898,  841,
 /*   470 */    -8,  606,  -56,  435,  254,  154,  539,   66,  118,  517,
 /*   480 */   363,   88,  130,  490,  489,  449,  -28,  -43, 1049, 1209,
 /*   490 */  1200, 1185, 1184, 1183, 1174, 1155, 1153, 1152, 1128, 1086,
 /*   500 */  1071, 1069, 1060, 1026, 1009,  975,  975,  981,  955, 1045,
 /*   510 */   928,  916,  616,  917,  902,  871,  897,  843,  616,  616,
 /*   520 */   753,  814,  733,  693,  616,  683,  595,  646,  573,  565,
 /*   530 */   555,  467,  506,  424,  420,  412,  247,  247,  340,  303,
 /*   540 */   184,  234,  234,  262,  240,  208,  166,  133,   84,   81,
 /*   550 */    74,   50,  -20,    2,
};
#define YY_REDUCE_USE_DFLT (-139)
#define YY_REDUCE_COUNT (312)
#define YY_REDUCE_MIN   (-138)
#define YY_REDUCE_MAX   (3308)
static const short yy_reduce_ofst[] = {
 /*     0 */  1062, -138,  372,  331, -123,  536,  492,  399, 1313, 1282,
 /*    10 */  1256, 1225, 1199, 1168, 1142, 1111, 1085, 1054, 1028,  997,
 /*    20 */   971,  940,  914,  883,  857,  826,  800,  769,  743,  716,
 /*    30 */   -88, 1737, 1693, 1685, 1641, 1633, 1589, 1581, 1537, 1529,
 /*    40 */  1485, 1477,  -17, 1453, 1428, 1403, 1377, 1348,  268, 2009,
 /*    50 */  2002, 1972, 1965, 1935, 1928, 1898, 1891,  472,  142, 2138,
 /*    60 */  2120, 2108, 2083, 2071, 2738, 2728, 2718, 2708, 2697, 2686,
 /*    70 */  2673, 2661, 2651, 2641, 2631, 2621, 2611, 2601, 2591, 2581,
 /*    80 */  2571, 2561, 2551, 2540, 2529, 2516, 2504, 2494, 2484, 2474,
 /*    90 */  2464, 2454, 2444, 2434, 2424, 2414, 2404, 2394, 2383, 2372,
 /*   100 */  2359, 1605, 2345, 2335, 2325, 2315, 2304, 2294, 2281, 2271,
 /*   110 */  2261, 2251, 2240, 2228, 2217, 2202, 2187, 2177, 2158, 2094,
 /*   120 */  1701, 1649,  680,  503,  457,  357,  648, 3305, 3298, 3291,
 /*   130 */  3281, 3255, 3245, 3238, 3231, 3219, 3195, 3181, 3170, 3159,
 /*   140 */  3145, 3134, 3123, 3098, 3087, 3080, 3073, 3062, 3036, 3026,
 /*   150 */  3019, 3012, 3000, 2988, 2976, 2964, 2952, 2940, 2928, 2916,
 /*   160 */  2904, 2891, 2879, 2867, 2855, 2843, 2831, 2819, 2807, 2795,
 /*   170 */  2783, 2771, 2759, 2747, 2052,  915,  858,  801, -113, 1143,
 /*   180 */    22,  112,   99, -124, 1447, 3308, 3184,  221,  186,  226,
 /*   190 */   -14,  705,  528,  590,  278,  450,  383,  188,  825,  984,
 /*   200 */   944,  887,  832,  742,  758,  640,  723,  626,  575,  685,
 /*   210 */   652,  371, 1541, 1540, 1539, 1538, 1536, 1535, 1533, 1532,
 /*   220 */  1530, 1471, 1470, 1468, 1467, 1466, 1455, 1441, 1446, 1419,
 /*   230 */  1496, 1495, 1493, 1488, 1487, 1476, 1475, 1474, 1473, 1465,
 /*   240 */  1452, 1451, 1449, 1445, 1443, 1429, 1457, 1429, 1375, 1369,
 /*   250 */  1368, 1394, 1366, 1363, 1358, 1436, 1357, 1356, 1354, 1347,
 /*   260 */  1346, 1341, 1342, 1409, 1321, 1317, 1323, 1319, 1318, 1388,
 /*   270 */  1306, 1316, 1310, 1307, 1299, 1331, 1303, 1295, 1293, 1289,
 /*   280 */  1292, 1276, 1291, 1275, 1258, 1250, 1251, 1233, 1239, 1231,
 /*   290 */  1232, 1194, 1253, 1252, 1218, 1195, 1193, 1192, 1243, 1182,
 /*   300 */  1163, 1217, 1160, 1137, 1134, 1107, 1106, 1079, 1156, 1125,
 /*   310 */  1129, 1068, 1052,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   871, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    10 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    20 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    30 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    40 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    50 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    60 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1083, 1087,
 /*    70 */  1082, 1086, 1172, 1168, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    80 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*    90 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1173,
 /*   100 */  1169, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   110 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   120 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1152, 1156, 1151,
 /*   130 */  1155, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   140 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   150 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   160 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   170 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   180 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   190 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   200 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   210 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   220 */  1329, 1275, 1273, 1275, 1275, 1275, 1281, 1281, 1273, 1273,
 /*   230 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   240 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1273, 1273,
 /*   250 */  1273, 1329, 1273, 1273, 1281, 1329, 1281, 1279, 1281, 1279,
 /*   260 */  1275, 1277, 1273, 1329, 1281, 1279, 1275, 1277, 1273, 1329,
 /*   270 */  1281, 1279, 1275, 1277, 1273, 1329, 1281, 1279, 1281, 1279,
 /*   280 */  1281, 1279, 1281, 1279, 1281, 1279, 1275, 1277, 1281, 1279,
 /*   290 */  1275, 1277, 1329, 1329, 1281, 1279, 1281, 1279, 1329, 1281,
 /*   300 */  1279, 1329, 1281, 1279, 1281, 1279, 1275, 1277, 1329, 1329,
 /*   310 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   320 */  1118, 1329, 1047, 1047, 1329, 1329,  938, 1329, 1329, 1329,
 /*   330 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   340 */  1329, 1329, 1329, 1265, 1262, 1329, 1154, 1158, 1153, 1157,
 /*   350 */  1149, 1148, 1147, 1146, 1145, 1144, 1143, 1136, 1329, 1329,
 /*   360 */  1329, 1329, 1329, 1329, 1329, 1280, 1274, 1276, 1272, 1329,
 /*   370 */  1329, 1329, 1001, 1133, 1105, 1000, 1061, 1073, 1072, 1071,
 /*   380 */  1070, 1069, 1068, 1067, 1053, 1085, 1089, 1084, 1088, 1018,
 /*   390 */  1174, 1170, 1049, 1046, 1045, 1044, 1043, 1042, 1041, 1040,
 /*   400 */  1039, 1038, 1037, 1036, 1035, 1034, 1329, 1329, 1175, 1171,
 /*   410 */  1076,  911,  912, 1033, 1032, 1031, 1030, 1029, 1028, 1027,
 /*   420 */   908,  907, 1166, 1135, 1134, 1122, 1121, 1106, 1107, 1006,
 /*   430 */  1007, 1329, 1329, 1329,  995,  996, 1063, 1062,  965,  964,
 /*   440 */  1020, 1019,  940,  939,  945,  944,  981,  982,  950,  949,
 /*   450 */   924,  925, 1329, 1329, 1002, 1329, 1329, 1239, 1243, 1242,
 /*   460 */  1240, 1329, 1329, 1329, 1329, 1329, 1329,  986, 1329, 1329,
 /*   470 */  1329, 1329, 1329, 1187, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   480 */   891, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   490 */  1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329, 1329,
 /*   500 */  1329, 1329, 1329, 1329, 1115, 1132, 1131, 1329, 1329, 1329,
 /*   510 */  1241, 1329, 1235, 1228, 1205, 1207, 1329, 1220, 1186, 1185,
 /*   520 */  1203, 1329, 1329, 1202, 1201, 1329, 1329, 1329, 1329, 1329,
 /*   530 */  1329, 1329, 1329, 1329, 1329, 1329, 1060, 1059, 1051, 1047,
 /*   540 */   895, 1017, 1016, 1329, 1329, 1329, 1329, 1074,  910,  906,
 /*   550 */   903,  899,  894, 1329, 1328, 1327, 1326, 1325, 1324, 1323,
 /*   560 */  1322, 1321, 1319, 1318, 1317, 1316, 1315, 1314, 1167, 1313,
 /*   570 */  1312, 1320, 1311, 1310, 1305, 1303, 1302, 1300, 1299, 1298,
 /*   580 */  1297, 1296, 1295, 1294, 1293, 1292, 1291, 1290, 1289, 1288,
 /*   590 */  1287, 1286, 1285, 1284, 1283, 1282, 1256, 1255, 1264, 1263,
 /*   600 */  1271, 1270, 1267, 1266, 1261, 1269, 1127, 1129, 1150, 1142,
 /*   610 */  1141, 1140, 1139, 1138, 1137, 1130, 1128, 1126, 1120, 1119,
 /*   620 */  1117, 1116, 1115, 1125, 1124, 1123, 1110, 1109, 1108, 1104,
 /*   630 */  1103, 1102, 1101, 1100, 1099, 1098, 1097, 1096, 1095, 1094,
 /*   640 */  1093, 1114, 1113, 1112, 1111, 1260, 1259, 1258, 1257, 1010,
 /*   650 */  1009, 1008, 1005, 1004, 1003, 1002, 1250, 1249, 1251, 1248,
 /*   660 */  1253, 1254, 1252, 1247, 1245, 1244, 1246, 1238, 1233, 1236,
 /*   670 */  1237, 1234, 1232, 1223, 1226, 1231, 1229, 1227, 1225, 1224,
 /*   680 */  1222, 1198, 1206, 1208, 1221, 1219, 1218, 1217, 1216, 1215,
 /*   690 */  1214, 1213, 1212, 1211, 1210, 1209, 1204, 1200, 1196, 1195,
 /*   700 */  1194, 1193, 1192, 1191, 1190,  899, 1189, 1188,  999,  998,
 /*   710 */   997,  994,  993,  992,  991,  990,  989,  988,  987,  986,
 /*   720 */  1199, 1197, 1177, 1180, 1181, 1184, 1183, 1182, 1179, 1178,
 /*   730 */  1176, 1309, 1308, 1307, 1306, 1304, 1301, 1055, 1057, 1066,
 /*   740 */  1065, 1064, 1058, 1056, 1054,  963,  962,  961,  960,  959,
 /*   750 */   958,  968,  967,  966,  957,  956,  955,  954, 1278, 1050,
 /*   760 */  1052,  896, 1012, 1014, 1078, 1081, 1080, 1079, 1077, 1026,
 /*   770 */  1025, 1024, 1023, 1022, 1021, 1015, 1013, 1011,  936, 1160,
 /*   780 */  1166, 1165, 1164, 1163, 1162, 1161, 1159, 1048,  943,  942,
 /*   790 */   941,  948,  947,  946,  938,  937,  936,  935,  934,  933,
 /*   800 */  1091, 1092, 1090, 1075,  983,  985,  984,  980,  979,  978,
 /*   810 */   977,  976,  975,  974,  973,  972,  971,  970,  969,  909,
 /*   820 */   953,  952,  951,  932,  931,  930,  929,  926,  928,  927,
 /*   830 */   923,  922,  921,  920,  919,  918,  917,  916,  915,  914,
 /*   840 */   913,  905,  904,  902,  901,  900,  898,  897,  893,  889,
 /*   850 */   888,  892,  891,  890,  887,  886,  885,  884,  883,  882,
 /*   860 */   881,  880,  879,  878,  877,  876,  875,  874,  873,  872,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
    0,  /*          $ => nothing */
    0,  /*    INTEGER => nothing */
    0,  /* IDENTIFIER => nothing */
    0,  /* POUND_IDENTIFIER => nothing */
    0,  /* POUND_INTEGER => nothing */
    0,  /* AT_IDENTIFIER => nothing */
    0,  /* STRING_LITERAL => nothing */
    0,  /*     ASP_GR => nothing */
    0,  /*     ASP_CP => nothing */
    0,  /*     LUA_GR => nothing */
    0,  /*     LUA_CP => nothing */
    0,  /*  PYTHON_GR => nothing */
    0,  /*  PYTHON_CP => nothing */
    0,  /*    F2LP_GR => nothing */
    0,  /*    F2LP_CP => nothing */
    0,  /*    COMMENT => nothing */
    0,  /*  CONSTANTS => nothing */
    0,  /*    INCLUDE => nothing */
    0,  /*     MACROS => nothing */
    0,  /*    OBJECTS => nothing */
    0,  /*      QUERY => nothing */
    0,  /*       SHOW => nothing */
    0,  /*      SORTS => nothing */
    0,  /*  VARIABLES => nothing */
    0,  /*   ABACTION => nothing */
    0,  /*     ACTION => nothing */
    0,  /* ADDITIVEACTION => nothing */
    0,  /* ADDITIVEFLUENT => nothing */
    0,  /*      AFTER => nothing */
    0,  /*        ALL => nothing */
    0,  /*     ALWAYS => nothing */
    0,  /*   ASSUMING => nothing */
    0,  /*  ATTRIBUTE => nothing */
    0,  /*         BY => nothing */
    0,  /*     CAUSED => nothing */
    0,  /*     CAUSES => nothing */
    0,  /* IMPOSSIBLE => nothing */
    0,  /* CONSTRAINT => nothing */
    0,  /* DECREMENTS => nothing */
    0,  /*    DEFAULT => nothing */
    0,  /* EXTERNALACTION => nothing */
    0,  /*  EXOGENOUS => nothing */
    0,  /* EXOGENOUSACTION => nothing */
    0,  /*         IF => nothing */
    0,  /*     IFCONS => nothing */
    0,  /* INCREMENTS => nothing */
    0,  /*   INERTIAL => nothing */
    0,  /* INERTIALFLUENT => nothing */
    0,  /*      LABEL => nothing */
    0,  /*  MAY_CAUSE => nothing */
    0,  /* MAXADDITIVE => nothing */
    0,  /* MAXAFVALUE => nothing */
    0,  /*    MAXSTEP => nothing */
    0,  /*      NEVER => nothing */
    0,  /* NOCONCURRENCY => nothing */
    0,  /* STRONG_NOCONCURRENCY => nothing */
    0,  /* NONEXECUTABLE => nothing */
    0,  /*         OF => nothing */
    0,  /* POSSIBLY_CAUSED => nothing */
    0,  /*      RIGID => nothing */
    0,  /*   SDFLUENT => nothing */
    0,  /* SIMPLEFLUENT => nothing */
    0,  /* EXTERNALFLUENT => nothing */
    0,  /*     UNLESS => nothing */
    0,  /*      WHERE => nothing */
    0,  /*      FALSE => nothing */
    0,  /*       TRUE => nothing */
    0,  /*         AT => nothing */
    0,  /*  BRACKET_L => nothing */
    0,  /*  BRACKET_R => nothing */
    0,  /* COLON_DASH => nothing */
    0,  /* CBRACKET_L => nothing */
    0,  /* CBRACKET_R => nothing */
    0,  /*    PAREN_L => nothing */
    0,  /*    PAREN_R => nothing */
    0,  /*     PERIOD => nothing */
    0,  /* MACRO_STRING => nothing */
    0,  /*      TILDE => nothing */
    0,  /*  DBL_COLON => nothing */
    0,  /*  ARROW_LEQ => nothing */
    0,  /*  ARROW_REQ => nothing */
    0,  /* ARROW_LDASH => nothing */
    0,  /*      COLON => nothing */
    0,  /*         EQ => nothing */
    0,  /*     DBL_EQ => nothing */
    0,  /*        NEQ => nothing */
    0,  /*     NOT_EQ => nothing */
    0,  /*      LTHAN => nothing */
    0,  /*      GTHAN => nothing */
    0,  /*   LTHAN_EQ => nothing */
    0,  /*   GTHAN_EQ => nothing */
    0,  /* DBL_PERIOD => nothing */
    0,  /*   BIG_CONJ => nothing */
    0,  /*   BIG_DISJ => nothing */
    0,  /*      POUND => nothing */
    0,  /*  SEMICOLON => nothing */
    0,  /*      EQUIV => nothing */
    0,  /*       IMPL => nothing */
    0,  /* ARROW_RDASH => nothing */
    0,  /*   DBL_PLUS => nothing */
    0,  /*       PIPE => nothing */
    0,  /*  DBL_GTHAN => nothing */
    0,  /*  DBL_LTHAN => nothing */
    0,  /*        AMP => nothing */
    0,  /*      COMMA => nothing */
    0,  /*    DBL_AMP => nothing */
    0,  /*        NOT => nothing */
    0,  /*       DASH => nothing */
    0,  /*       PLUS => nothing */
    0,  /*       STAR => nothing */
    0,  /*    INT_DIV => nothing */
    0,  /*        MOD => nothing */
    0,  /*        ABS => nothing */
    0,  /*     CARROT => nothing */
    0,  /*     UMINUS => nothing */
    0,  /*     PREC_4 => nothing */
    0,  /*     PREC_3 => nothing */
    0,  /*     PREC_2 => nothing */
    0,  /*     PREC_1 => nothing */
    0,  /*     PREC_0 => nothing */
    0,  /*        EOF => nothing */
    0,  /*     ERR_IO => nothing */
    0,  /* ERR_UNKNOWN_SYMBOL => nothing */
    0,  /* ERR_UNTERMINATED_STRING => nothing */
    0,  /* ERR_UNTERMINATED_ASP => nothing */
    0,  /* ERR_UNTERMINATED_LUA => nothing */
    0,  /* ERR_UNTERMINATED_PYTHON => nothing */
    0,  /* ERR_UNTERMINATED_F2LP => nothing */
    0,  /* ERR_UNTERMINATED_BLK_COMMENT => nothing */
    0,  /* ERR_SYNTAX => nothing */
    0,  /* ERR_PAREN_MISMATCH => nothing */
    0,  /*        ARG => nothing */
    0,  /*       NOOP => nothing */
    2,  /* CONSTANT_ID => IDENTIFIER */
    2,  /* VARIABLE_ID => IDENTIFIER */
    2,  /*  OBJECT_ID => IDENTIFIER */
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  lemon_parserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif

  int yylookmajor;				/* major token type for the lookahead token */
  YYMINORTYPE yylookminor;		/* minor token type for the lookahead token */
  int yysyntaxerr;				/* a flag used to trigger a syntax error */

};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char const*yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void lemon_parserTrace(FILE *TraceFILE, char const*zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

//#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "INTEGER",       "IDENTIFIER",    "POUND_IDENTIFIER",
  "POUND_INTEGER",  "AT_IDENTIFIER",  "STRING_LITERAL",  "ASP_GR",      
  "ASP_CP",        "LUA_GR",        "LUA_CP",        "PYTHON_GR",   
  "PYTHON_CP",     "F2LP_GR",       "F2LP_CP",       "COMMENT",     
  "CONSTANTS",     "INCLUDE",       "MACROS",        "OBJECTS",     
  "QUERY",         "SHOW",          "SORTS",         "VARIABLES",   
  "ABACTION",      "ACTION",        "ADDITIVEACTION",  "ADDITIVEFLUENT",
  "AFTER",         "ALL",           "ALWAYS",        "ASSUMING",    
  "ATTRIBUTE",     "BY",            "CAUSED",        "CAUSES",      
  "IMPOSSIBLE",    "CONSTRAINT",    "DECREMENTS",    "DEFAULT",     
  "EXTERNALACTION",  "EXOGENOUS",     "EXOGENOUSACTION",  "IF",          
  "IFCONS",        "INCREMENTS",    "INERTIAL",      "INERTIALFLUENT",
  "LABEL",         "MAY_CAUSE",     "MAXADDITIVE",   "MAXAFVALUE",  
  "MAXSTEP",       "NEVER",         "NOCONCURRENCY",  "STRONG_NOCONCURRENCY",
  "NONEXECUTABLE",  "OF",            "POSSIBLY_CAUSED",  "RIGID",       
  "SDFLUENT",      "SIMPLEFLUENT",  "EXTERNALFLUENT",  "UNLESS",      
  "WHERE",         "FALSE",         "TRUE",          "AT",          
  "BRACKET_L",     "BRACKET_R",     "COLON_DASH",    "CBRACKET_L",  
  "CBRACKET_R",    "PAREN_L",       "PAREN_R",       "PERIOD",      
  "MACRO_STRING",  "TILDE",         "DBL_COLON",     "ARROW_LEQ",   
  "ARROW_REQ",     "ARROW_LDASH",   "COLON",         "EQ",          
  "DBL_EQ",        "NEQ",           "NOT_EQ",        "LTHAN",       
  "GTHAN",         "LTHAN_EQ",      "GTHAN_EQ",      "DBL_PERIOD",  
  "BIG_CONJ",      "BIG_DISJ",      "POUND",         "SEMICOLON",   
  "EQUIV",         "IMPL",          "ARROW_RDASH",   "DBL_PLUS",    
  "PIPE",          "DBL_GTHAN",     "DBL_LTHAN",     "AMP",         
  "COMMA",         "DBL_AMP",       "NOT",           "DASH",        
  "PLUS",          "STAR",          "INT_DIV",       "MOD",         
  "ABS",           "CARROT",        "UMINUS",        "PREC_4",      
  "PREC_3",        "PREC_2",        "PREC_1",        "PREC_0",      
  "EOF",           "ERR_IO",        "ERR_UNKNOWN_SYMBOL",  "ERR_UNTERMINATED_STRING",
  "ERR_UNTERMINATED_ASP",  "ERR_UNTERMINATED_LUA",  "ERR_UNTERMINATED_PYTHON",  "ERR_UNTERMINATED_F2LP",
  "ERR_UNTERMINATED_BLK_COMMENT",  "ERR_SYNTAX",    "ERR_PAREN_MISMATCH",  "ARG",         
  "NOOP",          "CONSTANT_ID",   "VARIABLE_ID",   "OBJECT_ID",   
  "HIDE",          "OBSERVED",      "error",         "start",       
  "statement_lst",  "statement",     "stmt_macro_def",  "stmt_constant_def",
  "stmt_object_def",  "stmt_variable_def",  "stmt_sort_def",  "stmt_code_blk",
  "stmt_law",      "stmt_show",     "stmt_hide",     "stmt_noconcurrency",
  "stmt_strong_noconcurrency",  "stmt_maxafvalue",  "stmt_maxadditive",  "stmt_query",  
  "base_elem",     "base_elem_no_const",  "constant",      "object",      
  "object_nullary",  "variable",      "lua",           "undeclared",  
  "term_lst",      "term",          "constant_one_const",  "term_no_const_lst",
  "term_no_const",  "const_anon",    "term_strong",   "term_strong_candidate",
  "term_no_const_strong",  "num_range",     "num_range_eval",  "term_integral",
  "term_int_eval",  "formula",       "formula_base",  "comparison",  
  "atomic_formula",  "formula_quant",  "formula_card",  "atomic_formula_anon",
  "formula_no_const",  "formula_no_const_base",  "comparison_no_const",  "atomic_formula_one_const",
  "quant_lst",     "quant_op",      "card_var_lst",  "card_var_lst_inner",
  "term_temporal",  "term_temporal_strong",  "term_temporal_strong_candidate",  "constant_temporal",
  "formula_temporal",  "formula_temporal_base",  "comparison_temporal",  "formula_temporal_quant",
  "formula_temporal_card",  "head_formula",  "atomic_head_formula",  "formula_smpl_card",
  "macro_def_lst",  "macro_bnd",     "macro_args",    "macro_arg",   
  "sort_lst",      "sort",          "sort_id_nr",    "sort_nr",     
  "sort_id",       "constant_bnd_lst",  "constant_bnd",  "constant_dcl_lst",
  "constant_dcl_type",  "attrib_spec",   "object_bnd_lst",  "object_bnd",  
  "object_lst",    "object_spec",   "variable_bnd_lst",  "variable_bnd",
  "variable_lst",  "sort_bnd_lst",  "sort_bnd",      "sort_dcl_lst",
  "show_lst",      "show_elem",     "query_lst",     "query_maxstep_decl",
  "query_label_decl",  "query_label_Decl",  "clause_if",     "clause_after",
  "clause_ifcons",  "clause_unless",  "clause_where",  "law_basic",   
  "law_caused",    "law_pcaused",   "law_impl",      "law_causes",  
  "law_increments",  "law_decrements",  "law_mcause",    "law_always",  
  "law_constraint",  "law_impossible",  "law_never",     "law_default", 
  "law_exogenous",  "law_inertial",  "law_nonexecutable",  "law_rigid",   
  "law_observed",  "law_temporal_constraint",
};
//#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "start ::= statement_lst EOF",
 /*   1 */ "statement_lst ::=",
 /*   2 */ "statement_lst ::= statement_lst error",
 /*   3 */ "statement_lst ::= statement_lst statement",
 /*   4 */ "statement ::= stmt_macro_def",
 /*   5 */ "statement ::= stmt_constant_def",
 /*   6 */ "statement ::= stmt_object_def",
 /*   7 */ "statement ::= stmt_variable_def",
 /*   8 */ "statement ::= stmt_sort_def",
 /*   9 */ "statement ::= stmt_code_blk",
 /*  10 */ "statement ::= stmt_law",
 /*  11 */ "statement ::= stmt_show",
 /*  12 */ "statement ::= stmt_hide",
 /*  13 */ "statement ::= stmt_noconcurrency",
 /*  14 */ "statement ::= stmt_strong_noconcurrency",
 /*  15 */ "statement ::= stmt_maxafvalue",
 /*  16 */ "statement ::= stmt_maxadditive",
 /*  17 */ "statement ::= stmt_query",
 /*  18 */ "base_elem ::= constant",
 /*  19 */ "base_elem ::= base_elem_no_const",
 /*  20 */ "base_elem_no_const ::= object",
 /*  21 */ "base_elem_no_const ::= variable",
 /*  22 */ "base_elem_no_const ::= lua",
 /*  23 */ "constant ::= CONSTANT_ID PAREN_L term_lst PAREN_R",
 /*  24 */ "constant ::= CONSTANT_ID",
 /*  25 */ "const_anon ::= IDENTIFIER",
 /*  26 */ "const_anon ::= IDENTIFIER PAREN_L term_lst PAREN_R",
 /*  27 */ "object ::= OBJECT_ID PAREN_L term_lst PAREN_R",
 /*  28 */ "object ::= object_nullary",
 /*  29 */ "object_nullary ::= OBJECT_ID",
 /*  30 */ "object ::= undeclared",
 /*  31 */ "variable ::= VARIABLE_ID",
 /*  32 */ "lua ::= AT_IDENTIFIER PAREN_L term_lst PAREN_R",
 /*  33 */ "lua ::= AT_IDENTIFIER",
 /*  34 */ "lua ::= AT_IDENTIFIER PAREN_L PAREN_R",
 /*  35 */ "undeclared ::= IDENTIFIER PAREN_L term_lst PAREN_R",
 /*  36 */ "undeclared ::= IDENTIFIER",
 /*  37 */ "term_lst ::= term",
 /*  38 */ "term_lst ::= term_lst COMMA term",
 /*  39 */ "constant_one_const ::= CONSTANT_ID PAREN_L term_no_const_lst PAREN_R",
 /*  40 */ "constant_one_const ::= CONSTANT_ID",
 /*  41 */ "term_no_const_lst ::= term_no_const",
 /*  42 */ "term_no_const_lst ::= term_no_const_lst COMMA term_no_const",
 /*  43 */ "term ::= base_elem",
 /*  44 */ "term ::= INTEGER",
 /*  45 */ "term ::= STRING_LITERAL",
 /*  46 */ "term ::= PAREN_L term PAREN_R",
 /*  47 */ "term ::= TRUE",
 /*  48 */ "term ::= FALSE",
 /*  49 */ "term ::= MAXSTEP",
 /*  50 */ "term ::= MAXADDITIVE",
 /*  51 */ "term ::= MAXAFVALUE",
 /*  52 */ "term ::= DASH term",
 /*  53 */ "term ::= ABS term",
 /*  54 */ "term ::= term DASH term",
 /*  55 */ "term ::= term PLUS term",
 /*  56 */ "term ::= term STAR term",
 /*  57 */ "term ::= term INT_DIV term",
 /*  58 */ "term ::= term MOD term",
 /*  59 */ "term_strong ::= base_elem_no_const",
 /*  60 */ "term_strong ::= INTEGER",
 /*  61 */ "term_strong ::= STRING_LITERAL",
 /*  62 */ "term_strong ::= PAREN_L term_strong PAREN_R",
 /*  63 */ "term_strong ::= MAXSTEP",
 /*  64 */ "term_strong ::= MAXADDITIVE",
 /*  65 */ "term_strong ::= MAXAFVALUE",
 /*  66 */ "term_strong ::= DASH term_strong",
 /*  67 */ "term_strong ::= ABS term",
 /*  68 */ "term_strong_candidate ::= DASH constant",
 /*  69 */ "term_strong ::= term_strong_candidate DASH term",
 /*  70 */ "term_strong ::= term_strong_candidate PLUS term",
 /*  71 */ "term_strong ::= term_strong_candidate STAR term",
 /*  72 */ "term_strong ::= term_strong_candidate INT_DIV term",
 /*  73 */ "term_strong ::= term_strong_candidate MOD term",
 /*  74 */ "term_strong ::= constant DASH term",
 /*  75 */ "term_strong ::= constant PLUS term",
 /*  76 */ "term_strong ::= constant STAR term",
 /*  77 */ "term_strong ::= constant INT_DIV term",
 /*  78 */ "term_strong ::= constant MOD term",
 /*  79 */ "term_strong ::= term_strong DASH term",
 /*  80 */ "term_strong ::= term_strong PLUS term",
 /*  81 */ "term_strong ::= term_strong STAR term",
 /*  82 */ "term_strong ::= term_strong INT_DIV term",
 /*  83 */ "term_strong ::= term_strong MOD term",
 /*  84 */ "term_no_const_strong ::= base_elem_no_const",
 /*  85 */ "term_no_const_strong ::= INTEGER",
 /*  86 */ "term_no_const_strong ::= STRING_LITERAL",
 /*  87 */ "term_no_const_strong ::= PAREN_L term_no_const_strong PAREN_R",
 /*  88 */ "term_no_const_strong ::= MAXSTEP",
 /*  89 */ "term_no_const_strong ::= MAXADDITIVE",
 /*  90 */ "term_no_const_strong ::= MAXAFVALUE",
 /*  91 */ "term_no_const_strong ::= constant",
 /*  92 */ "term_no_const_strong ::= DASH term_no_const_strong",
 /*  93 */ "term_no_const_strong ::= ABS term_no_const",
 /*  94 */ "term_no_const_strong ::= term_no_const_strong DASH term_no_const",
 /*  95 */ "term_no_const_strong ::= term_no_const_strong PLUS term_no_const",
 /*  96 */ "term_no_const_strong ::= term_no_const_strong STAR term_no_const",
 /*  97 */ "term_no_const_strong ::= term_no_const_strong INT_DIV term_no_const",
 /*  98 */ "term_no_const_strong ::= term_no_const_strong MOD term_no_const",
 /*  99 */ "term_no_const ::= base_elem_no_const",
 /* 100 */ "term_no_const ::= INTEGER",
 /* 101 */ "term_no_const ::= STRING_LITERAL",
 /* 102 */ "term_no_const ::= PAREN_L term_no_const PAREN_R",
 /* 103 */ "term_no_const ::= TRUE",
 /* 104 */ "term_no_const ::= FALSE",
 /* 105 */ "term_no_const ::= MAXSTEP",
 /* 106 */ "term_no_const ::= MAXADDITIVE",
 /* 107 */ "term_no_const ::= MAXAFVALUE",
 /* 108 */ "term_no_const ::= constant",
 /* 109 */ "term_no_const ::= DASH term_no_const",
 /* 110 */ "term_no_const ::= ABS term_no_const",
 /* 111 */ "term_no_const ::= term_no_const DASH term_no_const",
 /* 112 */ "term_no_const ::= term_no_const PLUS term_no_const",
 /* 113 */ "term_no_const ::= term_no_const STAR term_no_const",
 /* 114 */ "term_no_const ::= term_no_const INT_DIV term_no_const",
 /* 115 */ "term_no_const ::= term_no_const MOD term_no_const",
 /* 116 */ "term_integral ::= INTEGER",
 /* 117 */ "term_integral ::= PAREN_L term_integral PAREN_R",
 /* 118 */ "term_integral ::= TRUE",
 /* 119 */ "term_integral ::= FALSE",
 /* 120 */ "term_integral ::= MAXSTEP",
 /* 121 */ "term_integral ::= MAXADDITIVE",
 /* 122 */ "term_integral ::= MAXAFVALUE",
 /* 123 */ "term_integral ::= DASH term_integral",
 /* 124 */ "term_integral ::= ABS term_integral",
 /* 125 */ "term_integral ::= term_integral DASH term_integral",
 /* 126 */ "term_integral ::= term_integral PLUS term_integral",
 /* 127 */ "term_integral ::= term_integral STAR term_integral",
 /* 128 */ "term_integral ::= term_integral INT_DIV term_integral",
 /* 129 */ "term_integral ::= term_integral MOD term_integral",
 /* 130 */ "num_range ::= term_integral DBL_PERIOD term_integral",
 /* 131 */ "num_range_eval ::= term_int_eval DBL_PERIOD term_int_eval",
 /* 132 */ "term_int_eval ::= INTEGER",
 /* 133 */ "term_int_eval ::= PAREN_L term_int_eval PAREN_R",
 /* 134 */ "term_int_eval ::= DASH term_int_eval",
 /* 135 */ "term_int_eval ::= ABS term_int_eval",
 /* 136 */ "term_int_eval ::= term_int_eval DASH term_int_eval",
 /* 137 */ "term_int_eval ::= term_int_eval PLUS term_int_eval",
 /* 138 */ "term_int_eval ::= term_int_eval STAR term_int_eval",
 /* 139 */ "term_int_eval ::= term_int_eval INT_DIV term_int_eval",
 /* 140 */ "term_int_eval ::= term_int_eval MOD term_int_eval",
 /* 141 */ "formula ::= formula_base",
 /* 142 */ "formula ::= PAREN_L formula PAREN_R",
 /* 143 */ "formula ::= NOT formula",
 /* 144 */ "formula ::= DASH formula",
 /* 145 */ "formula ::= formula AMP formula",
 /* 146 */ "formula ::= formula DBL_PLUS formula",
 /* 147 */ "formula ::= formula PIPE formula",
 /* 148 */ "formula ::= formula EQUIV formula",
 /* 149 */ "formula ::= formula IMPL formula",
 /* 150 */ "formula ::= formula ARROW_RDASH formula",
 /* 151 */ "formula_base ::= comparison",
 /* 152 */ "formula_base ::= atomic_formula",
 /* 153 */ "formula_base ::= formula_quant",
 /* 154 */ "formula_base ::= formula_card",
 /* 155 */ "formula_base ::= TRUE",
 /* 156 */ "formula_base ::= FALSE",
 /* 157 */ "comparison ::= term_strong EQ term",
 /* 158 */ "comparison ::= term_strong DBL_EQ term",
 /* 159 */ "comparison ::= term_strong NEQ term",
 /* 160 */ "comparison ::= term_strong LTHAN term",
 /* 161 */ "comparison ::= term_strong GTHAN term",
 /* 162 */ "comparison ::= term_strong LTHAN_EQ term",
 /* 163 */ "comparison ::= term_strong GTHAN_EQ term",
 /* 164 */ "comparison ::= term_strong_candidate EQ term",
 /* 165 */ "comparison ::= term_strong_candidate DBL_EQ term",
 /* 166 */ "comparison ::= term_strong_candidate NEQ term",
 /* 167 */ "comparison ::= term_strong_candidate LTHAN term",
 /* 168 */ "comparison ::= term_strong_candidate GTHAN term",
 /* 169 */ "comparison ::= term_strong_candidate LTHAN_EQ term",
 /* 170 */ "comparison ::= term_strong_candidate GTHAN_EQ term",
 /* 171 */ "comparison ::= constant DBL_EQ term",
 /* 172 */ "comparison ::= constant NEQ term",
 /* 173 */ "comparison ::= constant LTHAN term",
 /* 174 */ "comparison ::= constant GTHAN term",
 /* 175 */ "comparison ::= constant LTHAN_EQ term",
 /* 176 */ "comparison ::= constant GTHAN_EQ term",
 /* 177 */ "atomic_formula ::= constant",
 /* 178 */ "atomic_formula ::= TILDE constant",
 /* 179 */ "atomic_formula ::= constant EQ term",
 /* 180 */ "atomic_formula_anon ::= atomic_formula",
 /* 181 */ "atomic_formula_anon ::= const_anon",
 /* 182 */ "atomic_formula_anon ::= TILDE const_anon",
 /* 183 */ "atomic_formula_anon ::= const_anon EQ term",
 /* 184 */ "formula_no_const ::= formula_no_const_base",
 /* 185 */ "formula_no_const ::= PAREN_L formula_no_const PAREN_R",
 /* 186 */ "formula_no_const ::= NOT formula_no_const",
 /* 187 */ "formula_no_const ::= DASH formula_no_const",
 /* 188 */ "formula_no_const ::= formula_no_const AMP formula_no_const",
 /* 189 */ "formula_no_const ::= formula_no_const DBL_PLUS formula_no_const",
 /* 190 */ "formula_no_const ::= formula_no_const PIPE formula_no_const",
 /* 191 */ "formula_no_const ::= formula_no_const EQUIV formula_no_const",
 /* 192 */ "formula_no_const ::= formula_no_const IMPL formula_no_const",
 /* 193 */ "formula_no_const ::= formula_no_const ARROW_RDASH formula_no_const",
 /* 194 */ "formula_no_const_base ::= comparison_no_const",
 /* 195 */ "formula_no_const_base ::= TRUE",
 /* 196 */ "formula_no_const_base ::= FALSE",
 /* 197 */ "comparison_no_const ::= term_no_const_strong EQ term_no_const",
 /* 198 */ "comparison_no_const ::= term_no_const_strong DBL_EQ term_no_const",
 /* 199 */ "comparison_no_const ::= term_no_const_strong NEQ term_no_const",
 /* 200 */ "comparison_no_const ::= term_no_const_strong LTHAN term_no_const",
 /* 201 */ "comparison_no_const ::= term_no_const_strong GTHAN term_no_const",
 /* 202 */ "comparison_no_const ::= term_no_const_strong LTHAN_EQ term_no_const",
 /* 203 */ "comparison_no_const ::= term_no_const_strong GTHAN_EQ term_no_const",
 /* 204 */ "atomic_formula_one_const ::= constant_one_const",
 /* 205 */ "atomic_formula_one_const ::= TILDE constant_one_const",
 /* 206 */ "atomic_formula_one_const ::= constant_one_const EQ term_no_const",
 /* 207 */ "formula_quant ::= BRACKET_L quant_lst PIPE formula BRACKET_R",
 /* 208 */ "quant_lst ::= quant_op variable",
 /* 209 */ "quant_lst ::= quant_lst quant_op variable",
 /* 210 */ "quant_op ::= BIG_CONJ",
 /* 211 */ "quant_op ::= BIG_DISJ",
 /* 212 */ "formula_card ::= CBRACKET_L card_var_lst formula CBRACKET_R",
 /* 213 */ "formula_card ::= term_strong CBRACKET_L card_var_lst formula CBRACKET_R",
 /* 214 */ "formula_card ::= CBRACKET_L card_var_lst formula CBRACKET_R term",
 /* 215 */ "formula_card ::= term_strong CBRACKET_L card_var_lst formula CBRACKET_R term",
 /* 216 */ "formula_card ::= CBRACKET_L formula CBRACKET_R",
 /* 217 */ "formula_card ::= term_strong CBRACKET_L formula CBRACKET_R",
 /* 218 */ "formula_card ::= CBRACKET_L formula CBRACKET_R term",
 /* 219 */ "formula_card ::= term_strong CBRACKET_L formula CBRACKET_R term",
 /* 220 */ "card_var_lst ::= card_var_lst_inner PIPE",
 /* 221 */ "card_var_lst_inner ::= variable",
 /* 222 */ "card_var_lst_inner ::= card_var_lst_inner COMMA variable",
 /* 223 */ "term_temporal ::= base_elem_no_const",
 /* 224 */ "term_temporal ::= INTEGER",
 /* 225 */ "term_temporal ::= STRING_LITERAL",
 /* 226 */ "term_temporal ::= PAREN_L term_temporal PAREN_R",
 /* 227 */ "term_temporal ::= TRUE",
 /* 228 */ "term_temporal ::= FALSE",
 /* 229 */ "term_temporal ::= MAXSTEP",
 /* 230 */ "term_temporal ::= MAXADDITIVE",
 /* 231 */ "term_temporal ::= MAXAFVALUE",
 /* 232 */ "term_temporal ::= constant",
 /* 233 */ "term_temporal ::= DASH term_temporal",
 /* 234 */ "term_temporal ::= ABS term_temporal",
 /* 235 */ "term_temporal ::= term_temporal COLON term",
 /* 236 */ "term_temporal ::= term_temporal DASH term_temporal",
 /* 237 */ "term_temporal ::= term_temporal PLUS term_temporal",
 /* 238 */ "term_temporal ::= term_temporal STAR term_temporal",
 /* 239 */ "term_temporal ::= term_temporal INT_DIV term_temporal",
 /* 240 */ "term_temporal ::= term_temporal MOD term_temporal",
 /* 241 */ "term_temporal_strong ::= base_elem_no_const",
 /* 242 */ "term_temporal_strong ::= INTEGER",
 /* 243 */ "term_temporal_strong ::= STRING_LITERAL",
 /* 244 */ "term_temporal_strong ::= PAREN_L term_temporal_strong PAREN_R",
 /* 245 */ "term_temporal_strong ::= MAXSTEP",
 /* 246 */ "term_temporal_strong ::= MAXADDITIVE",
 /* 247 */ "term_temporal_strong ::= MAXAFVALUE",
 /* 248 */ "term_temporal_strong ::= term_temporal_strong COLON term_strong",
 /* 249 */ "term_temporal_strong ::= DASH term_temporal_strong",
 /* 250 */ "term_temporal_strong ::= ABS term_temporal",
 /* 251 */ "term_temporal_strong ::= term_temporal_strong DASH term_temporal",
 /* 252 */ "term_temporal_strong ::= term_temporal_strong PLUS term_temporal",
 /* 253 */ "term_temporal_strong ::= term_temporal_strong STAR term_temporal",
 /* 254 */ "term_temporal_strong ::= term_temporal_strong INT_DIV term_temporal",
 /* 255 */ "term_temporal_strong ::= term_temporal_strong MOD term_temporal",
 /* 256 */ "formula_temporal ::= formula_temporal_base",
 /* 257 */ "formula_temporal ::= PAREN_L formula_temporal PAREN_R",
 /* 258 */ "formula_temporal ::= NOT formula_temporal",
 /* 259 */ "formula_temporal ::= DASH formula_temporal",
 /* 260 */ "formula_temporal ::= formula_temporal AMP formula_temporal",
 /* 261 */ "formula_temporal ::= formula_temporal DBL_PLUS formula_temporal",
 /* 262 */ "formula_temporal ::= formula_temporal PIPE formula_temporal",
 /* 263 */ "formula_temporal ::= formula_temporal EQUIV formula_temporal",
 /* 264 */ "formula_temporal ::= formula_temporal IMPL formula_temporal",
 /* 265 */ "formula_temporal ::= formula_temporal ARROW_RDASH formula_temporal",
 /* 266 */ "formula_temporal ::= term_temporal_strong COLON formula",
 /* 267 */ "formula_temporal_base ::= comparison_temporal",
 /* 268 */ "formula_temporal_base ::= atomic_formula",
 /* 269 */ "formula_temporal_base ::= formula_temporal_quant",
 /* 270 */ "formula_temporal_base ::= formula_temporal_card",
 /* 271 */ "formula_temporal_base ::= TRUE",
 /* 272 */ "formula_temporal_base ::= FALSE",
 /* 273 */ "comparison_temporal ::= term_temporal_strong EQ term_temporal",
 /* 274 */ "comparison_temporal ::= term_temporal_strong DBL_EQ term_temporal",
 /* 275 */ "comparison_temporal ::= term_temporal_strong NEQ term_temporal",
 /* 276 */ "comparison_temporal ::= term_temporal_strong LTHAN term_temporal",
 /* 277 */ "comparison_temporal ::= term_temporal_strong GTHAN term_temporal",
 /* 278 */ "comparison_temporal ::= term_temporal_strong LTHAN_EQ term_temporal",
 /* 279 */ "comparison_temporal ::= term_temporal_strong GTHAN_EQ term_temporal",
 /* 280 */ "formula_temporal_quant ::= BRACKET_L quant_lst PIPE formula_temporal BRACKET_R",
 /* 281 */ "formula_temporal_card ::= CBRACKET_L card_var_lst formula_temporal CBRACKET_R",
 /* 282 */ "formula_temporal_card ::= term_temporal_strong CBRACKET_L card_var_lst formula_temporal CBRACKET_R",
 /* 283 */ "formula_temporal_card ::= CBRACKET_L card_var_lst formula_temporal CBRACKET_R term_temporal",
 /* 284 */ "formula_temporal_card ::= term_temporal_strong CBRACKET_L card_var_lst formula_temporal CBRACKET_R term_temporal",
 /* 285 */ "formula_temporal_card ::= CBRACKET_L formula_temporal CBRACKET_R",
 /* 286 */ "formula_temporal_card ::= term_temporal_strong CBRACKET_L formula_temporal CBRACKET_R",
 /* 287 */ "formula_temporal_card ::= CBRACKET_L formula_temporal CBRACKET_R term_temporal",
 /* 288 */ "formula_temporal_card ::= term_temporal_strong CBRACKET_L formula_temporal CBRACKET_R term_temporal",
 /* 289 */ "head_formula ::= head_formula AMP head_formula",
 /* 290 */ "head_formula ::= PAREN_L head_formula PAREN_R",
 /* 291 */ "head_formula ::= comparison",
 /* 292 */ "head_formula ::= atomic_head_formula",
 /* 293 */ "head_formula ::= formula_smpl_card",
 /* 294 */ "head_formula ::= TRUE",
 /* 295 */ "head_formula ::= FALSE",
 /* 296 */ "atomic_head_formula ::= atomic_formula",
 /* 297 */ "atomic_head_formula ::= DASH constant",
 /* 298 */ "formula_smpl_card ::= CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R",
 /* 299 */ "formula_smpl_card ::= term_strong CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R",
 /* 300 */ "formula_smpl_card ::= CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R term",
 /* 301 */ "formula_smpl_card ::= term_strong CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R term",
 /* 302 */ "formula_smpl_card ::= CBRACKET_L atomic_formula_one_const CBRACKET_R",
 /* 303 */ "formula_smpl_card ::= term_strong CBRACKET_L atomic_formula_one_const CBRACKET_R",
 /* 304 */ "formula_smpl_card ::= CBRACKET_L atomic_formula_one_const CBRACKET_R term",
 /* 305 */ "formula_smpl_card ::= term_strong CBRACKET_L atomic_formula_one_const CBRACKET_R term",
 /* 306 */ "stmt_macro_def ::= COLON_DASH MACROS macro_def_lst PERIOD",
 /* 307 */ "macro_def_lst ::= macro_bnd",
 /* 308 */ "macro_def_lst ::= macro_def_lst SEMICOLON macro_bnd",
 /* 309 */ "macro_bnd ::= IDENTIFIER PAREN_L macro_args PAREN_R ARROW_RDASH MACRO_STRING",
 /* 310 */ "macro_bnd ::= IDENTIFIER ARROW_RDASH MACRO_STRING",
 /* 311 */ "macro_args ::= macro_arg",
 /* 312 */ "macro_args ::= macro_args COMMA macro_arg",
 /* 313 */ "macro_arg ::= POUND_INTEGER",
 /* 314 */ "macro_arg ::= POUND_IDENTIFIER",
 /* 315 */ "sort_lst ::= sort",
 /* 316 */ "sort_lst ::= sort_lst COMMA sort",
 /* 317 */ "sort ::= sort_id_nr",
 /* 318 */ "sort ::= sort_id_nr STAR",
 /* 319 */ "sort ::= sort_id_nr CARROT",
 /* 320 */ "sort ::= sort PLUS object_nullary",
 /* 321 */ "sort ::= sort PLUS IDENTIFIER",
 /* 322 */ "sort ::= sort PLUS INTEGER",
 /* 323 */ "sort_id_nr ::= sort_id",
 /* 324 */ "sort_id_nr ::= sort_nr",
 /* 325 */ "sort_nr ::= num_range",
 /* 326 */ "sort_id ::= IDENTIFIER",
 /* 327 */ "stmt_constant_def ::= COLON_DASH CONSTANTS constant_bnd_lst PERIOD",
 /* 328 */ "constant_bnd_lst ::= constant_bnd",
 /* 329 */ "constant_bnd_lst ::= constant_bnd_lst SEMICOLON constant_bnd",
 /* 330 */ "constant_bnd ::= constant_dcl_lst DBL_COLON constant_dcl_type PAREN_L sort PAREN_R",
 /* 331 */ "constant_bnd ::= constant_dcl_lst DBL_COLON sort",
 /* 332 */ "constant_bnd ::= constant_dcl_lst DBL_COLON constant_dcl_type",
 /* 333 */ "constant_bnd ::= constant_dcl_lst DBL_COLON attrib_spec OF IDENTIFIER",
 /* 334 */ "constant_bnd ::= constant_dcl_lst DBL_COLON attrib_spec OF IDENTIFIER PAREN_L sort_lst PAREN_R",
 /* 335 */ "constant_dcl_lst ::= IDENTIFIER",
 /* 336 */ "constant_dcl_lst ::= IDENTIFIER PAREN_L sort_lst PAREN_R",
 /* 337 */ "constant_dcl_lst ::= constant_dcl_lst COMMA IDENTIFIER",
 /* 338 */ "constant_dcl_lst ::= constant_dcl_lst COMMA IDENTIFIER PAREN_L sort_lst PAREN_R",
 /* 339 */ "constant_dcl_type ::= ABACTION",
 /* 340 */ "constant_dcl_type ::= ACTION",
 /* 341 */ "constant_dcl_type ::= ADDITIVEACTION",
 /* 342 */ "constant_dcl_type ::= ADDITIVEFLUENT",
 /* 343 */ "constant_dcl_type ::= EXTERNALACTION",
 /* 344 */ "constant_dcl_type ::= EXTERNALFLUENT",
 /* 345 */ "constant_dcl_type ::= EXOGENOUSACTION",
 /* 346 */ "constant_dcl_type ::= INERTIALFLUENT",
 /* 347 */ "constant_dcl_type ::= RIGID",
 /* 348 */ "constant_dcl_type ::= SIMPLEFLUENT",
 /* 349 */ "constant_dcl_type ::= SDFLUENT",
 /* 350 */ "attrib_spec ::= ATTRIBUTE",
 /* 351 */ "attrib_spec ::= ATTRIBUTE PAREN_L sort PAREN_R",
 /* 352 */ "stmt_object_def ::= COLON_DASH OBJECTS object_bnd_lst PERIOD",
 /* 353 */ "object_bnd_lst ::= object_bnd",
 /* 354 */ "object_bnd_lst ::= object_bnd_lst SEMICOLON object_bnd",
 /* 355 */ "object_bnd ::= object_lst DBL_COLON sort_id",
 /* 356 */ "object_lst ::= object_spec",
 /* 357 */ "object_lst ::= object_lst COMMA object_spec",
 /* 358 */ "object_spec ::= IDENTIFIER",
 /* 359 */ "object_spec ::= IDENTIFIER PAREN_L sort_lst PAREN_R",
 /* 360 */ "object_spec ::= INTEGER",
 /* 361 */ "object_spec ::= num_range",
 /* 362 */ "stmt_variable_def ::= COLON_DASH VARIABLES variable_bnd_lst PERIOD",
 /* 363 */ "variable_bnd_lst ::= variable_bnd",
 /* 364 */ "variable_bnd_lst ::= variable_bnd_lst SEMICOLON variable_bnd",
 /* 365 */ "variable_bnd ::= variable_lst DBL_COLON sort",
 /* 366 */ "variable_lst ::= IDENTIFIER",
 /* 367 */ "variable_lst ::= variable_lst COMMA IDENTIFIER",
 /* 368 */ "stmt_sort_def ::= COLON_DASH SORTS sort_bnd_lst PERIOD",
 /* 369 */ "sort_bnd_lst ::= sort_bnd",
 /* 370 */ "sort_bnd_lst ::= sort_bnd_lst SEMICOLON sort_bnd",
 /* 371 */ "sort_bnd ::= sort_dcl_lst",
 /* 372 */ "sort_bnd ::= sort_bnd DBL_LTHAN sort_bnd",
 /* 373 */ "sort_bnd ::= sort_bnd DBL_GTHAN sort_bnd",
 /* 374 */ "sort_bnd ::= PAREN_L sort_bnd PAREN_R",
 /* 375 */ "sort_dcl_lst ::= IDENTIFIER",
 /* 376 */ "sort_dcl_lst ::= sort_dcl_lst COMMA IDENTIFIER",
 /* 377 */ "stmt_show ::= COLON_DASH SHOW show_lst PERIOD",
 /* 378 */ "stmt_show ::= COLON_DASH SHOW ALL PERIOD",
 /* 379 */ "stmt_hide ::= COLON_DASH HIDE show_lst PERIOD",
 /* 380 */ "stmt_hide ::= COLON_DASH HIDE ALL PERIOD",
 /* 381 */ "show_lst ::= show_elem",
 /* 382 */ "show_lst ::= show_lst COMMA show_elem",
 /* 383 */ "show_lst ::= show_lst SEMICOLON show_elem",
 /* 384 */ "show_elem ::= atomic_formula_one_const",
 /* 385 */ "stmt_noconcurrency ::= NOCONCURRENCY PERIOD",
 /* 386 */ "stmt_strong_noconcurrency ::= STRONG_NOCONCURRENCY PERIOD",
 /* 387 */ "stmt_maxafvalue ::= COLON_DASH MAXAFVALUE EQ term_int_eval PERIOD",
 /* 388 */ "stmt_maxafvalue ::= COLON_DASH MAXAFVALUE DBL_COLON term_int_eval PERIOD",
 /* 389 */ "stmt_maxadditive ::= COLON_DASH MAXADDITIVE EQ term_int_eval PERIOD",
 /* 390 */ "stmt_maxadditive ::= COLON_DASH MAXADDITIVE DBL_COLON term_int_eval PERIOD",
 /* 391 */ "stmt_query ::= COLON_DASH QUERY query_lst PERIOD",
 /* 392 */ "query_lst ::= formula_temporal",
 /* 393 */ "query_lst ::= query_maxstep_decl",
 /* 394 */ "query_lst ::= query_label_decl",
 /* 395 */ "query_lst ::= query_lst SEMICOLON formula_temporal",
 /* 396 */ "query_lst ::= query_lst SEMICOLON query_maxstep_decl",
 /* 397 */ "query_lst ::= query_lst SEMICOLON query_label_decl",
 /* 398 */ "query_maxstep_decl ::= MAXSTEP DBL_COLON INTEGER",
 /* 399 */ "query_maxstep_decl ::= MAXSTEP DBL_COLON num_range_eval",
 /* 400 */ "query_label_decl ::= LABEL DBL_COLON INTEGER",
 /* 401 */ "query_label_decl ::= LABEL DBL_COLON IDENTIFIER",
 /* 402 */ "clause_if ::= IF formula",
 /* 403 */ "clause_if ::=",
 /* 404 */ "clause_after ::= AFTER formula",
 /* 405 */ "clause_after ::=",
 /* 406 */ "clause_ifcons ::= IFCONS formula",
 /* 407 */ "clause_ifcons ::=",
 /* 408 */ "clause_unless ::= UNLESS atomic_formula_anon",
 /* 409 */ "clause_unless ::=",
 /* 410 */ "clause_where ::= WHERE formula_no_const",
 /* 411 */ "clause_where ::=",
 /* 412 */ "stmt_law ::= law_basic",
 /* 413 */ "stmt_law ::= law_caused",
 /* 414 */ "stmt_law ::= law_pcaused",
 /* 415 */ "stmt_law ::= law_impl",
 /* 416 */ "stmt_law ::= law_causes",
 /* 417 */ "stmt_law ::= law_increments",
 /* 418 */ "stmt_law ::= law_decrements",
 /* 419 */ "stmt_law ::= law_mcause",
 /* 420 */ "stmt_law ::= law_always",
 /* 421 */ "stmt_law ::= law_constraint",
 /* 422 */ "stmt_law ::= law_impossible",
 /* 423 */ "stmt_law ::= law_never",
 /* 424 */ "stmt_law ::= law_default",
 /* 425 */ "stmt_law ::= law_exogenous",
 /* 426 */ "stmt_law ::= law_inertial",
 /* 427 */ "stmt_law ::= law_nonexecutable",
 /* 428 */ "stmt_law ::= law_rigid",
 /* 429 */ "stmt_law ::= law_observed",
 /* 430 */ "stmt_law ::= law_temporal_constraint",
 /* 431 */ "law_basic ::= head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD",
 /* 432 */ "law_caused ::= CAUSED head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD",
 /* 433 */ "law_pcaused ::= POSSIBLY_CAUSED head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD",
 /* 434 */ "law_impl ::= head_formula ARROW_LDASH formula clause_where PERIOD",
 /* 435 */ "law_impl ::= ARROW_LDASH formula clause_where PERIOD",
 /* 436 */ "law_causes ::= atomic_formula CAUSES head_formula clause_if clause_unless clause_where PERIOD",
 /* 437 */ "law_increments ::= atomic_formula INCREMENTS constant BY term clause_if clause_unless clause_where PERIOD",
 /* 438 */ "law_decrements ::= atomic_formula DECREMENTS constant BY term clause_if clause_unless clause_where PERIOD",
 /* 439 */ "law_mcause ::= atomic_formula MAY_CAUSE head_formula clause_if clause_unless clause_where PERIOD",
 /* 440 */ "law_always ::= ALWAYS formula clause_after clause_unless clause_where PERIOD",
 /* 441 */ "law_constraint ::= CONSTRAINT formula clause_after clause_unless clause_where PERIOD",
 /* 442 */ "law_impossible ::= IMPOSSIBLE formula clause_after clause_unless clause_where PERIOD",
 /* 443 */ "law_never ::= NEVER formula clause_after clause_unless clause_where PERIOD",
 /* 444 */ "law_default ::= DEFAULT atomic_head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD",
 /* 445 */ "law_exogenous ::= EXOGENOUS constant clause_if clause_ifcons clause_after clause_unless clause_where PERIOD",
 /* 446 */ "law_inertial ::= INERTIAL constant clause_if clause_ifcons clause_after clause_unless clause_where PERIOD",
 /* 447 */ "law_nonexecutable ::= NONEXECUTABLE formula clause_if clause_unless clause_where PERIOD",
 /* 448 */ "law_rigid ::= RIGID constant clause_where PERIOD",
 /* 449 */ "law_observed ::= OBSERVED atomic_head_formula AT term_int_eval PERIOD",
 /* 450 */ "law_temporal_constraint ::= CONSTRAINT formula AT term_int_eval PERIOD",
 /* 451 */ "stmt_code_blk ::= ASP_GR",
 /* 452 */ "stmt_code_blk ::= ASP_CP",
 /* 453 */ "stmt_code_blk ::= F2LP_GR",
 /* 454 */ "stmt_code_blk ::= F2LP_CP",
 /* 455 */ "stmt_code_blk ::= LUA_GR",
 /* 456 */ "stmt_code_blk ::= LUA_CP",
 /* 457 */ "stmt_code_blk ::= PYTHON_GR",
 /* 458 */ "stmt_code_blk ::= PYTHON_CP",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to lemon_parser and lemon_parserFree.
*/
void *lemon_parserAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
	pParser->yylookmajor = YYNOCODE;
	pParser->yylookminor = yyzerominor;
	pParser->yysyntaxerr = 0;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  lemon_parserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
      /* TERMINAL Destructor */
    case 1: /* INTEGER */
    case 2: /* IDENTIFIER */
    case 3: /* POUND_IDENTIFIER */
    case 4: /* POUND_INTEGER */
    case 5: /* AT_IDENTIFIER */
    case 6: /* STRING_LITERAL */
    case 7: /* ASP_GR */
    case 8: /* ASP_CP */
    case 9: /* LUA_GR */
    case 10: /* LUA_CP */
    case 11: /* PYTHON_GR */
    case 12: /* PYTHON_CP */
    case 13: /* F2LP_GR */
    case 14: /* F2LP_CP */
    case 15: /* COMMENT */
    case 16: /* CONSTANTS */
    case 17: /* INCLUDE */
    case 18: /* MACROS */
    case 19: /* OBJECTS */
    case 20: /* QUERY */
    case 21: /* SHOW */
    case 22: /* SORTS */
    case 23: /* VARIABLES */
    case 24: /* ABACTION */
    case 25: /* ACTION */
    case 26: /* ADDITIVEACTION */
    case 27: /* ADDITIVEFLUENT */
    case 28: /* AFTER */
    case 29: /* ALL */
    case 30: /* ALWAYS */
    case 31: /* ASSUMING */
    case 32: /* ATTRIBUTE */
    case 33: /* BY */
    case 34: /* CAUSED */
    case 35: /* CAUSES */
    case 36: /* IMPOSSIBLE */
    case 37: /* CONSTRAINT */
    case 38: /* DECREMENTS */
    case 39: /* DEFAULT */
    case 40: /* EXTERNALACTION */
    case 41: /* EXOGENOUS */
    case 42: /* EXOGENOUSACTION */
    case 43: /* IF */
    case 44: /* IFCONS */
    case 45: /* INCREMENTS */
    case 46: /* INERTIAL */
    case 47: /* INERTIALFLUENT */
    case 48: /* LABEL */
    case 49: /* MAY_CAUSE */
    case 50: /* MAXADDITIVE */
    case 51: /* MAXAFVALUE */
    case 52: /* MAXSTEP */
    case 53: /* NEVER */
    case 54: /* NOCONCURRENCY */
    case 55: /* STRONG_NOCONCURRENCY */
    case 56: /* NONEXECUTABLE */
    case 57: /* OF */
    case 58: /* POSSIBLY_CAUSED */
    case 59: /* RIGID */
    case 60: /* SDFLUENT */
    case 61: /* SIMPLEFLUENT */
    case 62: /* EXTERNALFLUENT */
    case 63: /* UNLESS */
    case 64: /* WHERE */
    case 65: /* FALSE */
    case 66: /* TRUE */
    case 67: /* AT */
    case 68: /* BRACKET_L */
    case 69: /* BRACKET_R */
    case 70: /* COLON_DASH */
    case 71: /* CBRACKET_L */
    case 72: /* CBRACKET_R */
    case 73: /* PAREN_L */
    case 74: /* PAREN_R */
    case 75: /* PERIOD */
    case 76: /* MACRO_STRING */
    case 77: /* TILDE */
    case 78: /* DBL_COLON */
    case 79: /* ARROW_LEQ */
    case 80: /* ARROW_REQ */
    case 81: /* ARROW_LDASH */
    case 82: /* COLON */
    case 83: /* EQ */
    case 84: /* DBL_EQ */
    case 85: /* NEQ */
    case 86: /* NOT_EQ */
    case 87: /* LTHAN */
    case 88: /* GTHAN */
    case 89: /* LTHAN_EQ */
    case 90: /* GTHAN_EQ */
    case 91: /* DBL_PERIOD */
    case 92: /* BIG_CONJ */
    case 93: /* BIG_DISJ */
    case 94: /* POUND */
    case 95: /* SEMICOLON */
    case 96: /* EQUIV */
    case 97: /* IMPL */
    case 98: /* ARROW_RDASH */
    case 99: /* DBL_PLUS */
    case 100: /* PIPE */
    case 101: /* DBL_GTHAN */
    case 102: /* DBL_LTHAN */
    case 103: /* AMP */
    case 104: /* COMMA */
    case 105: /* DBL_AMP */
    case 106: /* NOT */
    case 107: /* DASH */
    case 108: /* PLUS */
    case 109: /* STAR */
    case 110: /* INT_DIV */
    case 111: /* MOD */
    case 112: /* ABS */
    case 113: /* CARROT */
    case 114: /* UMINUS */
    case 115: /* PREC_4 */
    case 116: /* PREC_3 */
    case 117: /* PREC_2 */
    case 118: /* PREC_1 */
    case 119: /* PREC_0 */
    case 120: /* EOF */
    case 121: /* ERR_IO */
    case 122: /* ERR_UNKNOWN_SYMBOL */
    case 123: /* ERR_UNTERMINATED_STRING */
    case 124: /* ERR_UNTERMINATED_ASP */
    case 125: /* ERR_UNTERMINATED_LUA */
    case 126: /* ERR_UNTERMINATED_PYTHON */
    case 127: /* ERR_UNTERMINATED_F2LP */
    case 128: /* ERR_UNTERMINATED_BLK_COMMENT */
    case 129: /* ERR_SYNTAX */
    case 130: /* ERR_PAREN_MISMATCH */
    case 131: /* ARG */
    case 132: /* NOOP */
    case 133: /* CONSTANT_ID */
    case 134: /* VARIABLE_ID */
    case 135: /* OBJECT_ID */
    case 136: /* HIDE */
    case 137: /* OBSERVED */
{
#line 201 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy0));								
#line 2618 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 139: /* start */
    case 140: /* statement_lst */
    case 163: /* undeclared */
{
#line 211 "bcplus/parser/detail/lemon_parser.y"
 /* Intentionally left blank */			
#line 2627 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 141: /* statement */
    case 147: /* stmt_code_blk */
    case 148: /* stmt_law */
    case 149: /* stmt_show */
    case 150: /* stmt_hide */
    case 153: /* stmt_maxafvalue */
    case 154: /* stmt_maxadditive */
{
#line 215 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy376));								
#line 2640 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 142: /* stmt_macro_def */
{
#line 236 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy39));								
#line 2647 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 143: /* stmt_constant_def */
{
#line 238 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy151));								
#line 2654 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 144: /* stmt_object_def */
{
#line 240 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy492));								
#line 2661 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 145: /* stmt_variable_def */
{
#line 242 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy40));								
#line 2668 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 146: /* stmt_sort_def */
{
#line 244 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy93));								
#line 2675 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 151: /* stmt_noconcurrency */
{
#line 254 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy441));								
#line 2682 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 152: /* stmt_strong_noconcurrency */
{
#line 256 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy370));								
#line 2689 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 155: /* stmt_query */
{
#line 262 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy482));								
#line 2696 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 156: /* base_elem */
    case 157: /* base_elem_no_const */
    case 165: /* term */
    case 168: /* term_no_const */
    case 170: /* term_strong */
    case 171: /* term_strong_candidate */
    case 172: /* term_no_const_strong */
    case 175: /* term_integral */
    case 192: /* term_temporal */
    case 193: /* term_temporal_strong */
    case 194: /* term_temporal_strong_candidate */
    case 195: /* constant_temporal */
{
#line 296 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy223));								
#line 2714 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 158: /* constant */
    case 166: /* constant_one_const */
    case 169: /* const_anon */
{
#line 300 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy301));								
#line 2723 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 159: /* object */
    case 160: /* object_nullary */
{
#line 302 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy186));								
#line 2731 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 161: /* variable */
{
#line 306 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy1));								
#line 2738 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 162: /* lua */
{
#line 308 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy457));								
#line 2745 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 164: /* term_lst */
    case 167: /* term_no_const_lst */
{
#line 312 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy491));								
#line 2753 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 173: /* num_range */
{
#line 726 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy513));								
#line 2760 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 174: /* num_range_eval */
{
#line 728 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy97));								
#line 2767 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 176: /* term_int_eval */
{
#line 732 "bcplus/parser/detail/lemon_parser.y"
 /* Initially left Blank */				
#line 2774 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 177: /* formula */
    case 178: /* formula_base */
    case 179: /* comparison */
    case 182: /* formula_card */
    case 184: /* formula_no_const */
    case 185: /* formula_no_const_base */
    case 186: /* comparison_no_const */
    case 196: /* formula_temporal */
    case 197: /* formula_temporal_base */
    case 198: /* comparison_temporal */
    case 200: /* formula_temporal_card */
    case 201: /* head_formula */
{
#line 833 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy201));								
#line 2792 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 180: /* atomic_formula */
    case 183: /* atomic_formula_anon */
    case 187: /* atomic_formula_one_const */
    case 202: /* atomic_head_formula */
{
#line 839 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy371));								
#line 2802 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 181: /* formula_quant */
    case 199: /* formula_temporal_quant */
{
#line 841 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy77));								
#line 2810 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 188: /* quant_lst */
{
#line 1020 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy449));								
#line 2817 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 189: /* quant_op */
{
#line 1022 "bcplus/parser/detail/lemon_parser.y"
 /* Intentionally left blank */			
#line 2824 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 190: /* card_var_lst */
    case 191: /* card_var_lst_inner */
{
#line 1059 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy187));								
#line 2832 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 203: /* formula_smpl_card */
{
#line 1366 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy81));								
#line 2839 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 204: /* macro_def_lst */
{
#line 1430 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy458));                              
#line 2846 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 205: /* macro_bnd */
{
#line 1432 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy427));                              
#line 2853 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 206: /* macro_args */
{
#line 1434 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy374));                              
#line 2860 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 207: /* macro_arg */
{
#line 1436 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy167));                              
#line 2867 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 208: /* sort_lst */
{
#line 1526 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy255));							
#line 2874 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 209: /* sort */
    case 210: /* sort_id_nr */
    case 211: /* sort_nr */
    case 212: /* sort_id */
{
#line 1528 "bcplus/parser/detail/lemon_parser.y"
 /* Intentionally left blank */		
#line 2884 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 213: /* constant_bnd_lst */
    case 214: /* constant_bnd */
{
#line 1637 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy37));									
#line 2892 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 215: /* constant_dcl_lst */
{
#line 1641 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy414));									
#line 2899 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 216: /* constant_dcl_type */
{
#line 1643 "bcplus/parser/detail/lemon_parser.y"
 /* Intentionally left blank */				
#line 2906 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 217: /* attrib_spec */
{
#line 1645 "bcplus/parser/detail/lemon_parser.y"
 /* Intentionally left blank */				
#line 2913 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 218: /* object_bnd_lst */
{
#line 2004 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy262));									
#line 2920 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 219: /* object_bnd */
{
#line 2006 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy302));									
#line 2927 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 220: /* object_lst */
    case 221: /* object_spec */
{
#line 2008 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy285));									
#line 2935 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 222: /* variable_bnd_lst */
    case 223: /* variable_bnd */
{
#line 2140 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy89));									
#line 2943 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 224: /* variable_lst */
{
#line 2144 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy112));									
#line 2950 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 225: /* sort_bnd_lst */
    case 226: /* sort_bnd */
    case 227: /* sort_dcl_lst */
{
#line 2227 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy247));									
#line 2959 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 228: /* show_lst */
{
#line 2331 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy323));									
#line 2966 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 229: /* show_elem */
    case 237: /* clause_unless */
{
#line 2333 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy371));									
#line 2974 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 230: /* query_lst */
{
#line 2485 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy289).l); DEALLOC((yypminor->yy289).maxstep); DEALLOC((yypminor->yy289).label);	
#line 2981 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 231: /* query_maxstep_decl */
{
#line 2487 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy97));												
#line 2988 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 233: /* query_label_Decl */
{
#line 2489 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy0));												
#line 2995 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 234: /* clause_if */
    case 235: /* clause_after */
    case 236: /* clause_ifcons */
    case 238: /* clause_where */
{
#line 2643 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy201));									
#line 3005 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    case 239: /* law_basic */
    case 240: /* law_caused */
    case 241: /* law_pcaused */
    case 242: /* law_impl */
    case 243: /* law_causes */
    case 244: /* law_increments */
    case 245: /* law_decrements */
    case 246: /* law_mcause */
    case 247: /* law_always */
    case 248: /* law_constraint */
    case 249: /* law_impossible */
    case 250: /* law_never */
    case 251: /* law_default */
    case 252: /* law_exogenous */
    case 253: /* law_inertial */
    case 254: /* law_nonexecutable */
    case 255: /* law_rigid */
    case 256: /* law_observed */
    case 257: /* law_temporal_constraint */
{
#line 2684 "bcplus/parser/detail/lemon_parser.y"
 DEALLOC((yypminor->yy376));									
#line 3030 "bcplus/parser/detail/lemon_parser.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
  lemon_parserARG_STORE;
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from lemon_parserAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void lemon_parserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  if( pParser->yylookmajor != YYNOCODE ) yy_destructor(pParser, (YYCODETYPE)pParser->yylookmajor, &pParser->yylookminor);
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int lemon_parserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  if (iLookAhead == YYNOCODE) return YY_NO_ACTION;
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser /*, YYMINORTYPE *yypMinor */){
   lemon_parserARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
   lemon_parserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser/*, yypMinor */);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser/*, yypMinor */);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 139, 2 },
  { 140, 0 },
  { 140, 2 },
  { 140, 2 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 156, 1 },
  { 156, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 158, 4 },
  { 158, 1 },
  { 169, 1 },
  { 169, 4 },
  { 159, 4 },
  { 159, 1 },
  { 160, 1 },
  { 159, 1 },
  { 161, 1 },
  { 162, 4 },
  { 162, 1 },
  { 162, 3 },
  { 163, 4 },
  { 163, 1 },
  { 164, 1 },
  { 164, 3 },
  { 166, 4 },
  { 166, 1 },
  { 167, 1 },
  { 167, 3 },
  { 165, 1 },
  { 165, 1 },
  { 165, 1 },
  { 165, 3 },
  { 165, 1 },
  { 165, 1 },
  { 165, 1 },
  { 165, 1 },
  { 165, 1 },
  { 165, 2 },
  { 165, 2 },
  { 165, 3 },
  { 165, 3 },
  { 165, 3 },
  { 165, 3 },
  { 165, 3 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 3 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 2 },
  { 170, 2 },
  { 171, 2 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 3 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 2 },
  { 172, 2 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 3 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 2 },
  { 168, 2 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 175, 1 },
  { 175, 3 },
  { 175, 1 },
  { 175, 1 },
  { 175, 1 },
  { 175, 1 },
  { 175, 1 },
  { 175, 2 },
  { 175, 2 },
  { 175, 3 },
  { 175, 3 },
  { 175, 3 },
  { 175, 3 },
  { 175, 3 },
  { 173, 3 },
  { 174, 3 },
  { 176, 1 },
  { 176, 3 },
  { 176, 2 },
  { 176, 2 },
  { 176, 3 },
  { 176, 3 },
  { 176, 3 },
  { 176, 3 },
  { 176, 3 },
  { 177, 1 },
  { 177, 3 },
  { 177, 2 },
  { 177, 2 },
  { 177, 3 },
  { 177, 3 },
  { 177, 3 },
  { 177, 3 },
  { 177, 3 },
  { 177, 3 },
  { 178, 1 },
  { 178, 1 },
  { 178, 1 },
  { 178, 1 },
  { 178, 1 },
  { 178, 1 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 179, 3 },
  { 180, 1 },
  { 180, 2 },
  { 180, 3 },
  { 183, 1 },
  { 183, 1 },
  { 183, 2 },
  { 183, 3 },
  { 184, 1 },
  { 184, 3 },
  { 184, 2 },
  { 184, 2 },
  { 184, 3 },
  { 184, 3 },
  { 184, 3 },
  { 184, 3 },
  { 184, 3 },
  { 184, 3 },
  { 185, 1 },
  { 185, 1 },
  { 185, 1 },
  { 186, 3 },
  { 186, 3 },
  { 186, 3 },
  { 186, 3 },
  { 186, 3 },
  { 186, 3 },
  { 186, 3 },
  { 187, 1 },
  { 187, 2 },
  { 187, 3 },
  { 181, 5 },
  { 188, 2 },
  { 188, 3 },
  { 189, 1 },
  { 189, 1 },
  { 182, 4 },
  { 182, 5 },
  { 182, 5 },
  { 182, 6 },
  { 182, 3 },
  { 182, 4 },
  { 182, 4 },
  { 182, 5 },
  { 190, 2 },
  { 191, 1 },
  { 191, 3 },
  { 192, 1 },
  { 192, 1 },
  { 192, 1 },
  { 192, 3 },
  { 192, 1 },
  { 192, 1 },
  { 192, 1 },
  { 192, 1 },
  { 192, 1 },
  { 192, 1 },
  { 192, 2 },
  { 192, 2 },
  { 192, 3 },
  { 192, 3 },
  { 192, 3 },
  { 192, 3 },
  { 192, 3 },
  { 192, 3 },
  { 193, 1 },
  { 193, 1 },
  { 193, 1 },
  { 193, 3 },
  { 193, 1 },
  { 193, 1 },
  { 193, 1 },
  { 193, 3 },
  { 193, 2 },
  { 193, 2 },
  { 193, 3 },
  { 193, 3 },
  { 193, 3 },
  { 193, 3 },
  { 193, 3 },
  { 196, 1 },
  { 196, 3 },
  { 196, 2 },
  { 196, 2 },
  { 196, 3 },
  { 196, 3 },
  { 196, 3 },
  { 196, 3 },
  { 196, 3 },
  { 196, 3 },
  { 196, 3 },
  { 197, 1 },
  { 197, 1 },
  { 197, 1 },
  { 197, 1 },
  { 197, 1 },
  { 197, 1 },
  { 198, 3 },
  { 198, 3 },
  { 198, 3 },
  { 198, 3 },
  { 198, 3 },
  { 198, 3 },
  { 198, 3 },
  { 199, 5 },
  { 200, 4 },
  { 200, 5 },
  { 200, 5 },
  { 200, 6 },
  { 200, 3 },
  { 200, 4 },
  { 200, 4 },
  { 200, 5 },
  { 201, 3 },
  { 201, 3 },
  { 201, 1 },
  { 201, 1 },
  { 201, 1 },
  { 201, 1 },
  { 201, 1 },
  { 202, 1 },
  { 202, 2 },
  { 203, 4 },
  { 203, 5 },
  { 203, 5 },
  { 203, 6 },
  { 203, 3 },
  { 203, 4 },
  { 203, 4 },
  { 203, 5 },
  { 142, 4 },
  { 204, 1 },
  { 204, 3 },
  { 205, 6 },
  { 205, 3 },
  { 206, 1 },
  { 206, 3 },
  { 207, 1 },
  { 207, 1 },
  { 208, 1 },
  { 208, 3 },
  { 209, 1 },
  { 209, 2 },
  { 209, 2 },
  { 209, 3 },
  { 209, 3 },
  { 209, 3 },
  { 210, 1 },
  { 210, 1 },
  { 211, 1 },
  { 212, 1 },
  { 143, 4 },
  { 213, 1 },
  { 213, 3 },
  { 214, 6 },
  { 214, 3 },
  { 214, 3 },
  { 214, 5 },
  { 214, 8 },
  { 215, 1 },
  { 215, 4 },
  { 215, 3 },
  { 215, 6 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 216, 1 },
  { 217, 1 },
  { 217, 4 },
  { 144, 4 },
  { 218, 1 },
  { 218, 3 },
  { 219, 3 },
  { 220, 1 },
  { 220, 3 },
  { 221, 1 },
  { 221, 4 },
  { 221, 1 },
  { 221, 1 },
  { 145, 4 },
  { 222, 1 },
  { 222, 3 },
  { 223, 3 },
  { 224, 1 },
  { 224, 3 },
  { 146, 4 },
  { 225, 1 },
  { 225, 3 },
  { 226, 1 },
  { 226, 3 },
  { 226, 3 },
  { 226, 3 },
  { 227, 1 },
  { 227, 3 },
  { 149, 4 },
  { 149, 4 },
  { 150, 4 },
  { 150, 4 },
  { 228, 1 },
  { 228, 3 },
  { 228, 3 },
  { 229, 1 },
  { 151, 2 },
  { 152, 2 },
  { 153, 5 },
  { 153, 5 },
  { 154, 5 },
  { 154, 5 },
  { 155, 4 },
  { 230, 1 },
  { 230, 1 },
  { 230, 1 },
  { 230, 3 },
  { 230, 3 },
  { 230, 3 },
  { 231, 3 },
  { 231, 3 },
  { 232, 3 },
  { 232, 3 },
  { 234, 2 },
  { 234, 0 },
  { 235, 2 },
  { 235, 0 },
  { 236, 2 },
  { 236, 0 },
  { 237, 2 },
  { 237, 0 },
  { 238, 2 },
  { 238, 0 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 239, 7 },
  { 240, 8 },
  { 241, 8 },
  { 242, 5 },
  { 242, 4 },
  { 243, 7 },
  { 244, 9 },
  { 245, 9 },
  { 246, 7 },
  { 247, 6 },
  { 248, 6 },
  { 249, 6 },
  { 250, 6 },
  { 251, 8 },
  { 252, 8 },
  { 253, 8 },
  { 254, 6 },
  { 255, 4 },
  { 256, 5 },
  { 257, 5 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
};

/*
** Flags that a syntax error has occurred. 
*/
#define YYERROR { yypParser->yysyntaxerr = 1; yypParser->yyerrcnt = 3; }

static void yy_accept(yyParser*);  /* Forward Declaration */


/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  lemon_parserARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0: /* start ::= statement_lst EOF */
#line 217 "bcplus/parser/detail/lemon_parser.y"
{
  yy_destructor(yypParser,120,&yymsp[0].minor);
}
#line 3794 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 3: /* statement_lst ::= statement_lst statement */
#line 222 "bcplus/parser/detail/lemon_parser.y"
{
			ref_ptr<Statement> ptr = yymsp[0].minor.yy376;
			yymsp[0].minor.yy376  = NULL;
			parser->_handle_stmt(ptr);
		}
#line 3803 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 4: /* statement ::= stmt_macro_def */
#line 265 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy39; }
#line 3808 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 5: /* statement ::= stmt_constant_def */
#line 266 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy151; }
#line 3813 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 6: /* statement ::= stmt_object_def */
#line 267 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy492; }
#line 3818 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 7: /* statement ::= stmt_variable_def */
#line 268 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy40; }
#line 3823 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 8: /* statement ::= stmt_sort_def */
#line 269 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy93; }
#line 3828 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 9: /* statement ::= stmt_code_blk */
      case 10: /* statement ::= stmt_law */ yytestcase(yyruleno==10);
      case 11: /* statement ::= stmt_show */ yytestcase(yyruleno==11);
      case 12: /* statement ::= stmt_hide */ yytestcase(yyruleno==12);
      case 15: /* statement ::= stmt_maxafvalue */ yytestcase(yyruleno==15);
      case 16: /* statement ::= stmt_maxadditive */ yytestcase(yyruleno==16);
#line 270 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy376; }
#line 3838 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 13: /* statement ::= stmt_noconcurrency */
#line 274 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy441; }
#line 3843 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 14: /* statement ::= stmt_strong_noconcurrency */
#line 275 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy370; }
#line 3848 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 17: /* statement ::= stmt_query */
#line 278 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy376 = yymsp[0].minor.yy482; }
#line 3853 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 18: /* base_elem ::= constant */
#line 324 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy223 = yymsp[0].minor.yy301; }
#line 3858 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 19: /* base_elem ::= base_elem_no_const */
      case 43: /* term ::= base_elem */ yytestcase(yyruleno==43);
      case 59: /* term_strong ::= base_elem_no_const */ yytestcase(yyruleno==59);
      case 84: /* term_no_const_strong ::= base_elem_no_const */ yytestcase(yyruleno==84);
      case 99: /* term_no_const ::= base_elem_no_const */ yytestcase(yyruleno==99);
      case 223: /* term_temporal ::= base_elem_no_const */ yytestcase(yyruleno==223);
      case 241: /* term_temporal_strong ::= base_elem_no_const */ yytestcase(yyruleno==241);
#line 325 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy223 = yymsp[0].minor.yy223; }
#line 3869 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 20: /* base_elem_no_const ::= object */
#line 327 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy223 = yymsp[0].minor.yy186;	}
#line 3874 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 21: /* base_elem_no_const ::= variable */
#line 328 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy223 = yymsp[0].minor.yy1; }
#line 3879 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 22: /* base_elem_no_const ::= lua */
#line 329 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy223 = yymsp[0].minor.yy457; }
#line 3884 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 23: /* constant ::= CONSTANT_ID PAREN_L term_lst PAREN_R */
      case 39: /* constant_one_const ::= CONSTANT_ID PAREN_L term_no_const_lst PAREN_R */ yytestcase(yyruleno==39);
#line 456 "bcplus/parser/detail/lemon_parser.y"
{ BASE_ELEM_DEF(yygotominor.yy301, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[-1].minor.yy491, yymsp[0].minor.yy0, Symbol::Type::CONSTANT, Constant, ConstantSymbol);	}
#line 3890 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 24: /* constant ::= CONSTANT_ID */
      case 40: /* constant_one_const ::= CONSTANT_ID */ yytestcase(yyruleno==40);
#line 457 "bcplus/parser/detail/lemon_parser.y"
{ BASE_ELEM_DEF(yygotominor.yy301, yymsp[0].minor.yy0, NULL, NULL, NULL, Symbol::Type::CONSTANT, Constant, ConstantSymbol); }
#line 3896 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 25: /* const_anon ::= IDENTIFIER */
#line 459 "bcplus/parser/detail/lemon_parser.y"
{ BASE_ELEM_DEF9(yygotominor.yy301, yymsp[0].minor.yy0, NULL, NULL, NULL, Symbol::Type::CONSTANT, Constant, ConstantSymbol, true); }
#line 3901 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 26: /* const_anon ::= IDENTIFIER PAREN_L term_lst PAREN_R */
#line 460 "bcplus/parser/detail/lemon_parser.y"
{ BASE_ELEM_DEF9(yygotominor.yy301, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[-1].minor.yy491, yymsp[0].minor.yy0, Symbol::Type::CONSTANT, Constant, ConstantSymbol, true);	}
#line 3906 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 27: /* object ::= OBJECT_ID PAREN_L term_lst PAREN_R */
#line 463 "bcplus/parser/detail/lemon_parser.y"
{ BASE_ELEM_DEF(yygotominor.yy186, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[-1].minor.yy491, yymsp[0].minor.yy0, Symbol::Type::OBJECT, Object, ObjectSymbol);	}
#line 3911 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 28: /* object ::= object_nullary */
#line 464 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy186 = yymsp[0].minor.yy186; }
#line 3916 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 29: /* object_nullary ::= OBJECT_ID */
#line 465 "bcplus/parser/detail/lemon_parser.y"
{ BASE_ELEM_DEF(yygotominor.yy186, yymsp[0].minor.yy0, NULL, NULL, NULL, Symbol::Type::OBJECT, Object, ObjectSymbol); }
#line 3921 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 30: /* object ::= undeclared */
#line 466 "bcplus/parser/detail/lemon_parser.y"
{ /* never called */ }
#line 3926 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 31: /* variable ::= VARIABLE_ID */
#line 469 "bcplus/parser/detail/lemon_parser.y"
{ 
		yygotominor.yy1 = NULL;
		ref_ptr<const Token> id_ptr = yymsp[0].minor.yy0;
		
		if (!parser->lang()->support(Language::Feature::VARIABLE)) {
			parser->_feature_error(Language::Feature::VARIABLE, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else {
			BASE_ELEM_BARE_DEF(yygotominor.yy1, yymsp[0].minor.yy0, Symbol::Type::VARIABLE, Variable, VariableSymbol); 
		}
	}
#line 3941 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 32: /* lua ::= AT_IDENTIFIER PAREN_L term_lst PAREN_R */
#line 480 "bcplus/parser/detail/lemon_parser.y"
{ BASE_LUA_ELEM(yygotominor.yy457, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[-1].minor.yy491, yymsp[0].minor.yy0); }
#line 3946 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 33: /* lua ::= AT_IDENTIFIER */
#line 481 "bcplus/parser/detail/lemon_parser.y"
{ BASE_LUA_ELEM(yygotominor.yy457, yymsp[0].minor.yy0, NULL, NULL, NULL); }
#line 3951 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 34: /* lua ::= AT_IDENTIFIER PAREN_L PAREN_R */
#line 482 "bcplus/parser/detail/lemon_parser.y"
{ BASE_LUA_ELEM(yygotominor.yy457, yymsp[-2].minor.yy0, yymsp[-1].minor.yy0, NULL, yymsp[0].minor.yy0); }
#line 3956 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 35: /* undeclared ::= IDENTIFIER PAREN_L term_lst PAREN_R */
#line 485 "bcplus/parser/detail/lemon_parser.y"
{ UNDECLARED(yygotominor.yy361, yymsp[-3].minor.yy0, yymsp[-1].minor.yy491);   yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 3963 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 36: /* undeclared ::= IDENTIFIER */
#line 486 "bcplus/parser/detail/lemon_parser.y"
{ UNDECLARED(yygotominor.yy361, yymsp[0].minor.yy0, NULL); }
#line 3968 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 37: /* term_lst ::= term */
      case 41: /* term_no_const_lst ::= term_no_const */ yytestcase(yyruleno==41);
#line 489 "bcplus/parser/detail/lemon_parser.y"
{
			yygotominor.yy491 = new TermList();
			yygotominor.yy491->push_back(yymsp[0].minor.yy223);
		}
#line 3977 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 38: /* term_lst ::= term_lst COMMA term */
      case 42: /* term_no_const_lst ::= term_no_const_lst COMMA term_no_const */ yytestcase(yyruleno==42);
#line 495 "bcplus/parser/detail/lemon_parser.y"
{
			yygotominor.yy491 = yymsp[-2].minor.yy491;
			yymsp[-2].minor.yy491->push_back(yymsp[0].minor.yy223);
		  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 3987 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 44: /* term ::= INTEGER */
      case 60: /* term_strong ::= INTEGER */ yytestcase(yyruleno==60);
      case 85: /* term_no_const_strong ::= INTEGER */ yytestcase(yyruleno==85);
      case 100: /* term_no_const ::= INTEGER */ yytestcase(yyruleno==100);
      case 116: /* term_integral ::= INTEGER */ yytestcase(yyruleno==116);
      case 224: /* term_temporal ::= INTEGER */ yytestcase(yyruleno==224);
      case 242: /* term_temporal_strong ::= INTEGER */ yytestcase(yyruleno==242);
#line 594 "bcplus/parser/detail/lemon_parser.y"
{ BASIC_TERM(yygotominor.yy223, yymsp[0].minor.yy0);	}
#line 3998 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 45: /* term ::= STRING_LITERAL */
      case 47: /* term ::= TRUE */ yytestcase(yyruleno==47);
      case 48: /* term ::= FALSE */ yytestcase(yyruleno==48);
      case 61: /* term_strong ::= STRING_LITERAL */ yytestcase(yyruleno==61);
      case 86: /* term_no_const_strong ::= STRING_LITERAL */ yytestcase(yyruleno==86);
      case 101: /* term_no_const ::= STRING_LITERAL */ yytestcase(yyruleno==101);
      case 103: /* term_no_const ::= TRUE */ yytestcase(yyruleno==103);
      case 104: /* term_no_const ::= FALSE */ yytestcase(yyruleno==104);
      case 118: /* term_integral ::= TRUE */ yytestcase(yyruleno==118);
      case 119: /* term_integral ::= FALSE */ yytestcase(yyruleno==119);
      case 225: /* term_temporal ::= STRING_LITERAL */ yytestcase(yyruleno==225);
      case 227: /* term_temporal ::= TRUE */ yytestcase(yyruleno==227);
      case 228: /* term_temporal ::= FALSE */ yytestcase(yyruleno==228);
      case 243: /* term_temporal_strong ::= STRING_LITERAL */ yytestcase(yyruleno==243);
#line 595 "bcplus/parser/detail/lemon_parser.y"
{ BASIC_TERM(yygotominor.yy223, yymsp[0].minor.yy0); }
#line 4016 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 46: /* term ::= PAREN_L term PAREN_R */
      case 62: /* term_strong ::= PAREN_L term_strong PAREN_R */ yytestcase(yyruleno==62);
      case 87: /* term_no_const_strong ::= PAREN_L term_no_const_strong PAREN_R */ yytestcase(yyruleno==87);
      case 102: /* term_no_const ::= PAREN_L term_no_const PAREN_R */ yytestcase(yyruleno==102);
      case 117: /* term_integral ::= PAREN_L term_integral PAREN_R */ yytestcase(yyruleno==117);
      case 226: /* term_temporal ::= PAREN_L term_temporal PAREN_R */ yytestcase(yyruleno==226);
      case 244: /* term_temporal_strong ::= PAREN_L term_temporal_strong PAREN_R */ yytestcase(yyruleno==244);
#line 596 "bcplus/parser/detail/lemon_parser.y"
{ TERM_PARENS(yygotominor.yy223, yymsp[-2].minor.yy0, yymsp[-1].minor.yy223, yymsp[0].minor.yy0); }
#line 4027 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 49: /* term ::= MAXSTEP */
      case 63: /* term_strong ::= MAXSTEP */ yytestcase(yyruleno==63);
      case 88: /* term_no_const_strong ::= MAXSTEP */ yytestcase(yyruleno==88);
      case 105: /* term_no_const ::= MAXSTEP */ yytestcase(yyruleno==105);
      case 120: /* term_integral ::= MAXSTEP */ yytestcase(yyruleno==120);
      case 229: /* term_temporal ::= MAXSTEP */ yytestcase(yyruleno==229);
      case 245: /* term_temporal_strong ::= MAXSTEP */ yytestcase(yyruleno==245);
#line 599 "bcplus/parser/detail/lemon_parser.y"
{ NULLARY_TERM(yygotominor.yy223, yymsp[0].minor.yy0, Language::Feature::MAXSTEP, NullaryTerm::Operator::MAXSTEP); }
#line 4038 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 50: /* term ::= MAXADDITIVE */
      case 64: /* term_strong ::= MAXADDITIVE */ yytestcase(yyruleno==64);
      case 89: /* term_no_const_strong ::= MAXADDITIVE */ yytestcase(yyruleno==89);
      case 106: /* term_no_const ::= MAXADDITIVE */ yytestcase(yyruleno==106);
      case 121: /* term_integral ::= MAXADDITIVE */ yytestcase(yyruleno==121);
      case 230: /* term_temporal ::= MAXADDITIVE */ yytestcase(yyruleno==230);
      case 246: /* term_temporal_strong ::= MAXADDITIVE */ yytestcase(yyruleno==246);
#line 600 "bcplus/parser/detail/lemon_parser.y"
{ NULLARY_TERM(yygotominor.yy223, yymsp[0].minor.yy0, Language::Feature::MAXADDITIVE, NullaryTerm::Operator::MAXADDITIVE); }
#line 4049 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 51: /* term ::= MAXAFVALUE */
      case 65: /* term_strong ::= MAXAFVALUE */ yytestcase(yyruleno==65);
      case 90: /* term_no_const_strong ::= MAXAFVALUE */ yytestcase(yyruleno==90);
      case 107: /* term_no_const ::= MAXAFVALUE */ yytestcase(yyruleno==107);
      case 122: /* term_integral ::= MAXAFVALUE */ yytestcase(yyruleno==122);
      case 231: /* term_temporal ::= MAXAFVALUE */ yytestcase(yyruleno==231);
      case 247: /* term_temporal_strong ::= MAXAFVALUE */ yytestcase(yyruleno==247);
#line 601 "bcplus/parser/detail/lemon_parser.y"
{ NULLARY_TERM(yygotominor.yy223, yymsp[0].minor.yy0, Language::Feature::MAXAFVALUE, NullaryTerm::Operator::MAXAFVALUE); }
#line 4060 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 52: /* term ::= DASH term */
      case 66: /* term_strong ::= DASH term_strong */ yytestcase(yyruleno==66);
      case 92: /* term_no_const_strong ::= DASH term_no_const_strong */ yytestcase(yyruleno==92);
      case 109: /* term_no_const ::= DASH term_no_const */ yytestcase(yyruleno==109);
      case 123: /* term_integral ::= DASH term_integral */ yytestcase(yyruleno==123);
      case 233: /* term_temporal ::= DASH term_temporal */ yytestcase(yyruleno==233);
      case 249: /* term_temporal_strong ::= DASH term_temporal_strong */ yytestcase(yyruleno==249);
#line 605 "bcplus/parser/detail/lemon_parser.y"
{ UNARY_ARITH(yygotominor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, UnaryTerm::Operator::NEGATIVE); }
#line 4071 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 53: /* term ::= ABS term */
      case 67: /* term_strong ::= ABS term */ yytestcase(yyruleno==67);
      case 93: /* term_no_const_strong ::= ABS term_no_const */ yytestcase(yyruleno==93);
      case 110: /* term_no_const ::= ABS term_no_const */ yytestcase(yyruleno==110);
      case 124: /* term_integral ::= ABS term_integral */ yytestcase(yyruleno==124);
      case 234: /* term_temporal ::= ABS term_temporal */ yytestcase(yyruleno==234);
      case 250: /* term_temporal_strong ::= ABS term_temporal */ yytestcase(yyruleno==250);
#line 606 "bcplus/parser/detail/lemon_parser.y"
{ UNARY_ARITH(yygotominor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, UnaryTerm::Operator::ABS); }
#line 4082 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 54: /* term ::= term DASH term */
      case 69: /* term_strong ::= term_strong_candidate DASH term */ yytestcase(yyruleno==69);
      case 79: /* term_strong ::= term_strong DASH term */ yytestcase(yyruleno==79);
      case 94: /* term_no_const_strong ::= term_no_const_strong DASH term_no_const */ yytestcase(yyruleno==94);
      case 111: /* term_no_const ::= term_no_const DASH term_no_const */ yytestcase(yyruleno==111);
      case 125: /* term_integral ::= term_integral DASH term_integral */ yytestcase(yyruleno==125);
      case 236: /* term_temporal ::= term_temporal DASH term_temporal */ yytestcase(yyruleno==236);
      case 251: /* term_temporal_strong ::= term_temporal_strong DASH term_temporal */ yytestcase(yyruleno==251);
#line 610 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::MINUS); }
#line 4094 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 55: /* term ::= term PLUS term */
      case 70: /* term_strong ::= term_strong_candidate PLUS term */ yytestcase(yyruleno==70);
      case 80: /* term_strong ::= term_strong PLUS term */ yytestcase(yyruleno==80);
      case 95: /* term_no_const_strong ::= term_no_const_strong PLUS term_no_const */ yytestcase(yyruleno==95);
      case 112: /* term_no_const ::= term_no_const PLUS term_no_const */ yytestcase(yyruleno==112);
      case 126: /* term_integral ::= term_integral PLUS term_integral */ yytestcase(yyruleno==126);
      case 237: /* term_temporal ::= term_temporal PLUS term_temporal */ yytestcase(yyruleno==237);
      case 252: /* term_temporal_strong ::= term_temporal_strong PLUS term_temporal */ yytestcase(yyruleno==252);
#line 611 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::PLUS); }
#line 4106 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 56: /* term ::= term STAR term */
      case 71: /* term_strong ::= term_strong_candidate STAR term */ yytestcase(yyruleno==71);
      case 81: /* term_strong ::= term_strong STAR term */ yytestcase(yyruleno==81);
      case 96: /* term_no_const_strong ::= term_no_const_strong STAR term_no_const */ yytestcase(yyruleno==96);
      case 113: /* term_no_const ::= term_no_const STAR term_no_const */ yytestcase(yyruleno==113);
      case 127: /* term_integral ::= term_integral STAR term_integral */ yytestcase(yyruleno==127);
      case 238: /* term_temporal ::= term_temporal STAR term_temporal */ yytestcase(yyruleno==238);
      case 253: /* term_temporal_strong ::= term_temporal_strong STAR term_temporal */ yytestcase(yyruleno==253);
#line 612 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::TIMES); }
#line 4118 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 57: /* term ::= term INT_DIV term */
      case 72: /* term_strong ::= term_strong_candidate INT_DIV term */ yytestcase(yyruleno==72);
      case 82: /* term_strong ::= term_strong INT_DIV term */ yytestcase(yyruleno==82);
      case 97: /* term_no_const_strong ::= term_no_const_strong INT_DIV term_no_const */ yytestcase(yyruleno==97);
      case 114: /* term_no_const ::= term_no_const INT_DIV term_no_const */ yytestcase(yyruleno==114);
      case 128: /* term_integral ::= term_integral INT_DIV term_integral */ yytestcase(yyruleno==128);
      case 239: /* term_temporal ::= term_temporal INT_DIV term_temporal */ yytestcase(yyruleno==239);
      case 254: /* term_temporal_strong ::= term_temporal_strong INT_DIV term_temporal */ yytestcase(yyruleno==254);
#line 613 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::DIVIDE); }
#line 4130 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 58: /* term ::= term MOD term */
      case 73: /* term_strong ::= term_strong_candidate MOD term */ yytestcase(yyruleno==73);
      case 83: /* term_strong ::= term_strong MOD term */ yytestcase(yyruleno==83);
      case 98: /* term_no_const_strong ::= term_no_const_strong MOD term_no_const */ yytestcase(yyruleno==98);
      case 115: /* term_no_const ::= term_no_const MOD term_no_const */ yytestcase(yyruleno==115);
      case 129: /* term_integral ::= term_integral MOD term_integral */ yytestcase(yyruleno==129);
      case 240: /* term_temporal ::= term_temporal MOD term_temporal */ yytestcase(yyruleno==240);
      case 255: /* term_temporal_strong ::= term_temporal_strong MOD term_temporal */ yytestcase(yyruleno==255);
#line 614 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::MOD); }
#line 4142 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 68: /* term_strong_candidate ::= DASH constant */
#line 633 "bcplus/parser/detail/lemon_parser.y"
{ UNARY_ARITH(yygotominor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy301, UnaryTerm::Operator::NEGATIVE); }
#line 4147 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 74: /* term_strong ::= constant DASH term */
#line 642 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy301, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::MINUS); }
#line 4152 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 75: /* term_strong ::= constant PLUS term */
#line 643 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy301, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::PLUS); }
#line 4157 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 76: /* term_strong ::= constant STAR term */
#line 644 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy301, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::TIMES); }
#line 4162 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 77: /* term_strong ::= constant INT_DIV term */
#line 645 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy301, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::DIVIDE); }
#line 4167 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 78: /* term_strong ::= constant MOD term */
#line 646 "bcplus/parser/detail/lemon_parser.y"
{ BINARY_ARITH(yygotominor.yy223, yymsp[-2].minor.yy301, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BinaryTerm::Operator::MOD); }
#line 4172 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 91: /* term_no_const_strong ::= constant */
#line 668 "bcplus/parser/detail/lemon_parser.y"
{
		// error handling for constants so they don'yygotominor.yy223 default to undeclared identifiers
		yygotominor.yy223 = NULL;
		ref_ptr<const Referenced> c_ptr = yymsp[0].minor.yy301;
		parser->_parse_error("Encountered unexpected constant symbol.", &yymsp[0].minor.yy301->beginLoc());
		YYERROR;
	}
#line 4183 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 108: /* term_no_const ::= constant */
#line 701 "bcplus/parser/detail/lemon_parser.y"
{
		// error handline for constants so they don'yygotominor.yy223 default to undeclared identifiers
		yygotominor.yy223 = NULL;
		ref_ptr<const Referenced> c_ptr = yymsp[0].minor.yy301;
		parser->_parse_error("Encountered unexpected constant symbol.", &yymsp[0].minor.yy301->beginLoc());
		YYERROR;
	}
#line 4194 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 130: /* num_range ::= term_integral DBL_PERIOD term_integral */
#line 758 "bcplus/parser/detail/lemon_parser.y"
{
	ref_ptr<const Referenced> l_ptr = yymsp[-2].minor.yy223, r_ptr = yymsp[0].minor.yy223, s_ptr = yymsp[-1].minor.yy0;
	yygotominor.yy513 = NULL;

	if (yymsp[-2].minor.yy223->domainType() != DomainType::INTEGRAL) {
		parser->_parse_error("Number ranges cannot have non-numeric operands.", &yymsp[-1].minor.yy0->beginLoc());
		YYERROR;
	}
	
	if (yymsp[0].minor.yy223->domainType() != DomainType::INTEGRAL) {
		parser->_parse_error("Number ranges cannot have non-numeric operands.", &yymsp[0].minor.yy223->beginLoc());
		YYERROR;
	}

	yygotominor.yy513 = new NumberRange(yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());
}
#line 4214 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 131: /* num_range_eval ::= term_int_eval DBL_PERIOD term_int_eval */
#line 776 "bcplus/parser/detail/lemon_parser.y"
{
	ref_ptr<const Referenced> l_ptr = yymsp[-2].minor.yy248, r_ptr = yymsp[0].minor.yy248, s_ptr = yymsp[-1].minor.yy0;
	yygotominor.yy97 = new NumberRangeEval(yymsp[-2].minor.yy248->val(), yymsp[0].minor.yy248->val(), yymsp[-2].minor.yy248->beginLoc(), yymsp[0].minor.yy248->endLoc());
}
#line 4222 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 132: /* term_int_eval ::= INTEGER */
#line 782 "bcplus/parser/detail/lemon_parser.y"
{
	ref_ptr<const Referenced> i_ptr = yymsp[0].minor.yy0;

	yygotominor.yy248 = 0;
	try {
		yygotominor.yy248 = new Number(boost::lexical_cast<int>(*yymsp[0].minor.yy0->str()), yymsp[0].minor.yy0->beginLoc());
	} catch (boost::bad_lexical_cast const& e) {
	parser->_parse_error("INTERNAL ERROR: Failed to parse integer \"" + *yymsp[0].minor.yy0->str() + "\".", &yymsp[0].minor.yy0->beginLoc());
		YYERROR;
	}
}
#line 4237 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 133: /* term_int_eval ::= PAREN_L term_int_eval PAREN_R */
#line 794 "bcplus/parser/detail/lemon_parser.y"
{
	ref_ptr<const Referenced> pl_ptr = yymsp[-2].minor.yy0, pr_ptr = yymsp[0].minor.yy0;
	yygotominor.yy248 = yymsp[-1].minor.yy248;
	yygotominor.yy248->beginLoc(yymsp[-2].minor.yy0->beginLoc());
	yygotominor.yy248->endLoc(yymsp[0].minor.yy0->endLoc());
}
#line 4247 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 134: /* term_int_eval ::= DASH term_int_eval */
#line 814 "bcplus/parser/detail/lemon_parser.y"
{ NUM_UOP(yygotominor.yy248, yymsp[0].minor.yy248, -1 * yymsp[0].minor.yy248->val());   yy_destructor(yypParser,107,&yymsp[-1].minor);
}
#line 4253 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 135: /* term_int_eval ::= ABS term_int_eval */
#line 815 "bcplus/parser/detail/lemon_parser.y"
{ NUM_UOP(yygotominor.yy248, yymsp[0].minor.yy248, yymsp[0].minor.yy248->val() < 0 ? - yymsp[0].minor.yy248->val() : yymsp[0].minor.yy248->val());   yy_destructor(yypParser,112,&yymsp[-1].minor);
}
#line 4259 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 136: /* term_int_eval ::= term_int_eval DASH term_int_eval */
#line 817 "bcplus/parser/detail/lemon_parser.y"
{ NUM_BOP(yygotominor.yy248, yymsp[-2].minor.yy248, yymsp[0].minor.yy248, yymsp[-2].minor.yy248->val() - yymsp[0].minor.yy248->val());   yy_destructor(yypParser,107,&yymsp[-1].minor);
}
#line 4265 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 137: /* term_int_eval ::= term_int_eval PLUS term_int_eval */
#line 818 "bcplus/parser/detail/lemon_parser.y"
{ NUM_BOP(yygotominor.yy248, yymsp[-2].minor.yy248, yymsp[0].minor.yy248, yymsp[-2].minor.yy248->val() + yymsp[0].minor.yy248->val());   yy_destructor(yypParser,108,&yymsp[-1].minor);
}
#line 4271 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 138: /* term_int_eval ::= term_int_eval STAR term_int_eval */
#line 819 "bcplus/parser/detail/lemon_parser.y"
{ NUM_BOP(yygotominor.yy248, yymsp[-2].minor.yy248, yymsp[0].minor.yy248, yymsp[-2].minor.yy248->val() * yymsp[0].minor.yy248->val());   yy_destructor(yypParser,109,&yymsp[-1].minor);
}
#line 4277 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 139: /* term_int_eval ::= term_int_eval INT_DIV term_int_eval */
#line 820 "bcplus/parser/detail/lemon_parser.y"
{ NUM_BOP(yygotominor.yy248, yymsp[-2].minor.yy248, yymsp[0].minor.yy248, yymsp[-2].minor.yy248->val() / yymsp[0].minor.yy248->val());   yy_destructor(yypParser,110,&yymsp[-1].minor);
}
#line 4283 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 140: /* term_int_eval ::= term_int_eval MOD term_int_eval */
#line 821 "bcplus/parser/detail/lemon_parser.y"
{ NUM_BOP(yygotominor.yy248, yymsp[-2].minor.yy248, yymsp[0].minor.yy248, yymsp[-2].minor.yy248->val() % yymsp[0].minor.yy248->val());   yy_destructor(yypParser,111,&yymsp[-1].minor);
}
#line 4289 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 141: /* formula ::= formula_base */
      case 184: /* formula_no_const ::= formula_no_const_base */ yytestcase(yyruleno==184);
      case 256: /* formula_temporal ::= formula_temporal_base */ yytestcase(yyruleno==256);
#line 881 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = yymsp[0].minor.yy201;				}
#line 4296 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 142: /* formula ::= PAREN_L formula PAREN_R */
      case 185: /* formula_no_const ::= PAREN_L formula_no_const PAREN_R */ yytestcase(yyruleno==185);
      case 257: /* formula_temporal ::= PAREN_L formula_temporal PAREN_R */ yytestcase(yyruleno==257);
#line 882 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = yymsp[-1].minor.yy201; yygotominor.yy201->parens(true); 	  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 4305 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 143: /* formula ::= NOT formula */
      case 186: /* formula_no_const ::= NOT formula_no_const */ yytestcase(yyruleno==186);
      case 258: /* formula_temporal ::= NOT formula_temporal */ yytestcase(yyruleno==258);
#line 883 "bcplus/parser/detail/lemon_parser.y"
{ NESTED_UOP(yygotominor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, UnaryFormula::Operator::NOT, Language::Feature::FORMULA_NOT_KEYWORD); }
#line 4312 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 144: /* formula ::= DASH formula */
      case 187: /* formula_no_const ::= DASH formula_no_const */ yytestcase(yyruleno==187);
      case 259: /* formula_temporal ::= DASH formula_temporal */ yytestcase(yyruleno==259);
#line 884 "bcplus/parser/detail/lemon_parser.y"
{ NESTED_UOP(yygotominor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, UnaryFormula::Operator::NOT, Language::Feature::FORMULA_NOT_DASH); }
#line 4319 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 145: /* formula ::= formula AMP formula */
      case 188: /* formula_no_const ::= formula_no_const AMP formula_no_const */ yytestcase(yyruleno==188);
      case 260: /* formula_temporal ::= formula_temporal AMP formula_temporal */ yytestcase(yyruleno==260);
#line 885 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new BinaryFormula(BinaryFormula::Operator::AND, yymsp[-2].minor.yy201, yymsp[0].minor.yy201, yymsp[-2].minor.yy201->beginLoc(), yymsp[0].minor.yy201->endLoc());   yy_destructor(yypParser,103,&yymsp[-1].minor);
}
#line 4327 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 146: /* formula ::= formula DBL_PLUS formula */
      case 147: /* formula ::= formula PIPE formula */ yytestcase(yyruleno==147);
      case 189: /* formula_no_const ::= formula_no_const DBL_PLUS formula_no_const */ yytestcase(yyruleno==189);
      case 190: /* formula_no_const ::= formula_no_const PIPE formula_no_const */ yytestcase(yyruleno==190);
      case 261: /* formula_temporal ::= formula_temporal DBL_PLUS formula_temporal */ yytestcase(yyruleno==261);
      case 262: /* formula_temporal ::= formula_temporal PIPE formula_temporal */ yytestcase(yyruleno==262);
#line 887 "bcplus/parser/detail/lemon_parser.y"
{ NESTED_BOP(yygotominor.yy201, yymsp[-2].minor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, BinaryFormula::Operator::OR); }
#line 4337 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 148: /* formula ::= formula EQUIV formula */
      case 191: /* formula_no_const ::= formula_no_const EQUIV formula_no_const */ yytestcase(yyruleno==191);
      case 263: /* formula_temporal ::= formula_temporal EQUIV formula_temporal */ yytestcase(yyruleno==263);
#line 889 "bcplus/parser/detail/lemon_parser.y"
{ NESTED_BOP(yygotominor.yy201, yymsp[-2].minor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, BinaryFormula::Operator::EQUIV); }
#line 4344 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 149: /* formula ::= formula IMPL formula */
      case 150: /* formula ::= formula ARROW_RDASH formula */ yytestcase(yyruleno==150);
      case 192: /* formula_no_const ::= formula_no_const IMPL formula_no_const */ yytestcase(yyruleno==192);
      case 193: /* formula_no_const ::= formula_no_const ARROW_RDASH formula_no_const */ yytestcase(yyruleno==193);
      case 264: /* formula_temporal ::= formula_temporal IMPL formula_temporal */ yytestcase(yyruleno==264);
      case 265: /* formula_temporal ::= formula_temporal ARROW_RDASH formula_temporal */ yytestcase(yyruleno==265);
#line 890 "bcplus/parser/detail/lemon_parser.y"
{ NESTED_BOP(yygotominor.yy201, yymsp[-2].minor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, BinaryFormula::Operator::IMPL); }
#line 4354 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 151: /* formula_base ::= comparison */
      case 194: /* formula_no_const_base ::= comparison_no_const */ yytestcase(yyruleno==194);
      case 267: /* formula_temporal_base ::= comparison_temporal */ yytestcase(yyruleno==267);
      case 291: /* head_formula ::= comparison */ yytestcase(yyruleno==291);
#line 893 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = yymsp[0].minor.yy201; }
#line 4362 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 152: /* formula_base ::= atomic_formula */
      case 292: /* head_formula ::= atomic_head_formula */ yytestcase(yyruleno==292);
#line 894 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = yymsp[0].minor.yy371; }
#line 4368 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 153: /* formula_base ::= formula_quant */
      case 269: /* formula_temporal_base ::= formula_temporal_quant */ yytestcase(yyruleno==269);
#line 895 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = yymsp[0].minor.yy77; }
#line 4374 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 154: /* formula_base ::= formula_card */
      case 270: /* formula_temporal_base ::= formula_temporal_card */ yytestcase(yyruleno==270);
#line 897 "bcplus/parser/detail/lemon_parser.y"
{ 
		yygotominor.yy201 = yymsp[0].minor.yy201;
		if (!parser->lang()->support(Language::Feature::FORMULA_CARDINALITY_BODY)) {
			parser->_feature_error(Language::Feature::FORMULA_CARDINALITY_BODY, &yymsp[0].minor.yy201->beginLoc());
			YYERROR;
		}
	}
#line 4386 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 155: /* formula_base ::= TRUE */
      case 195: /* formula_no_const_base ::= TRUE */ yytestcase(yyruleno==195);
      case 271: /* formula_temporal_base ::= TRUE */ yytestcase(yyruleno==271);
      case 294: /* head_formula ::= TRUE */ yytestcase(yyruleno==294);
#line 904 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new NullaryFormula(NullaryFormula::Operator::TRUE, yymsp[0].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc()); }
#line 4394 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 156: /* formula_base ::= FALSE */
      case 196: /* formula_no_const_base ::= FALSE */ yytestcase(yyruleno==196);
      case 272: /* formula_temporal_base ::= FALSE */ yytestcase(yyruleno==272);
      case 295: /* head_formula ::= FALSE */ yytestcase(yyruleno==295);
#line 905 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new NullaryFormula(NullaryFormula::Operator::FALSE, yymsp[0].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc()); }
#line 4402 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 157: /* comparison ::= term_strong EQ term */
      case 164: /* comparison ::= term_strong_candidate EQ term */ yytestcase(yyruleno==164);
      case 197: /* comparison_no_const ::= term_no_const_strong EQ term_no_const */ yytestcase(yyruleno==197);
      case 273: /* comparison_temporal ::= term_temporal_strong EQ term_temporal */ yytestcase(yyruleno==273);
#line 907 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::EQ, yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,83,&yymsp[-1].minor);
}
#line 4411 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 158: /* comparison ::= term_strong DBL_EQ term */
      case 165: /* comparison ::= term_strong_candidate DBL_EQ term */ yytestcase(yyruleno==165);
      case 198: /* comparison_no_const ::= term_no_const_strong DBL_EQ term_no_const */ yytestcase(yyruleno==198);
      case 274: /* comparison_temporal ::= term_temporal_strong DBL_EQ term_temporal */ yytestcase(yyruleno==274);
#line 908 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::EQ, yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,84,&yymsp[-1].minor);
}
#line 4420 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 159: /* comparison ::= term_strong NEQ term */
      case 166: /* comparison ::= term_strong_candidate NEQ term */ yytestcase(yyruleno==166);
      case 199: /* comparison_no_const ::= term_no_const_strong NEQ term_no_const */ yytestcase(yyruleno==199);
      case 275: /* comparison_temporal ::= term_temporal_strong NEQ term_temporal */ yytestcase(yyruleno==275);
#line 909 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::NEQ, yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,85,&yymsp[-1].minor);
}
#line 4429 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 160: /* comparison ::= term_strong LTHAN term */
      case 167: /* comparison ::= term_strong_candidate LTHAN term */ yytestcase(yyruleno==167);
      case 200: /* comparison_no_const ::= term_no_const_strong LTHAN term_no_const */ yytestcase(yyruleno==200);
      case 276: /* comparison_temporal ::= term_temporal_strong LTHAN term_temporal */ yytestcase(yyruleno==276);
#line 910 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::LTHAN, yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,87,&yymsp[-1].minor);
}
#line 4438 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 161: /* comparison ::= term_strong GTHAN term */
      case 168: /* comparison ::= term_strong_candidate GTHAN term */ yytestcase(yyruleno==168);
      case 201: /* comparison_no_const ::= term_no_const_strong GTHAN term_no_const */ yytestcase(yyruleno==201);
      case 277: /* comparison_temporal ::= term_temporal_strong GTHAN term_temporal */ yytestcase(yyruleno==277);
#line 911 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::GTHAN, yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,88,&yymsp[-1].minor);
}
#line 4447 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 162: /* comparison ::= term_strong LTHAN_EQ term */
      case 169: /* comparison ::= term_strong_candidate LTHAN_EQ term */ yytestcase(yyruleno==169);
      case 202: /* comparison_no_const ::= term_no_const_strong LTHAN_EQ term_no_const */ yytestcase(yyruleno==202);
      case 278: /* comparison_temporal ::= term_temporal_strong LTHAN_EQ term_temporal */ yytestcase(yyruleno==278);
#line 912 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::LTHAN_EQ, yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,89,&yymsp[-1].minor);
}
#line 4456 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 163: /* comparison ::= term_strong GTHAN_EQ term */
      case 170: /* comparison ::= term_strong_candidate GTHAN_EQ term */ yytestcase(yyruleno==170);
      case 203: /* comparison_no_const ::= term_no_const_strong GTHAN_EQ term_no_const */ yytestcase(yyruleno==203);
      case 279: /* comparison_temporal ::= term_temporal_strong GTHAN_EQ term_temporal */ yytestcase(yyruleno==279);
#line 913 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::GTHAN_EQ, yymsp[-2].minor.yy223, yymsp[0].minor.yy223, yymsp[-2].minor.yy223->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,90,&yymsp[-1].minor);
}
#line 4465 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 171: /* comparison ::= constant DBL_EQ term */
#line 921 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::EQ, yymsp[-2].minor.yy301, yymsp[0].minor.yy223, yymsp[-2].minor.yy301->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,84,&yymsp[-1].minor);
}
#line 4471 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 172: /* comparison ::= constant NEQ term */
#line 922 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::NEQ, yymsp[-2].minor.yy301, yymsp[0].minor.yy223, yymsp[-2].minor.yy301->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,85,&yymsp[-1].minor);
}
#line 4477 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 173: /* comparison ::= constant LTHAN term */
#line 923 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::LTHAN, yymsp[-2].minor.yy301, yymsp[0].minor.yy223, yymsp[-2].minor.yy301->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,87,&yymsp[-1].minor);
}
#line 4483 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 174: /* comparison ::= constant GTHAN term */
#line 924 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::GTHAN, yymsp[-2].minor.yy301, yymsp[0].minor.yy223, yymsp[-2].minor.yy301->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,88,&yymsp[-1].minor);
}
#line 4489 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 175: /* comparison ::= constant LTHAN_EQ term */
#line 925 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::LTHAN_EQ, yymsp[-2].minor.yy301, yymsp[0].minor.yy223, yymsp[-2].minor.yy301->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,89,&yymsp[-1].minor);
}
#line 4495 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 176: /* comparison ::= constant GTHAN_EQ term */
#line 926 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = new ComparisonFormula(ComparisonFormula::Operator::GTHAN_EQ, yymsp[-2].minor.yy301, yymsp[0].minor.yy223, yymsp[-2].minor.yy301->beginLoc(), yymsp[0].minor.yy223->endLoc());   yy_destructor(yypParser,90,&yymsp[-1].minor);
}
#line 4501 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 177: /* atomic_formula ::= constant */
      case 181: /* atomic_formula_anon ::= const_anon */ yytestcase(yyruleno==181);
      case 204: /* atomic_formula_one_const ::= constant_one_const */ yytestcase(yyruleno==204);
#line 956 "bcplus/parser/detail/lemon_parser.y"
{ ATOMIC_FORMULA(yygotominor.yy371, yymsp[0].minor.yy301, true); }
#line 4508 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 178: /* atomic_formula ::= TILDE constant */
      case 182: /* atomic_formula_anon ::= TILDE const_anon */ yytestcase(yyruleno==182);
      case 205: /* atomic_formula_one_const ::= TILDE constant_one_const */ yytestcase(yyruleno==205);
#line 957 "bcplus/parser/detail/lemon_parser.y"
{ ATOMIC_FORMULA(yygotominor.yy371, yymsp[0].minor.yy301, false);   yy_destructor(yypParser,77,&yymsp[-1].minor);
}
#line 4516 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 179: /* atomic_formula ::= constant EQ term */
      case 183: /* atomic_formula_anon ::= const_anon EQ term */ yytestcase(yyruleno==183);
      case 206: /* atomic_formula_one_const ::= constant_one_const EQ term_no_const */ yytestcase(yyruleno==206);
#line 958 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy371 = new AtomicFormula(yymsp[-2].minor.yy301, yymsp[0].minor.yy223, yymsp[-2].minor.yy301->beginLoc(), yymsp[0].minor.yy223->endLoc());	  yy_destructor(yypParser,83,&yymsp[-1].minor);
}
#line 4524 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 180: /* atomic_formula_anon ::= atomic_formula */
      case 296: /* atomic_head_formula ::= atomic_formula */ yytestcase(yyruleno==296);
      case 384: /* show_elem ::= atomic_formula_one_const */ yytestcase(yyruleno==384);
#line 960 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy371 = yymsp[0].minor.yy371; }
#line 4531 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 207: /* formula_quant ::= BRACKET_L quant_lst PIPE formula BRACKET_R */
      case 280: /* formula_temporal_quant ::= BRACKET_L quant_lst PIPE formula_temporal BRACKET_R */ yytestcase(yyruleno==280);
#line 1025 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy77=NULL;
		ref_ptr<const Token> bl_ptr = yymsp[-4].minor.yy0;
		ref_ptr<QuantifierFormula::QuantifierList> lst_ptr = yymsp[-3].minor.yy449;
		ref_ptr<Formula> sub_ptr = yymsp[-1].minor.yy201;
		ref_ptr<const Token> br_ptr = yymsp[0].minor.yy0;

		if (!parser->lang()->support(Language::Feature::FORMULA_QUANTIFIER)) {
			parser->_feature_error(Language::Feature::FORMULA_QUANTIFIER, &yymsp[-4].minor.yy0->beginLoc());
			YYERROR;
		} else yygotominor.yy77 = new QuantifierFormula(yymsp[-3].minor.yy449, yymsp[-1].minor.yy201, yymsp[-4].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
	  yy_destructor(yypParser,100,&yymsp[-2].minor);
}
#line 4549 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 208: /* quant_lst ::= quant_op variable */
#line 1039 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy449 = new QuantifierFormula::QuantifierList();
		yygotominor.yy449->push_back(QuantifierFormula::Quantifier(yymsp[-1].minor.yy514, yymsp[0].minor.yy1));
	}
#line 4557 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 209: /* quant_lst ::= quant_lst quant_op variable */
#line 1045 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy449 = yymsp[-2].minor.yy449;
		yygotominor.yy449->push_back(QuantifierFormula::Quantifier(yymsp[-1].minor.yy514, yymsp[0].minor.yy1));
	}
#line 4565 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 210: /* quant_op ::= BIG_CONJ */
#line 1050 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy514 = QuantifierFormula::Operator::CONJ;   yy_destructor(yypParser,92,&yymsp[0].minor);
}
#line 4571 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 211: /* quant_op ::= BIG_DISJ */
#line 1051 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy514 = QuantifierFormula::Operator::DISJ;   yy_destructor(yypParser,93,&yymsp[0].minor);
}
#line 4577 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 212: /* formula_card ::= CBRACKET_L card_var_lst formula CBRACKET_R */
      case 281: /* formula_temporal_card ::= CBRACKET_L card_var_lst formula_temporal CBRACKET_R */ yytestcase(yyruleno==281);
#line 1097 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, NULL, yymsp[-3].minor.yy0, yymsp[-2].minor.yy187, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, NULL);  }
#line 4583 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 213: /* formula_card ::= term_strong CBRACKET_L card_var_lst formula CBRACKET_R */
      case 282: /* formula_temporal_card ::= term_temporal_strong CBRACKET_L card_var_lst formula_temporal CBRACKET_R */ yytestcase(yyruleno==282);
#line 1098 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, yymsp[-4].minor.yy223, yymsp[-3].minor.yy0, yymsp[-2].minor.yy187, yymsp[-1].minor.yy201,  yymsp[0].minor.yy0, NULL);  }
#line 4589 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 214: /* formula_card ::= CBRACKET_L card_var_lst formula CBRACKET_R term */
      case 283: /* formula_temporal_card ::= CBRACKET_L card_var_lst formula_temporal CBRACKET_R term_temporal */ yytestcase(yyruleno==283);
#line 1099 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, NULL, yymsp[-4].minor.yy0, yymsp[-3].minor.yy187, yymsp[-2].minor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4595 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 215: /* formula_card ::= term_strong CBRACKET_L card_var_lst formula CBRACKET_R term */
      case 284: /* formula_temporal_card ::= term_temporal_strong CBRACKET_L card_var_lst formula_temporal CBRACKET_R term_temporal */ yytestcase(yyruleno==284);
#line 1100 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, yymsp[-5].minor.yy223, yymsp[-4].minor.yy0, yymsp[-3].minor.yy187, yymsp[-2].minor.yy201,  yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4601 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 216: /* formula_card ::= CBRACKET_L formula CBRACKET_R */
      case 285: /* formula_temporal_card ::= CBRACKET_L formula_temporal CBRACKET_R */ yytestcase(yyruleno==285);
#line 1101 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, NULL, yymsp[-2].minor.yy0, NULL, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, NULL);  }
#line 4607 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 217: /* formula_card ::= term_strong CBRACKET_L formula CBRACKET_R */
      case 286: /* formula_temporal_card ::= term_temporal_strong CBRACKET_L formula_temporal CBRACKET_R */ yytestcase(yyruleno==286);
#line 1102 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, yymsp[-3].minor.yy223, yymsp[-2].minor.yy0, NULL, yymsp[-1].minor.yy201,  yymsp[0].minor.yy0, NULL);  }
#line 4613 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 218: /* formula_card ::= CBRACKET_L formula CBRACKET_R term */
      case 287: /* formula_temporal_card ::= CBRACKET_L formula_temporal CBRACKET_R term_temporal */ yytestcase(yyruleno==287);
#line 1103 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, NULL, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4619 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 219: /* formula_card ::= term_strong CBRACKET_L formula CBRACKET_R term */
      case 288: /* formula_temporal_card ::= term_temporal_strong CBRACKET_L formula_temporal CBRACKET_R term_temporal */ yytestcase(yyruleno==288);
#line 1104 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy201, yymsp[-4].minor.yy223, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy201,  yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4625 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 220: /* card_var_lst ::= card_var_lst_inner PIPE */
#line 1108 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy187 = yymsp[-1].minor.yy187;
	  yy_destructor(yypParser,100,&yymsp[0].minor);
}
#line 4633 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 221: /* card_var_lst_inner ::= variable */
#line 1113 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Referenced> v_ptr = yymsp[0].minor.yy1;
		yygotominor.yy187 = new CardinalityFormula::VariableList();
		yygotominor.yy187->push_back(yymsp[0].minor.yy1->symbol());
	}
#line 4642 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 222: /* card_var_lst_inner ::= card_var_lst_inner COMMA variable */
#line 1120 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Referenced> v_ptr = yymsp[0].minor.yy1;
		yygotominor.yy187 = yymsp[-2].minor.yy187;
		yygotominor.yy187->push_back(yymsp[0].minor.yy1->symbol());
	  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 4652 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 232: /* term_temporal ::= constant */
#line 1174 "bcplus/parser/detail/lemon_parser.y"
{
		// error handline for constants so they don'yygotominor.yy223 default to undeclared identifiers
		yygotominor.yy223 = NULL;
		ref_ptr<const Referenced> c_ptr = yymsp[0].minor.yy301;
		parser->_parse_error("All constant symbols must be bound to a step using the i:F notation.", &yymsp[0].minor.yy301->beginLoc());
		YYERROR;
	}
#line 4663 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 235: /* term_temporal ::= term_temporal COLON term */
      case 248: /* term_temporal_strong ::= term_temporal_strong COLON term_strong */ yytestcase(yyruleno==248);
#line 1186 "bcplus/parser/detail/lemon_parser.y"
{ BINDING(yygotominor.yy223, yymsp[-2].minor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy223, BindingTerm); }
#line 4669 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 266: /* formula_temporal ::= term_temporal_strong COLON formula */
#line 1268 "bcplus/parser/detail/lemon_parser.y"
{ BINDING(yygotominor.yy201, yymsp[-2].minor.yy223, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, BindingFormula); }
#line 4674 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 268: /* formula_temporal_base ::= atomic_formula */
#line 1274 "bcplus/parser/detail/lemon_parser.y"
{
		// error handline for more useful error messages
		yygotominor.yy201 = NULL;
		ref_ptr<const Referenced> l_ptr = yymsp[0].minor.yy371;
		parser->_parse_error("All constant symbols must be bound to a step using the i:F notation.", &yymsp[0].minor.yy371->beginLoc());
		YYERROR;
	}
#line 4685 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 289: /* head_formula ::= head_formula AMP head_formula */
#line 1369 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy201 = new BinaryFormula(BinaryFormula::Operator::AND, yymsp[-2].minor.yy201, yymsp[0].minor.yy201, yymsp[-2].minor.yy201->beginLoc(), yymsp[0].minor.yy201->endLoc());
	  yy_destructor(yypParser,103,&yymsp[-1].minor);
}
#line 4693 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 290: /* head_formula ::= PAREN_L head_formula PAREN_R */
#line 1377 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Referenced> lp_ptr = yymsp[-2].minor.yy0, rp_ptr = yymsp[0].minor.yy0;
		yygotominor.yy201 = yymsp[-1].minor.yy201;
		yygotominor.yy201->parens(true);																									\
		yygotominor.yy201->beginLoc(yymsp[-2].minor.yy0->beginLoc());																					\
		yygotominor.yy201->endLoc(yymsp[0].minor.yy0->endLoc());
		
	}
#line 4705 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 293: /* head_formula ::= formula_smpl_card */
#line 1388 "bcplus/parser/detail/lemon_parser.y"
{ 
		yygotominor.yy201 = yymsp[0].minor.yy81;
		if (!parser->lang()->support(Language::Feature::FORMULA_CARDINALITY_HEAD)) {
			parser->_feature_error(Language::Feature::FORMULA_CARDINALITY_HEAD, &yymsp[0].minor.yy81->beginLoc());
			YYERROR;
		}
	}
#line 4716 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 297: /* atomic_head_formula ::= DASH constant */
#line 1401 "bcplus/parser/detail/lemon_parser.y"
{ 
		yygotominor.yy371 = NULL;
		ref_ptr<const Token> d_ptr = yymsp[-1].minor.yy0;
		ref_ptr<Constant> c_ptr = yymsp[0].minor.yy301;

		if (!parser->lang()->support(Language::Feature::FORMULA_NOT_DASH_HEAD)) {
			parser->_feature_error(Language::Feature::FORMULA_NOT_DASH_HEAD);
			YYERROR;
		} else {
			ATOMIC_FORMULA(yygotominor.yy371, yymsp[0].minor.yy301, false); 
		}
	}
#line 4732 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 298: /* formula_smpl_card ::= CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R */
#line 1414 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, NULL, yymsp[-3].minor.yy0, yymsp[-2].minor.yy187, yymsp[-1].minor.yy371, yymsp[0].minor.yy0, NULL);  }
#line 4737 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 299: /* formula_smpl_card ::= term_strong CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R */
#line 1415 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, yymsp[-4].minor.yy223, yymsp[-3].minor.yy0, yymsp[-2].minor.yy187, yymsp[-1].minor.yy371,  yymsp[0].minor.yy0, NULL);  }
#line 4742 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 300: /* formula_smpl_card ::= CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R term */
#line 1416 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, NULL, yymsp[-4].minor.yy0, yymsp[-3].minor.yy187, yymsp[-2].minor.yy371, yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4747 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 301: /* formula_smpl_card ::= term_strong CBRACKET_L card_var_lst atomic_formula_one_const CBRACKET_R term */
#line 1417 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, yymsp[-5].minor.yy223, yymsp[-4].minor.yy0, yymsp[-3].minor.yy187, yymsp[-2].minor.yy371,  yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4752 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 302: /* formula_smpl_card ::= CBRACKET_L atomic_formula_one_const CBRACKET_R */
#line 1418 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, NULL, yymsp[-2].minor.yy0, NULL, yymsp[-1].minor.yy371, yymsp[0].minor.yy0, NULL);  }
#line 4757 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 303: /* formula_smpl_card ::= term_strong CBRACKET_L atomic_formula_one_const CBRACKET_R */
#line 1419 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, yymsp[-3].minor.yy223, yymsp[-2].minor.yy0, NULL, yymsp[-1].minor.yy371,  yymsp[0].minor.yy0, NULL);  }
#line 4762 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 304: /* formula_smpl_card ::= CBRACKET_L atomic_formula_one_const CBRACKET_R term */
#line 1420 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, NULL, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy371, yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4767 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 305: /* formula_smpl_card ::= term_strong CBRACKET_L atomic_formula_one_const CBRACKET_R term */
#line 1421 "bcplus/parser/detail/lemon_parser.y"
{ CARD_FORMULA(yygotominor.yy81, yymsp[-4].minor.yy223, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy371,  yymsp[-1].minor.yy0, yymsp[0].minor.yy223); 	}
#line 4772 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 306: /* stmt_macro_def ::= COLON_DASH MACROS macro_def_lst PERIOD */
#line 1440 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy39 = NULL;
        ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0;
        ref_ptr<const Token> kw_ptr = yymsp[-2].minor.yy0;
        ref_ptr<MacroDeclaration::ElementList> l_ptr = yymsp[-1].minor.yy458;
        ref_ptr<const Token> p_ptr = yymsp[0].minor.yy0;

        if (!parser->lang()->support(Language::Feature::DECL_MACRO)) {
            parser->_feature_error(Language::Feature::DECL_MACRO, &yymsp[-2].minor.yy0->beginLoc());
            YYERROR;
        } else {
		    BOOST_FOREACH(symbols::MacroSymbol* m, *yymsp[-1].minor.yy458) {
			    if (!parser->symtab()->create(m)) {
	    	        // Check if it's a duplicate
	    	        symbols::MacroSymbol* m2 = (symbols::MacroSymbol*)parser->symtab()->resolve(symbols::Symbol::Type::MACRO, *m->base(), m->arity());
		            if (!m2 || m2 != m) {
		                parser->_parse_error("Detected conflicting definition of symbol \"" + *m->name() + "\".", &yygotominor.yy39->beginLoc());
		            } else {
		                parser->_parse_error("Detected a duplicate definition of symbol \"" + *m->name() + "\".", &yygotominor.yy39->beginLoc());
		            }
		        }
		    }

			yygotominor.yy39 = new MacroDeclaration(yymsp[-1].minor.yy458, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
        }
    }
#line 4802 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 307: /* macro_def_lst ::= macro_bnd */
#line 1468 "bcplus/parser/detail/lemon_parser.y"
{
        yygotominor.yy458 = new MacroDeclaration::ElementList();
        yygotominor.yy458->push_back(yymsp[0].minor.yy427);
    }
#line 4810 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 308: /* macro_def_lst ::= macro_def_lst SEMICOLON macro_bnd */
#line 1474 "bcplus/parser/detail/lemon_parser.y"
{
        yygotominor.yy458 = yymsp[-2].minor.yy458;
        yygotominor.yy458->push_back(yymsp[0].minor.yy427);
      yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 4819 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 309: /* macro_bnd ::= IDENTIFIER PAREN_L macro_args PAREN_R ARROW_RDASH MACRO_STRING */
#line 1480 "bcplus/parser/detail/lemon_parser.y"
{
        ref_ptr<const Token> id_ptr = yymsp[-5].minor.yy0;
        ref_ptr<MacroSymbol::ArgumentList> args_ptr = yymsp[-3].minor.yy374;
        ref_ptr<const Token> def_ptr = yymsp[0].minor.yy0;

        yygotominor.yy427 = new MacroSymbol(yymsp[-5].minor.yy0->str(), yymsp[0].minor.yy0->str(), yymsp[-3].minor.yy374);
      yy_destructor(yypParser,73,&yymsp[-4].minor);
  yy_destructor(yypParser,74,&yymsp[-2].minor);
  yy_destructor(yypParser,98,&yymsp[-1].minor);
}
#line 4833 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 310: /* macro_bnd ::= IDENTIFIER ARROW_RDASH MACRO_STRING */
#line 1489 "bcplus/parser/detail/lemon_parser.y"
{
        ref_ptr<const Token> id_ptr = yymsp[-2].minor.yy0;
        ref_ptr<const Token> def_ptr = yymsp[0].minor.yy0;

        yygotominor.yy427 = new MacroSymbol(yymsp[-2].minor.yy0->str(), yymsp[0].minor.yy0->str());
      yy_destructor(yypParser,98,&yymsp[-1].minor);
}
#line 4844 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 311: /* macro_args ::= macro_arg */
#line 1497 "bcplus/parser/detail/lemon_parser.y"
{
        yygotominor.yy374 = new MacroSymbol::ArgumentList();
        yygotominor.yy374->push_back(yymsp[0].minor.yy167->str());
        delete yymsp[0].minor.yy167;
    }
#line 4853 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 312: /* macro_args ::= macro_args COMMA macro_arg */
#line 1503 "bcplus/parser/detail/lemon_parser.y"
{
        yygotominor.yy374 = yymsp[-2].minor.yy374;
        yygotominor.yy374->push_back(yymsp[0].minor.yy167->str());
        delete yymsp[0].minor.yy167;
      yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 4863 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 313: /* macro_arg ::= POUND_INTEGER */
      case 314: /* macro_arg ::= POUND_IDENTIFIER */ yytestcase(yyruleno==314);
#line 1510 "bcplus/parser/detail/lemon_parser.y"
{
        yygotominor.yy167 = yymsp[0].minor.yy0;
    }
#line 4871 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 315: /* sort_lst ::= sort */
#line 1537 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy255 = new ConstantSymbol::SortList();
		yygotominor.yy255->push_back(yymsp[0].minor.yy229);
	}
#line 4879 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 316: /* sort_lst ::= sort_lst COMMA sort */
#line 1542 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy255 = yymsp[-2].minor.yy255;
		yygotominor.yy255->push_back(yymsp[0].minor.yy229);
	  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 4888 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 317: /* sort ::= sort_id_nr */
      case 323: /* sort_id_nr ::= sort_id */ yytestcase(yyruleno==323);
      case 324: /* sort_id_nr ::= sort_nr */ yytestcase(yyruleno==324);
#line 1567 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy229 = yymsp[0].minor.yy229; }
#line 4895 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 318: /* sort ::= sort_id_nr STAR */
#line 1568 "bcplus/parser/detail/lemon_parser.y"
{ DYNAMIC_SORT_PLUS(yygotominor.yy229, yymsp[-1].minor.yy229, yymsp[0].minor.yy0, Language::Feature::STAR_SORT, parser->symtab()->bobj(SymbolTable::BuiltinObject::NONE)); }
#line 4900 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 319: /* sort ::= sort_id_nr CARROT */
#line 1569 "bcplus/parser/detail/lemon_parser.y"
{ DYNAMIC_SORT_PLUS(yygotominor.yy229, yymsp[-1].minor.yy229, yymsp[0].minor.yy0, Language::Feature::CARROT_SORT, parser->symtab()->bobj(SymbolTable::BuiltinObject::UNKNOWN)); }
#line 4905 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 320: /* sort ::= sort PLUS object_nullary */
#line 1571 "bcplus/parser/detail/lemon_parser.y"
{ u::ref_ptr<const Object> o_ptr = yymsp[0].minor.yy186; DYNAMIC_SORT_PLUS(yygotominor.yy229, yymsp[-2].minor.yy229, yymsp[-1].minor.yy0, Language::Feature::SORT_PLUS, yymsp[0].minor.yy186->symbol()); }
#line 4910 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 321: /* sort ::= sort PLUS IDENTIFIER */
#line 1574 "bcplus/parser/detail/lemon_parser.y"
{
												  u::ref_ptr<const Referenced> s_ptr = yymsp[-2].minor.yy229, op_ptr = yymsp[-1].minor.yy0, id_ptr = yymsp[0].minor.yy0;
												  u::ref_ptr<const ObjectSymbol> obj = parser->symtab()->resolveOrCreate(new ObjectSymbol(yymsp[0].minor.yy0->str()));
												  if(!obj) {
													if (parser->lang()->support(Language::Feature::SORT_PLUS)) 
														parser->_parse_error("\"" + *yymsp[0].minor.yy0->str() + "\" could not be declared as an object as this conflicts with a previous declarations of this identifier.", &yymsp[0].minor.yy0->beginLoc());
													else 
														parser->_feature_error(Language::Feature::SORT_PLUS, &yymsp[-1].minor.yy0->beginLoc());
													YYERROR;
												  } else {
													DYNAMIC_SORT_PLUS(yygotominor.yy229, yymsp[-2].minor.yy229, yymsp[-1].minor.yy0, Language::Feature::SORT_PLUS, obj);
												  }
												}
#line 4927 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 322: /* sort ::= sort PLUS INTEGER */
#line 1588 "bcplus/parser/detail/lemon_parser.y"
{ 
												  ref_ptr<const Object> t_ptr;
												  BASIC_TERM(t_ptr, yymsp[0].minor.yy0);
												  DYNAMIC_SORT_PLUS(yygotominor.yy229, yymsp[-2].minor.yy229, yymsp[-1].minor.yy0, Language::Feature::SORT_PLUS, t_ptr->symbol()); 
												}
#line 4936 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 325: /* sort_nr ::= num_range */
#line 1599 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Referenced> nr_ptr = yymsp[0].minor.yy513;

		yygotominor.yy229 = NULL;

		if (!parser->lang()->support(Language::Feature::NUMRANGE_SORT)) {
			parser->_feature_error(Language::Feature::NUMRANGE_SORT, &yymsp[0].minor.yy513->beginLoc());
			YYERROR;
		}

		// X..Y becomes __rsort_N_
		if(!(yygotominor.yy229 = parser->_newRange(yymsp[0].minor.yy513->min(), yymsp[0].minor.yy513->max()))) {
			parser->_parse_error("INTERNAL ERROR: An error occurred while instantiating the dynamic sort declaration.", &yymsp[0].minor.yy513->beginLoc());
			YYERROR;
		}
	}
#line 4956 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 326: /* sort_id ::= IDENTIFIER */
#line 1617 "bcplus/parser/detail/lemon_parser.y"
{
		// dynamically declare the sort
		yygotominor.yy229 = (SortSymbol*)parser->symtab()->resolve(Symbol::Type::SORT, *yymsp[0].minor.yy0->str());
		if (!yygotominor.yy229) {
			parser->_parse_error("\"" + Symbol::genName(*yymsp[0].minor.yy0->str(),0) + "\" is not a declared sort.", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
		delete yymsp[0].minor.yy0;
	}
#line 4969 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 327: /* stmt_constant_def ::= COLON_DASH CONSTANTS constant_bnd_lst PERIOD */
#line 1648 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0;
		ref_ptr<const Token> kw_ptr = yymsp[-2].minor.yy0;
		ref_ptr<ConstantDeclaration::ElementList> l_ptr = yymsp[-1].minor.yy37;
		ref_ptr<const Token> p_ptr = yymsp[0].minor.yy0;
		
		if (!parser->lang()->support(Language::Feature::DECL_CONSTANT)) {
			yygotominor.yy151 = NULL;
			parser->_feature_error(Language::Feature::DECL_CONSTANT, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy151 = new ConstantDeclaration(yymsp[-1].minor.yy37, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());

		}
	}
#line 4988 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 328: /* constant_bnd_lst ::= constant_bnd */
#line 1665 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy37 = yymsp[0].minor.yy37;
	}
#line 4995 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 329: /* constant_bnd_lst ::= constant_bnd_lst SEMICOLON constant_bnd */
#line 1670 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<ConstantDeclaration::ElementList> bnd_ptr = yymsp[0].minor.yy37;
		yygotominor.yy37 = yymsp[-2].minor.yy37;
		yygotominor.yy37->splice(yygotominor.yy37->end(), *yymsp[0].minor.yy37);
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5005 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 330: /* constant_bnd ::= constant_dcl_lst DBL_COLON constant_dcl_type PAREN_L sort PAREN_R */
#line 1690 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const SortSymbol> s_ptr = yymsp[-1].minor.yy229;
		ref_ptr<const Referenced> names_ptr = yymsp[-5].minor.yy414;
		yygotominor.yy37 = new ConstantDeclaration::ElementList();

		// NOTE: additive constants default to the additive sort, not the boolean sort
		if (yymsp[-3].minor.yy313 & ConstantSymbol::Type::M_ADDITIVE) s_ptr = parser->symtab()->bsort(SymbolTable::BuiltinSort::ADDITIVE);

		// external constants should have "unknown" in their sort
		else if (yymsp[-3].minor.yy313 & ConstantSymbol::Type::M_EXTERNAL) s_ptr = parser->symtab()->carrot(yymsp[-1].minor.yy229);

		// non-boolean abActions should contain "none"
		else if (yymsp[-3].minor.yy313 == ConstantSymbol::Type::ABACTION && s_ptr->domainType() != DomainType::BOOLEAN) s_ptr = parser->symtab()->star(yymsp[-1].minor.yy229);

		BOOST_FOREACH(IdentifierDecl& decl, *yymsp[-5].minor.yy414) {
			// attempt to declare each symbol
			ref_ptr<ConstantSymbol> c = new ConstantSymbol(yymsp[-3].minor.yy313, decl.first->str(), s_ptr, decl.second);
			yygotominor.yy37->push_back(c);
			CONSTANT_DECL(c, decl.first->beginLoc());
		}
	  yy_destructor(yypParser,78,&yymsp[-4].minor);
  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 5033 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 331: /* constant_bnd ::= constant_dcl_lst DBL_COLON sort */
#line 1712 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Referenced> names_ptr = yymsp[-2].minor.yy414, s_ptr = yymsp[0].minor.yy229;
		yygotominor.yy37 = new ConstantDeclaration::ElementList();
		BOOST_FOREACH(IdentifierDecl& decl, *yymsp[-2].minor.yy414) {
			// attempt to declare each symbol
			ref_ptr<ConstantSymbol> c = new ConstantSymbol(ConstantSymbol::Type::RIGID, decl.first->str(), yymsp[0].minor.yy229, decl.second);
			yygotominor.yy37->push_back(c);
			CONSTANT_DECL(c, decl.first->beginLoc());
		}
	  yy_destructor(yypParser,78,&yymsp[-1].minor);
}
#line 5048 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 332: /* constant_bnd ::= constant_dcl_lst DBL_COLON constant_dcl_type */
#line 1723 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Referenced> names_ptr = yymsp[-2].minor.yy414;
		yygotominor.yy37 = new ConstantDeclaration::ElementList();
		BOOST_FOREACH(IdentifierDecl& decl, *yymsp[-2].minor.yy414) {
			// attempt to declare each symbol
			ref_ptr<SortSymbol> s = parser->symtab()->bsort(SymbolTable::BuiltinSort::BOOLEAN);

			// NOTE: additive constants default to the additive sort, not the boolean sort
			if (yymsp[0].minor.yy313 & ConstantSymbol::Type::M_ADDITIVE && s->domainType() == DomainType::BOOLEAN ) 
				s = parser->symtab()->bsort(SymbolTable::BuiltinSort::ADDITIVE);

			// external constants should have "unknown" in their sort
			else if (yymsp[0].minor.yy313 & ConstantSymbol::Type::M_EXTERNAL) 
				s = parser->symtab()->carrot(s);

			// non-boolean abActions should contain "none"
			else if (yymsp[0].minor.yy313 == ConstantSymbol::Type::ABACTION && s->domainType() != DomainType::BOOLEAN) 
				s = parser->symtab()->star(s);


			ref_ptr<ConstantSymbol> c = new ConstantSymbol(yymsp[0].minor.yy313, decl.first->str(), s, decl.second);
			yygotominor.yy37->push_back(c);
			CONSTANT_DECL(c, decl.first->beginLoc());
		}
	  yy_destructor(yypParser,78,&yymsp[-1].minor);
}
#line 5078 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 333: /* constant_bnd ::= constant_dcl_lst DBL_COLON attrib_spec OF IDENTIFIER */
#line 1749 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy37 = NULL;
		ref_ptr<const Referenced> names_ptr = yymsp[-4].minor.yy414, s_ptr = yymsp[-2].minor.yy256, id_ptr = yymsp[0].minor.yy0;


		// attempt to resolve the attribute parent symbol
		ConstantSymbol const* c = (ConstantSymbol const*) parser->symtab()->resolve(Symbol::Type::CONSTANT, *yymsp[0].minor.yy0->str());

		if (!c) {
			parser->_parse_error("\"" + Symbol::genName(*yymsp[0].minor.yy0->str(), 0) + "\" is not a valid constant symbol.", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else if (c->constType() != ConstantSymbol::Type::ABACTION && c->constType() != ConstantSymbol::Type::ACTION && c->constType() != ConstantSymbol::Type::EXOGENOUSACTION) {
			parser->_parse_error("Unexpected constant type of attribute parent \"" + Symbol::genName(*yymsp[0].minor.yy0->str(), 0) + "\". Attribute parents must be an \"abAction\", \"action\", or \"exogenousAction\".", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy37 = new ConstantDeclaration::ElementList();
			BOOST_FOREACH(IdentifierDecl& decl, *yymsp[-4].minor.yy414) {
				ref_ptr<ConstantSymbol> c= new AttributeSymbol(decl.first->str(), yymsp[-2].minor.yy256, c, decl.second);
				yygotominor.yy37->push_back(c);
				CONSTANT_DECL(c, decl.first->beginLoc());
			}
		}
	  yy_destructor(yypParser,78,&yymsp[-3].minor);
  yy_destructor(yypParser,57,&yymsp[-1].minor);
}
#line 5107 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 334: /* constant_bnd ::= constant_dcl_lst DBL_COLON attrib_spec OF IDENTIFIER PAREN_L sort_lst PAREN_R */
#line 1773 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy37 = NULL;
		ref_ptr<const Referenced> names_ptr = yymsp[-7].minor.yy414, s_ptr = yymsp[-5].minor.yy256, id_ptr = yymsp[-3].minor.yy0, lst_ptr = yymsp[-1].minor.yy255;

		// attempt to resolve the attribute parent symbol
		ConstantSymbol const* c = (ConstantSymbol const*) parser->symtab()->resolve(Symbol::Type::CONSTANT, *yymsp[-3].minor.yy0->str(), yymsp[-1].minor.yy255->size());

		if (!c) {
			parser->_parse_error("\"" + Symbol::genName(*yymsp[-3].minor.yy0->str(), yymsp[-1].minor.yy255->size()) + "\" is not a valid constant symbol.", &yymsp[-3].minor.yy0->beginLoc());
			YYERROR;
		} else if (c->constType() != ConstantSymbol::Type::ABACTION && c->constType() != ConstantSymbol::Type::ACTION && c->constType() != ConstantSymbol::Type::EXOGENOUSACTION) {
			parser->_parse_error("Unexpected constant type of attribute parent \"" + Symbol::genName(*yymsp[-3].minor.yy0->str(), yymsp[-1].minor.yy255->size()) + "\". Attribute parents must be an \"abAction\", \"action\", or \"exogenousAction\".", &yymsp[-3].minor.yy0->beginLoc());
			YYERROR;
		} else {
			// ensure that the sorts match the declaration of the symbol
			SortList::const_iterator it = yymsp[-1].minor.yy255->begin();
			BOOST_FOREACH(SortSymbol const* sort, *c) {
				if (*it != sort) {
					// check to see if it'yymsp[-5].minor.yy256 a subsort, which is also permissable
					bool found = false;
					for (SortSymbol::SortList::const_iterator it2 = sort->beginSubSorts(); it2 != sort->endSubSorts(); it2++) {
						if (*it == *it2) {
							found = true;
							break;
						}
					}

					if (!found) {
						parser->_parse_error("Detected a sort mismatch in an attribute parent declaration. \"" + *(*it)->base() + "\" is not an explicit subsort of \"" + *sort->base() + "\".", &yymsp[-3].minor.yy0->beginLoc());
						YYERROR;
					}
				}
				it++;
			}

			yygotominor.yy37 = new ConstantDeclaration::ElementList();
			BOOST_FOREACH(IdentifierDecl& decl, *yymsp[-7].minor.yy414) {
				// ensure that the sorts match the start of the sort list for each of the symbols
				if (decl.second->size() < yymsp[-1].minor.yy255->size()) {
					parser->_parse_error("Detected a malformed attribute declaration. An attribute must duplicate its parent'yymsp[-5].minor.yy256 parameters.", &decl.first->beginLoc());
					YYERROR;
				} else {
					bool good_sort = true;		
					it = decl.second->begin();
					BOOST_FOREACH(SortSymbol const* sort, *yymsp[-1].minor.yy255) {
						if (*it != sort) {
							// check to see if it'yymsp[-5].minor.yy256 a subsort, which is also permissable
							bool found = false;
							for (SortSymbol::SortList::const_iterator it2 = sort->beginSubSorts(); it2 != sort->endSubSorts(); it2++) {
								if (*it == *it2) {
									found = true;
									break;
								}
							}
							if (!found) {
								good_sort = false;
								parser->_parse_error("Detected a sort mismatch in an attribute declaration. \"" + *(*it)->base() + "\" is not an explicit subsort of \"" + *sort->base() + "\".", &decl.first->beginLoc());
								YYERROR;
							}
						}
						it++;	
					}

					if (good_sort) {
						ref_ptr<ConstantSymbol> sym = new AttributeSymbol(decl.first->str(), yymsp[-5].minor.yy256, c, decl.second);
						yygotominor.yy37->push_back(sym);
						CONSTANT_DECL(sym, decl.first->beginLoc());

					}
				}
			}
		}
	  yy_destructor(yypParser,78,&yymsp[-6].minor);
  yy_destructor(yypParser,57,&yymsp[-4].minor);
  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 5188 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 335: /* constant_dcl_lst ::= IDENTIFIER */
#line 1849 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy414 = new IdentifierDeclList();
		yygotominor.yy414->push_back(IdentifierDecl(yymsp[0].minor.yy0, NULL));
	}
#line 5196 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 336: /* constant_dcl_lst ::= IDENTIFIER PAREN_L sort_lst PAREN_R */
#line 1854 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy414 = new IdentifierDeclList();
		yygotominor.yy414->push_back(IdentifierDecl(yymsp[-3].minor.yy0, yymsp[-1].minor.yy255));
	  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 5206 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 337: /* constant_dcl_lst ::= constant_dcl_lst COMMA IDENTIFIER */
#line 1859 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy414 = yymsp[-2].minor.yy414;
		yygotominor.yy414->push_back(IdentifierDecl(yymsp[0].minor.yy0, NULL));
	  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 5215 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 338: /* constant_dcl_lst ::= constant_dcl_lst COMMA IDENTIFIER PAREN_L sort_lst PAREN_R */
#line 1864 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy414 = yymsp[-5].minor.yy414;
		yygotominor.yy414->push_back(IdentifierDecl(yymsp[-3].minor.yy0, yymsp[-1].minor.yy255));
	  yy_destructor(yypParser,104,&yymsp[-4].minor);
  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 5226 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 339: /* constant_dcl_type ::= ABACTION */
#line 1871 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::ABACTION;
		if (!parser->lang()->support(Language::Feature::CONST_ABACTION)) {
			parser->_feature_error(Language::Feature::CONST_ABACTION, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5238 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 340: /* constant_dcl_type ::= ACTION */
#line 1880 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::ACTION;
		if (!parser->lang()->support(Language::Feature::CONST_ACTION)) {
			parser->_feature_error(Language::Feature::CONST_ACTION, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5250 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 341: /* constant_dcl_type ::= ADDITIVEACTION */
#line 1889 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::ADDITIVEACTION;
		if (!parser->lang()->support(Language::Feature::CONST_ADDITIVEACTION)) {
			parser->_feature_error(Language::Feature::CONST_ADDITIVEACTION, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5262 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 342: /* constant_dcl_type ::= ADDITIVEFLUENT */
#line 1898 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::ADDITIVEFLUENT;
		if (!parser->lang()->support(Language::Feature::CONST_ADDITIVEFLUENT)) {
			parser->_feature_error(Language::Feature::CONST_ADDITIVEFLUENT, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5274 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 343: /* constant_dcl_type ::= EXTERNALACTION */
#line 1907 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::EXTERNALACTION;
		if (!parser->lang()->support(Language::Feature::CONST_EXTERNALACTION)) {
			parser->_feature_error(Language::Feature::CONST_EXTERNALACTION, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5286 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 344: /* constant_dcl_type ::= EXTERNALFLUENT */
#line 1916 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::EXTERNALFLUENT;
		if (!parser->lang()->support(Language::Feature::CONST_EXTERNALFLUENT)) {
			parser->_feature_error(Language::Feature::CONST_EXTERNALFLUENT, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5298 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 345: /* constant_dcl_type ::= EXOGENOUSACTION */
#line 1925 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::EXOGENOUSACTION;
		if (!parser->lang()->support(Language::Feature::CONST_EXOGENOUSACTION)) {
			parser->_feature_error(Language::Feature::CONST_EXOGENOUSACTION, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5310 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 346: /* constant_dcl_type ::= INERTIALFLUENT */
#line 1934 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::INERTIALFLUENT;
		if (!parser->lang()->support(Language::Feature::CONST_INERTIALFLUENT)) {
			parser->_feature_error(Language::Feature::CONST_INERTIALFLUENT, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5322 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 347: /* constant_dcl_type ::= RIGID */
#line 1943 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::RIGID;
		if (!parser->lang()->support(Language::Feature::CONST_RIGID)) {
			parser->_feature_error(Language::Feature::CONST_RIGID, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5334 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 348: /* constant_dcl_type ::= SIMPLEFLUENT */
#line 1952 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::SIMPLEFLUENT;
		if (!parser->lang()->support(Language::Feature::CONST_SIMPLEFLUENT)) {
			parser->_feature_error(Language::Feature::CONST_SIMPLEFLUENT, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5346 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 349: /* constant_dcl_type ::= SDFLUENT */
#line 1962 "bcplus/parser/detail/lemon_parser.y"
{ 
		ref_ptr<const Token> tok_ptr = yymsp[0].minor.yy0;
		yygotominor.yy313 = ConstantSymbol::Type::SDFLUENT;
		if (!parser->lang()->support(Language::Feature::CONST_SDFLUENT)) {
			parser->_feature_error(Language::Feature::CONST_SDFLUENT, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
#line 5358 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 350: /* attrib_spec ::= ATTRIBUTE */
#line 1972 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy256 = NULL;
		ref_ptr<const Referenced> kw_ptr = yymsp[0].minor.yy0;
		if (!parser->lang()->support(Language::Feature::CONST_ATTRIBUTE)) {
			parser->_feature_error(Language::Feature::CONST_ATTRIBUTE, &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else {
			// grab the boolean sort and provide it
			yygotominor.yy256 = parser->symtab()->star(parser->symtab()->bsort(SymbolTable::BuiltinSort::BOOLEAN));
		}
	}
#line 5373 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 351: /* attrib_spec ::= ATTRIBUTE PAREN_L sort PAREN_R */
#line 1985 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy256 = NULL;
		ref_ptr<const Referenced> kw_ptr = yymsp[-3].minor.yy0, s_ptr = yymsp[-1].minor.yy229;
		if (!parser->lang()->support(Language::Feature::CONST_ATTRIBUTE)) {
			parser->_feature_error(Language::Feature::CONST_ATTRIBUTE, &yymsp[-3].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy256 = parser->symtab()->star(yymsp[-1].minor.yy229);
		}
	  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 5389 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 352: /* stmt_object_def ::= COLON_DASH OBJECTS object_bnd_lst PERIOD */
#line 2013 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0;
		ref_ptr<const Token> p_ptr = yymsp[0].minor.yy0;
		ref_ptr<const Token> kw_ptr = yymsp[-2].minor.yy0;
		ref_ptr<ObjectDeclaration::ElementList> l_ptr = yymsp[-1].minor.yy262;
		
		if (!parser->lang()->support(Language::Feature::DECL_OBJECT)) {
			yygotominor.yy492 = NULL;
			parser->_feature_error(Language::Feature::DECL_OBJECT, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy492 = new ObjectDeclaration(yymsp[-1].minor.yy262, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());

			// Go ahead and add them to the symbol table
			BOOST_FOREACH(ObjectDeclaration::Element* bnd, *yymsp[-1].minor.yy262) {
				BOOST_FOREACH(Symbol const* o, *bnd) {
					switch (o->type()) {
					case Symbol::Type::OBJECT:
						bnd->sort()->add((ObjectSymbol const*)o);
						break;
					case Symbol::Type::RANGE:
						bnd->sort()->add((NumberRangeSymbol const*)o);
						break;
					default:
						// will not happen
						break;
					}
				}
			}
		}
	}
#line 5424 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 353: /* object_bnd_lst ::= object_bnd */
#line 2046 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy262 = new ObjectDeclaration::ElementList();
		yygotominor.yy262->push_back(yymsp[0].minor.yy302);
	}
#line 5432 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 354: /* object_bnd_lst ::= object_bnd_lst SEMICOLON object_bnd */
#line 2052 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy262 = yymsp[-2].minor.yy262;
		yygotominor.yy262->push_back(yymsp[0].minor.yy302);
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5441 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 355: /* object_bnd ::= object_lst DBL_COLON sort_id */
#line 2058 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy302 = new ObjectDeclaration::Element(yymsp[0].minor.yy229, yymsp[-2].minor.yy285);
	  yy_destructor(yypParser,78,&yymsp[-1].minor);
}
#line 5449 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 356: /* object_lst ::= object_spec */
#line 2063 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy285 = yymsp[0].minor.yy285;
	}
#line 5456 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 357: /* object_lst ::= object_lst COMMA object_spec */
#line 2067 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy285 = yymsp[-2].minor.yy285;
		yygotominor.yy285->splice(yygotominor.yy285->end(), *yymsp[0].minor.yy285);
		delete yymsp[0].minor.yy285;
	  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 5466 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 358: /* object_spec ::= IDENTIFIER */
#line 2076 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Token> id_ptr = yymsp[0].minor.yy0;
		yygotominor.yy285 = NULL;
		ref_ptr<const Symbol> o = parser->symtab()->resolveOrCreate(new ObjectSymbol(yymsp[0].minor.yy0->str()));
		if (!o) {
			parser->_parse_error("Detected a conflicting definition of \"" + Symbol::genName(*yymsp[0].minor.yy0->str(),0) + "\".", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy285 = new ObjectDeclaration::Element::ObjectList();
			yygotominor.yy285->push_back(o);
		}
	}
#line 5482 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 359: /* object_spec ::= IDENTIFIER PAREN_L sort_lst PAREN_R */
#line 2089 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy285 = NULL;
		ref_ptr<ObjectSymbol::SortList> lst_ptr = yymsp[-1].minor.yy255;
		ref_ptr<const Token> id_ptr = yymsp[-3].minor.yy0;
		ref_ptr<ObjectSymbol> o = parser->symtab()->resolveOrCreate(new ObjectSymbol(yymsp[-3].minor.yy0->str(), yymsp[-1].minor.yy255));
		if (!o) {
			parser->_parse_error("Detected a conflicting definition of \"" + Symbol::genName(*yymsp[-3].minor.yy0->str(),yymsp[-1].minor.yy255->size()) + "\".", &yymsp[-3].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy285 = new  ObjectDeclaration::Element::ObjectList();
			yygotominor.yy285->push_back(o.get());
		}
	  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 5501 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 360: /* object_spec ::= INTEGER */
#line 2104 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy285 = NULL;
		ref_ptr<const Token> id_ptr = yymsp[0].minor.yy0;
		ref_ptr<const ObjectSymbol> o = parser->symtab()->resolveOrCreate(new ObjectSymbol(yymsp[0].minor.yy0->str()));
		if (!o) {
			parser->_parse_error("Detected a conflicting definition of \"" + Symbol::genName(*yymsp[0].minor.yy0->str(),0) + "\".", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy285 = new ObjectDeclaration::Element::ObjectList();
			yygotominor.yy285->push_back(o.get());
		}
	}
#line 5517 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 361: /* object_spec ::= num_range */
#line 2118 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy285 = new ObjectDeclaration::Element::ObjectList();
		ref_ptr<const Referenced> nr_ptr = yymsp[0].minor.yy513;

		// iterate over the range and add it to the list
		ref_ptr<const Symbol> o = parser->symtab()->resolveOrCreate(parser->_newRangeSymbol( yymsp[0].minor.yy513->min(), yymsp[0].minor.yy513->max()));
		if (!o) {
			parser->_parse_error("INTERNAL ERROR: Could not create object symbol.", &yymsp[0].minor.yy513->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy285->push_back(o.get());
		}
	}
#line 5534 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 362: /* stmt_variable_def ::= COLON_DASH VARIABLES variable_bnd_lst PERIOD */
#line 2147 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0;
		ref_ptr<const Token> p_ptr = yymsp[0].minor.yy0;
		ref_ptr<const Token> kw_ptr = yymsp[-2].minor.yy0;
		ref_ptr<VariableDeclaration::ElementList> l_ptr = yymsp[-1].minor.yy89;
		
		if (!parser->lang()->support(Language::Feature::DECL_VARIABLE)) {
			yygotominor.yy40 = NULL;
			parser->_feature_error(Language::Feature::DECL_VARIABLE, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else {

			VariableSymbol* v2;

			// Go ahead and add them to the symbol table
			BOOST_FOREACH(ref_ptr<VariableSymbol>& v, *yymsp[-1].minor.yy89) {
				if (!(v2 = parser->symtab()->resolveOrCreate(v))) {
					// Check if it's a duplicate
					v2 = (VariableSymbol*)parser->symtab()->resolve(Symbol::Type::VARIABLE, *v->base());
					if (!v2 || v2 != v) {
						parser->_parse_error("Detected conflicting definition of symbol \"" + *v->name() + "\".", &yymsp[-3].minor.yy0->beginLoc());
					} else {
						parser->_parse_error("Detected a duplicate definition of symbol \"" + *v->name() + "\".", &yymsp[-3].minor.yy0->beginLoc());
					}
				} else {
					v = v2;
				}
			}

			yygotominor.yy40 = new VariableDeclaration(yymsp[-1].minor.yy89, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());


		}
	}
#line 5572 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 363: /* variable_bnd_lst ::= variable_bnd */
#line 2183 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy89 = yymsp[0].minor.yy89;
	}
#line 5579 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 364: /* variable_bnd_lst ::= variable_bnd_lst SEMICOLON variable_bnd */
#line 2188 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy89 = yymsp[-2].minor.yy89;
		yygotominor.yy89->splice(yygotominor.yy89->end(), *yymsp[0].minor.yy89);
		delete yymsp[0].minor.yy89;
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5589 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 365: /* variable_bnd ::= variable_lst DBL_COLON sort */
#line 2195 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy89 = new VariableDeclaration::ElementList();

		BOOST_FOREACH(Token const* tok, *yymsp[-2].minor.yy112) {
			yygotominor.yy89->push_back(new VariableSymbol(tok->str(), yymsp[0].minor.yy229));
		}



		delete yymsp[-2].minor.yy112;
	  yy_destructor(yypParser,78,&yymsp[-1].minor);
}
#line 5605 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 366: /* variable_lst ::= IDENTIFIER */
#line 2208 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy112 = new TokenList();
		yygotominor.yy112->push_back(yymsp[0].minor.yy0);
	}
#line 5613 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 367: /* variable_lst ::= variable_lst COMMA IDENTIFIER */
#line 2213 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy112 = yymsp[-2].minor.yy112;
		yygotominor.yy112->push_back(yymsp[0].minor.yy0);
	  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 5622 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 368: /* stmt_sort_def ::= COLON_DASH SORTS sort_bnd_lst PERIOD */
#line 2234 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0;
		ref_ptr<const Token> p_ptr = yymsp[0].minor.yy0;
		ref_ptr<const Token> kw_ptr = yymsp[-2].minor.yy0;
		ref_ptr<SortDeclaration::ElementList> l_ptr = yymsp[-1].minor.yy247;
		
		if (!parser->lang()->support(Language::Feature::DECL_SORT)) {
			yygotominor.yy93 = NULL;
			parser->_feature_error(Language::Feature::DECL_SORT, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy93 = new SortDeclaration(yymsp[-1].minor.yy247, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
		}
	}
#line 5640 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 369: /* sort_bnd_lst ::= sort_bnd */
      case 371: /* sort_bnd ::= sort_dcl_lst */ yytestcase(yyruleno==371);
#line 2250 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy247 = yymsp[0].minor.yy247;
	}
#line 5648 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 370: /* sort_bnd_lst ::= sort_bnd_lst SEMICOLON sort_bnd */
#line 2255 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy247 = yymsp[-2].minor.yy247;
		yygotominor.yy247->splice(yygotominor.yy247->end(), *yymsp[0].minor.yy247);
		delete yymsp[0].minor.yy247;
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5658 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 372: /* sort_bnd ::= sort_bnd DBL_LTHAN sort_bnd */
#line 2267 "bcplus/parser/detail/lemon_parser.y"
{
		BOOST_FOREACH(SortSymbol* sym, *yymsp[-2].minor.yy247) {
			BOOST_FOREACH(SortSymbol* sym2, *yymsp[0].minor.yy247) {
				sym2->addSubSort(sym);
			}
		}
		yygotominor.yy247 = yymsp[-2].minor.yy247;
		yygotominor.yy247->splice(yymsp[-2].minor.yy247->end(), *yymsp[0].minor.yy247);
		delete yymsp[0].minor.yy247;

	  yy_destructor(yypParser,102,&yymsp[-1].minor);
}
#line 5674 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 373: /* sort_bnd ::= sort_bnd DBL_GTHAN sort_bnd */
#line 2279 "bcplus/parser/detail/lemon_parser.y"
{
		BOOST_FOREACH(SortSymbol* sym, *yymsp[-2].minor.yy247) {
			BOOST_FOREACH(SortSymbol* sym2, *yymsp[0].minor.yy247) {
				sym->addSubSort(sym2);
			}
		}
		yygotominor.yy247 = yymsp[-2].minor.yy247;
		yygotominor.yy247->splice(yymsp[-2].minor.yy247->end(), *yymsp[0].minor.yy247);
		delete yymsp[0].minor.yy247;
	  yy_destructor(yypParser,101,&yymsp[-1].minor);
}
#line 5689 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 374: /* sort_bnd ::= PAREN_L sort_bnd PAREN_R */
#line 2290 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy247 = yymsp[-1].minor.yy247;
	  yy_destructor(yypParser,73,&yymsp[-2].minor);
  yy_destructor(yypParser,74,&yymsp[0].minor);
}
#line 5698 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 375: /* sort_dcl_lst ::= IDENTIFIER */
#line 2295 "bcplus/parser/detail/lemon_parser.y"
{
		ref_ptr<SortSymbol> s = parser->symtab()->resolveOrCreate(new SortSymbol(yymsp[0].minor.yy0->str()));
		if (!s) {
			yygotominor.yy247 = NULL;
			parser->_parse_error("Detected conflicting definition of sort symbol \"" + Symbol::genName(*yymsp[0].minor.yy0->str(),0) + "\".", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy247 = new SortDeclaration::ElementList();
			yygotominor.yy247->push_back(s);
		}

		delete yymsp[0].minor.yy0;
	}
#line 5715 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 376: /* sort_dcl_lst ::= sort_dcl_lst COMMA IDENTIFIER */
#line 2309 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy247 = yymsp[-2].minor.yy247;
		ref_ptr<SortSymbol> s = parser->symtab()->resolveOrCreate(new SortSymbol(yymsp[0].minor.yy0->str()));
		if (!s) {
			yymsp[-2].minor.yy247 = NULL;
			parser->_parse_error("Detected conflicting definition of sort symbol \"" + Symbol::genName(*yymsp[0].minor.yy0->str(),0) + "\".", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yymsp[-2].minor.yy247->push_back(s);
		}

		delete yymsp[0].minor.yy0;

	  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 5734 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 377: /* stmt_show ::= COLON_DASH SHOW show_lst PERIOD */
#line 2336 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy376 = NULL;
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0, kw_ptr = yymsp[-2].minor.yy0, p_ptr = yymsp[0].minor.yy0;
		ref_ptr<ShowStatement::ElementList> lst_ptr = yymsp[-1].minor.yy323;

		if (!parser->lang()->support(Language::Feature::DECL_SHOW)) {
			parser->_feature_error(Language::Feature::DECL_SHOW, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy376 = new ShowStatement(yymsp[-1].minor.yy323, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
		}
	}
#line 5750 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 378: /* stmt_show ::= COLON_DASH SHOW ALL PERIOD */
#line 2350 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy376 = NULL;
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0, kw_ptr = yymsp[-2].minor.yy0, p_ptr = yymsp[0].minor.yy0, all_ptr = yymsp[-1].minor.yy0;
		
		if (!parser->lang()->support(Language::Feature::DECL_SHOW)) {
			parser->_feature_error(Language::Feature::DECL_SHOW, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else if (!parser->lang()->support(Language::Feature::DECL_SHOW_ALL)) {
			parser->_feature_error(Language::Feature::DECL_SHOW_ALL, &yymsp[-1].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy376 = new ShowAllStatement(yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
		}
	}
#line 5768 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 379: /* stmt_hide ::= COLON_DASH HIDE show_lst PERIOD */
#line 2367 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy376 = NULL;
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0, kw_ptr = yymsp[-2].minor.yy0, p_ptr = yymsp[0].minor.yy0;
		ref_ptr<HideStatement::ElementList> lst_ptr = yymsp[-1].minor.yy323;

		if (!parser->lang()->support(Language::Feature::DECL_HIDE)) {
			parser->_feature_error(Language::Feature::DECL_HIDE, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy376 = new HideStatement(yymsp[-1].minor.yy323, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
		}
	}
#line 5784 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 380: /* stmt_hide ::= COLON_DASH HIDE ALL PERIOD */
#line 2381 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy376 = NULL;
		ref_ptr<const Token> cd_ptr = yymsp[-3].minor.yy0, kw_ptr = yymsp[-2].minor.yy0, p_ptr = yymsp[0].minor.yy0, all_ptr = yymsp[-1].minor.yy0;
		
		if (!parser->lang()->support(Language::Feature::DECL_HIDE)) {
			parser->_feature_error(Language::Feature::DECL_HIDE, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else if (!parser->lang()->support(Language::Feature::DECL_HIDE_ALL)) {
			parser->_feature_error(Language::Feature::DECL_HIDE_ALL, &yymsp[-1].minor.yy0->beginLoc());
			YYERROR;
		} else {
			yygotominor.yy376 = new HideAllStatement(yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
		}
	}
#line 5802 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 381: /* show_lst ::= show_elem */
#line 2399 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy323 = new ShowStatement::ElementList();
		yygotominor.yy323->push_back(yymsp[0].minor.yy371);
	}
#line 5810 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 382: /* show_lst ::= show_lst COMMA show_elem */
#line 2404 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy323 = yymsp[-2].minor.yy323;
		yygotominor.yy323->push_back(yymsp[0].minor.yy371);
	  yy_destructor(yypParser,104,&yymsp[-1].minor);
}
#line 5819 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 383: /* show_lst ::= show_lst SEMICOLON show_elem */
#line 2409 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy323 = yymsp[-2].minor.yy323;
		yygotominor.yy323->push_back(yymsp[0].minor.yy371);
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5828 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 385: /* stmt_noconcurrency ::= NOCONCURRENCY PERIOD */
#line 2437 "bcplus/parser/detail/lemon_parser.y"
{ NC_STATEMENT(yygotominor.yy441, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, Language::Feature::NOCONCURRENCY, NCStatement); }
#line 5833 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 386: /* stmt_strong_noconcurrency ::= STRONG_NOCONCURRENCY PERIOD */
#line 2438 "bcplus/parser/detail/lemon_parser.y"
{ NC_STATEMENT(yygotominor.yy370, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, Language::Feature::STRONG_NOCONCURRENCY, StrongNCStatement); }
#line 5838 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 387: /* stmt_maxafvalue ::= COLON_DASH MAXAFVALUE EQ term_int_eval PERIOD */
#line 2464 "bcplus/parser/detail/lemon_parser.y"
{ VALUE_DECL(yygotominor.yy376, yymsp[-4].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy248, yymsp[0].minor.yy0, Language::Feature::DECL_MAXAFVALUE, MaxAFValueStatement);   yy_destructor(yypParser,83,&yymsp[-2].minor);
}
#line 5844 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 388: /* stmt_maxafvalue ::= COLON_DASH MAXAFVALUE DBL_COLON term_int_eval PERIOD */
#line 2465 "bcplus/parser/detail/lemon_parser.y"
{ VALUE_DECL(yygotominor.yy376, yymsp[-4].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy248, yymsp[0].minor.yy0, Language::Feature::DECL_MAXAFVALUE, MaxAFValueStatement);   yy_destructor(yypParser,78,&yymsp[-2].minor);
}
#line 5850 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 389: /* stmt_maxadditive ::= COLON_DASH MAXADDITIVE EQ term_int_eval PERIOD */
#line 2466 "bcplus/parser/detail/lemon_parser.y"
{ VALUE_DECL(yygotominor.yy376, yymsp[-4].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy248, yymsp[0].minor.yy0, Language::Feature::DECL_MAXADDITIVE, MaxAdditiveStatement);   yy_destructor(yypParser,83,&yymsp[-2].minor);
}
#line 5856 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 390: /* stmt_maxadditive ::= COLON_DASH MAXADDITIVE DBL_COLON term_int_eval PERIOD */
#line 2467 "bcplus/parser/detail/lemon_parser.y"
{ VALUE_DECL(yygotominor.yy376, yymsp[-4].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy248, yymsp[0].minor.yy0, Language::Feature::DECL_MAXADDITIVE, MaxAdditiveStatement);   yy_destructor(yypParser,78,&yymsp[-2].minor);
}
#line 5862 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 391: /* stmt_query ::= COLON_DASH QUERY query_lst PERIOD */
#line 2492 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy482 = NULL;
		ref_ptr<const Referenced> cd_ptr = yymsp[-3].minor.yy0, kw_ptr = yymsp[-2].minor.yy0, data_l_ptr = yymsp[-1].minor.yy289.l, p_ptr = yymsp[0].minor.yy0;
		ref_ptr<const Referenced> data_maxstep_ptr = yymsp[-1].minor.yy289.maxstep, data_label_ptr = yymsp[-1].minor.yy289.label;

		ref_ptr<const ReferencedString> label;
		if (yymsp[-1].minor.yy289.label) label = yymsp[-1].minor.yy289.label->str();
		else label = new ReferencedString("0");

		int min = -1, max = -1;
		if (yymsp[-1].minor.yy289.maxstep) {
			min = yymsp[-1].minor.yy289.maxstep->min();
			max = yymsp[-1].minor.yy289.maxstep->max();
		}

		if (!parser->lang()->support(Language::Feature::DECL_QUERY)) {
			parser->_feature_error(Language::Feature::DECL_QUERY, &yymsp[-2].minor.yy0->beginLoc());
			YYERROR;
		} else {
			bool good = true;

			// resolve the query label
			ref_ptr<QuerySymbol> sym = new QuerySymbol(label, min, max);
			if (!parser->symtab()->create(sym)) {
				parser->_parse_error("Could not create query, the label \"" + *label + "\" already exists.", (yymsp[-1].minor.yy289.label ? &yymsp[-1].minor.yy289.label->beginLoc() : &yymsp[-2].minor.yy0->beginLoc()));
				good = false;
				YYERROR;
			}


			if (good) yygotominor.yy482 = new QueryStatement(sym, yymsp[-1].minor.yy289.l, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
		}
	}
#line 5899 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 392: /* query_lst ::= formula_temporal */
#line 2528 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy289.l = new QueryStatement::FormulaList();
		yygotominor.yy289.maxstep = NULL;
		yygotominor.yy289.label = NULL;

		yygotominor.yy289.l->push_back(yymsp[0].minor.yy201);
	}
#line 5910 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 393: /* query_lst ::= query_maxstep_decl */
#line 2537 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy289.l = new QueryStatement::FormulaList();
		yygotominor.yy289.maxstep = yymsp[0].minor.yy97;
		yygotominor.yy289.label = NULL;
	}
#line 5919 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 394: /* query_lst ::= query_label_decl */
#line 2544 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy289.l = new QueryStatement::FormulaList();
		yygotominor.yy289.maxstep = NULL;
		yygotominor.yy289.label = yymsp[0].minor.yy167;
	}
#line 5928 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 395: /* query_lst ::= query_lst SEMICOLON formula_temporal */
#line 2551 "bcplus/parser/detail/lemon_parser.y"
{ 
		yygotominor.yy289 = yymsp[-2].minor.yy289;
		yymsp[-2].minor.yy289.l->push_back(yymsp[0].minor.yy201);
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5937 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 396: /* query_lst ::= query_lst SEMICOLON query_maxstep_decl */
#line 2557 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy289 = yymsp[-2].minor.yy289;

		if (yygotominor.yy289.maxstep) {
			parser->_parse_error("Encountered multiple maxstep definitions within a query.", &yymsp[0].minor.yy97->beginLoc());
			delete yymsp[0].minor.yy97;
			YYERROR;
		} else {
			yygotominor.yy289.maxstep = yymsp[0].minor.yy97;
		}
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5953 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 397: /* query_lst ::= query_lst SEMICOLON query_label_decl */
#line 2570 "bcplus/parser/detail/lemon_parser.y"
{
		yygotominor.yy289 = yymsp[-2].minor.yy289;
		if (yygotominor.yy289.label) {
			parser->_parse_error("Encountered multiple maxstep definitions within a query.", &yymsp[0].minor.yy167->beginLoc());
			delete yymsp[0].minor.yy167;
			YYERROR;

		} else {
			yygotominor.yy289.label = yymsp[0].minor.yy167;
		}
	  yy_destructor(yypParser,95,&yymsp[-1].minor);
}
#line 5969 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 398: /* query_maxstep_decl ::= MAXSTEP DBL_COLON INTEGER */
#line 2596 "bcplus/parser/detail/lemon_parser.y"
{ 
	yygotominor.yy97 = NULL;
	ref_ptr<const Referenced> kw_ptr = yymsp[-2].minor.yy0, i_ptr = yymsp[0].minor.yy0;


	if (!parser->lang()->support(Language::Feature::QUERY_MAXSTEP)) {
		parser->_feature_error(Language::Feature::QUERY_MAXSTEP, &yymsp[-2].minor.yy0->beginLoc());
		YYERROR;
	} else {

		int max = -1;
		try {
			max = boost::lexical_cast<int>(*yymsp[0].minor.yy0->str());
			yygotominor.yy97 = new NumberRangeEval(-1, max, yymsp[0].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
		} catch (boost::bad_lexical_cast const& e) {
			parser->_parse_error("INTERNAL ERROR: An error occurred extracting an integer from \"" + *yymsp[0].minor.yy0->str() + "\".", &yymsp[0].minor.yy0->beginLoc());
			YYERROR;
		}
	}
  yy_destructor(yypParser,78,&yymsp[-1].minor);
}
#line 5994 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 399: /* query_maxstep_decl ::= MAXSTEP DBL_COLON num_range_eval */
#line 2617 "bcplus/parser/detail/lemon_parser.y"
{
	yygotominor.yy97 = NULL;
	ref_ptr<const Referenced> kw_ptr = yymsp[-2].minor.yy0, nr_ptr = yymsp[0].minor.yy97;

	if (!parser->lang()->support(Language::Feature::QUERY_MAXSTEP)) {
		parser->_feature_error(Language::Feature::QUERY_MAXSTEP, &yymsp[-2].minor.yy0->beginLoc());
		YYERROR;
	} else {
		yygotominor.yy97 = yymsp[0].minor.yy97;
		nr_ptr.release();
	}
  yy_destructor(yypParser,78,&yymsp[-1].minor);
}
#line 6011 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 400: /* query_label_decl ::= LABEL DBL_COLON INTEGER */
      case 401: /* query_label_decl ::= LABEL DBL_COLON IDENTIFIER */ yytestcase(yyruleno==401);
#line 2631 "bcplus/parser/detail/lemon_parser.y"
{ QUERY_DECL(yygotominor.yy167, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, Language::Feature::QUERY_LABEL);   yy_destructor(yypParser,78,&yymsp[-1].minor);
}
#line 6018 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 402: /* clause_if ::= IF formula */
#line 2666 "bcplus/parser/detail/lemon_parser.y"
{ CLAUSE(yygotominor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, Language::Feature::CLAUSE_IF); 		}
#line 6023 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 403: /* clause_if ::= */
      case 405: /* clause_after ::= */ yytestcase(yyruleno==405);
      case 407: /* clause_ifcons ::= */ yytestcase(yyruleno==407);
      case 411: /* clause_where ::= */ yytestcase(yyruleno==411);
#line 2667 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy201 = NULL; }
#line 6031 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 404: /* clause_after ::= AFTER formula */
#line 2668 "bcplus/parser/detail/lemon_parser.y"
{ CLAUSE(yygotominor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, Language::Feature::CLAUSE_AFTER);	}
#line 6036 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 406: /* clause_ifcons ::= IFCONS formula */
#line 2670 "bcplus/parser/detail/lemon_parser.y"
{ CLAUSE(yygotominor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, Language::Feature::CLAUSE_IFCONS); 	}
#line 6041 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 408: /* clause_unless ::= UNLESS atomic_formula_anon */
#line 2672 "bcplus/parser/detail/lemon_parser.y"
{ CLAUSE(yygotominor.yy371, yymsp[-1].minor.yy0, yymsp[0].minor.yy371, Language::Feature::CLAUSE_UNLESS); 	}
#line 6046 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 409: /* clause_unless ::= */
#line 2673 "bcplus/parser/detail/lemon_parser.y"
{ yygotominor.yy371 = NULL; }
#line 6051 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 410: /* clause_where ::= WHERE formula_no_const */
#line 2674 "bcplus/parser/detail/lemon_parser.y"
{ CLAUSE(yygotominor.yy201, yymsp[-1].minor.yy0, yymsp[0].minor.yy201, Language::Feature::CLAUSE_WHERE); 	}
#line 6056 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 412: /* stmt_law ::= law_basic */
      case 413: /* stmt_law ::= law_caused */ yytestcase(yyruleno==413);
      case 414: /* stmt_law ::= law_pcaused */ yytestcase(yyruleno==414);
      case 415: /* stmt_law ::= law_impl */ yytestcase(yyruleno==415);
      case 416: /* stmt_law ::= law_causes */ yytestcase(yyruleno==416);
      case 417: /* stmt_law ::= law_increments */ yytestcase(yyruleno==417);
      case 418: /* stmt_law ::= law_decrements */ yytestcase(yyruleno==418);
      case 419: /* stmt_law ::= law_mcause */ yytestcase(yyruleno==419);
      case 420: /* stmt_law ::= law_always */ yytestcase(yyruleno==420);
      case 421: /* stmt_law ::= law_constraint */ yytestcase(yyruleno==421);
      case 422: /* stmt_law ::= law_impossible */ yytestcase(yyruleno==422);
      case 423: /* stmt_law ::= law_never */ yytestcase(yyruleno==423);
      case 424: /* stmt_law ::= law_default */ yytestcase(yyruleno==424);
      case 425: /* stmt_law ::= law_exogenous */ yytestcase(yyruleno==425);
      case 426: /* stmt_law ::= law_inertial */ yytestcase(yyruleno==426);
      case 427: /* stmt_law ::= law_nonexecutable */ yytestcase(yyruleno==427);
      case 428: /* stmt_law ::= law_rigid */ yytestcase(yyruleno==428);
      case 429: /* stmt_law ::= law_observed */ yytestcase(yyruleno==429);
      case 430: /* stmt_law ::= law_temporal_constraint */ yytestcase(yyruleno==430);
#line 2722 "bcplus/parser/detail/lemon_parser.y"
{yygotominor.yy376 = yymsp[0].minor.yy376;}
#line 6079 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 431: /* law_basic ::= head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD */
#line 2843 "bcplus/parser/detail/lemon_parser.y"
{ 
		if (yymsp[-5].minor.yy201 || yymsp[-4].minor.yy201 || yymsp[-3].minor.yy201 || yymsp[-2].minor.yy371 || yymsp[-1].minor.yy201) {
			LAW_BASIC_FORM(yygotominor.yy376, NULL, yymsp[-6].minor.yy201, yymsp[-5].minor.yy201, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, 
				yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, Language::Feature::LAW_BASIC_S, 
				Language::Feature::LAW_BASIC_D, BasicLaw); 
		} else {
			LAW_BASIC_FORM(yygotominor.yy376, NULL, yymsp[-6].minor.yy201, yymsp[-5].minor.yy201, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, 
				yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, Language::Feature::LAW_BASIC_FACT, 
				Language::Feature::LAW_BASIC_FACT, BasicLaw); 
		}
	}
#line 6094 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 432: /* law_caused ::= CAUSED head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD */
#line 2855 "bcplus/parser/detail/lemon_parser.y"
{ LAW_BASIC_FORM(yygotominor.yy376, yymsp[-7].minor.yy0, yymsp[-6].minor.yy201, yymsp[-5].minor.yy201, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, 
																																														yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, Language::Feature::LAW_CAUSED_S, 
																																															Language::Feature::LAW_CAUSED_D, CausedLaw); }
#line 6101 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 433: /* law_pcaused ::= POSSIBLY_CAUSED head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD */
#line 2859 "bcplus/parser/detail/lemon_parser.y"
{ LAW_BASIC_FORM(yygotominor.yy376, yymsp[-7].minor.yy0, yymsp[-6].minor.yy201, yymsp[-5].minor.yy201, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, 
																																														yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, Language::Feature::LAW_PCAUSED_S, 
																																															Language::Feature::LAW_PCAUSED_D, PossiblyCausedLaw); }
#line 6108 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 434: /* law_impl ::= head_formula ARROW_LDASH formula clause_where PERIOD */
#line 2863 "bcplus/parser/detail/lemon_parser.y"
{ LAW_IMPL_FORM(yygotominor.yy376, yymsp[-4].minor.yy201, yymsp[-3].minor.yy0, yymsp[-2].minor.yy201, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, 
																																														Language::Feature::LAW_IMPL, ImplicationLaw); }
#line 6114 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 435: /* law_impl ::= ARROW_LDASH formula clause_where PERIOD */
#line 2866 "bcplus/parser/detail/lemon_parser.y"
{ LAW_IMPL_FORM(yygotominor.yy376, NULL, yymsp[-3].minor.yy0, yymsp[-2].minor.yy201, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, 
																																														Language::Feature::LAW_IMPL, ImplicationLaw); }
#line 6120 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 436: /* law_causes ::= atomic_formula CAUSES head_formula clause_if clause_unless clause_where PERIOD */
#line 2869 "bcplus/parser/detail/lemon_parser.y"
{ LAW_DYNAMIC_FORM(yygotominor.yy376, yymsp[-6].minor.yy371, yymsp[-5].minor.yy0, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_CAUSES, CausesLaw); }
#line 6126 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 437: /* law_increments ::= atomic_formula INCREMENTS constant BY term clause_if clause_unless clause_where PERIOD */
#line 2873 "bcplus/parser/detail/lemon_parser.y"
{ LAW_INCREMENTAL_FORM(yygotominor.yy376, yymsp[-8].minor.yy371, yymsp[-7].minor.yy0, yymsp[-6].minor.yy301, yymsp[-4].minor.yy223, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_INCREMENTS, IncrementsLaw);   yy_destructor(yypParser,33,&yymsp[-5].minor);
}
#line 6133 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 438: /* law_decrements ::= atomic_formula DECREMENTS constant BY term clause_if clause_unless clause_where PERIOD */
#line 2876 "bcplus/parser/detail/lemon_parser.y"
{ LAW_INCREMENTAL_FORM(yygotominor.yy376, yymsp[-8].minor.yy371, yymsp[-7].minor.yy0, yymsp[-6].minor.yy301, yymsp[-4].minor.yy223, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_DECREMENTS, DecrementsLaw);   yy_destructor(yypParser,33,&yymsp[-5].minor);
}
#line 6140 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 439: /* law_mcause ::= atomic_formula MAY_CAUSE head_formula clause_if clause_unless clause_where PERIOD */
#line 2880 "bcplus/parser/detail/lemon_parser.y"
{ LAW_DYNAMIC_FORM(yygotominor.yy376, yymsp[-6].minor.yy371, yymsp[-5].minor.yy0, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_MCAUSE, MayCauseLaw); }
#line 6146 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 440: /* law_always ::= ALWAYS formula clause_after clause_unless clause_where PERIOD */
#line 2884 "bcplus/parser/detail/lemon_parser.y"
{ LAW_CONSTRAINT_FORM(yygotominor.yy376, yymsp[-5].minor.yy0, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_ALWAYS_S, 
																																															Language::Feature::LAW_ALWAYS_D, AlwaysLaw); }
#line 6153 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 441: /* law_constraint ::= CONSTRAINT formula clause_after clause_unless clause_where PERIOD */
#line 2888 "bcplus/parser/detail/lemon_parser.y"
{ LAW_CONSTRAINT_FORM(yygotominor.yy376, yymsp[-5].minor.yy0, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_CONSTRAINT_S, 
																																															Language::Feature::LAW_CONSTRAINT_D, ConstraintLaw); }
#line 6160 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 442: /* law_impossible ::= IMPOSSIBLE formula clause_after clause_unless clause_where PERIOD */
#line 2892 "bcplus/parser/detail/lemon_parser.y"
{ LAW_CONSTRAINT_FORM(yygotominor.yy376, yymsp[-5].minor.yy0, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_IMPOSSIBLE_S, 
																																															Language::Feature::LAW_IMPOSSIBLE_D, ImpossibleLaw); }
#line 6167 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 443: /* law_never ::= NEVER formula clause_after clause_unless clause_where PERIOD */
#line 2896 "bcplus/parser/detail/lemon_parser.y"
{ LAW_CONSTRAINT_FORM(yygotominor.yy376, yymsp[-5].minor.yy0, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_NEVER_S, 
																																															Language::Feature::LAW_NEVER_D, NeverLaw); }
#line 6174 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 444: /* law_default ::= DEFAULT atomic_head_formula clause_if clause_ifcons clause_after clause_unless clause_where PERIOD */
#line 2900 "bcplus/parser/detail/lemon_parser.y"
{ LAW_BASIC_FORM(yygotominor.yy376, yymsp[-7].minor.yy0, yymsp[-6].minor.yy371, yymsp[-5].minor.yy201, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, 
																																														yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, Language::Feature::LAW_DEFAULT_S,
																																															Language::Feature::LAW_DEFAULT_D, DefaultLaw); }
#line 6181 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 445: /* law_exogenous ::= EXOGENOUS constant clause_if clause_ifcons clause_after clause_unless clause_where PERIOD */
#line 2904 "bcplus/parser/detail/lemon_parser.y"
{ LAW_BASIC_FORM(yygotominor.yy376, yymsp[-7].minor.yy0, yymsp[-6].minor.yy301, yymsp[-5].minor.yy201, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, 
																																														yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, Language::Feature::LAW_EXOGENOUS_S,
																																															Language::Feature::LAW_EXOGENOUS_D, ExogenousLaw); }
#line 6188 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 446: /* law_inertial ::= INERTIAL constant clause_if clause_ifcons clause_after clause_unless clause_where PERIOD */
#line 2908 "bcplus/parser/detail/lemon_parser.y"
{ LAW_BASIC_FORM(yygotominor.yy376, yymsp[-7].minor.yy0, yymsp[-6].minor.yy301, yymsp[-5].minor.yy201, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, 
																																														yymsp[-2].minor.yy371, yymsp[-1].minor.yy201, yymsp[0].minor.yy0, Language::Feature::LAW_INERTIAL_S,
																																															Language::Feature::LAW_INERTIAL_D, InertialLaw); }
#line 6195 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 447: /* law_nonexecutable ::= NONEXECUTABLE formula clause_if clause_unless clause_where PERIOD */
#line 2912 "bcplus/parser/detail/lemon_parser.y"
{ LAW_DYNAMIC_CONSTRAINT_FORM(yygotominor.yy376, yymsp[-5].minor.yy0, yymsp[-4].minor.yy201, yymsp[-3].minor.yy201, yymsp[-2].minor.yy371, yymsp[-1].minor.yy201,
																																														yymsp[0].minor.yy0, Language::Feature::LAW_NONEXECUTABLE, NonexecutableLaw); }
#line 6201 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 448: /* law_rigid ::= RIGID constant clause_where PERIOD */
#line 2916 "bcplus/parser/detail/lemon_parser.y"
{ LAW_SIMPLE_FORM(yygotominor.yy376, yymsp[-3].minor.yy0, yymsp[-2].minor.yy301, yymsp[-1].minor.yy201, yymsp[0].minor.yy0,
																																														Language::Feature::LAW_RIGID, RigidLaw); }
#line 6207 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 449: /* law_observed ::= OBSERVED atomic_head_formula AT term_int_eval PERIOD */
#line 2921 "bcplus/parser/detail/lemon_parser.y"
{ 
			LAW_SIMPLE_FORM(yygotominor.yy376, yymsp[-4].minor.yy0, yymsp[-3].minor.yy371, yymsp[-1].minor.yy248, yymsp[0].minor.yy0, Language::Feature::LAW_OBSERVED, ObservedLaw); 
		  yy_destructor(yypParser,67,&yymsp[-2].minor);
}
#line 6215 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 450: /* law_temporal_constraint ::= CONSTRAINT formula AT term_int_eval PERIOD */
#line 2926 "bcplus/parser/detail/lemon_parser.y"
{ 
			LAW_SIMPLE_FORM(yygotominor.yy376, yymsp[-4].minor.yy0, yymsp[-3].minor.yy201, yymsp[-1].minor.yy248, yymsp[0].minor.yy0, Language::Feature::LAW_TEMPORAL_CONSTRAINT, TemporalConstraintLaw); 
		  yy_destructor(yypParser,67,&yymsp[-2].minor);
}
#line 6223 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 451: /* stmt_code_blk ::= ASP_GR */
#line 2949 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_ASP_GR, ASPBlock);	}
#line 6228 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 452: /* stmt_code_blk ::= ASP_CP */
#line 2950 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_ASP_CP, ASPBlock);	}
#line 6233 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 453: /* stmt_code_blk ::= F2LP_GR */
#line 2951 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_F2LP_GR, F2LPBlock);	}
#line 6238 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 454: /* stmt_code_blk ::= F2LP_CP */
#line 2952 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_F2LP_CP, F2LPBlock); }
#line 6243 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 455: /* stmt_code_blk ::= LUA_GR */
#line 2953 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_LUA_GR, LUABlock);   }
#line 6248 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 456: /* stmt_code_blk ::= LUA_CP */
#line 2954 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_LUA_CP, LUABlock);   }
#line 6253 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 457: /* stmt_code_blk ::= PYTHON_GR */
#line 2955 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_PYTHON_GR, PYTHONBlock);   }
#line 6258 "bcplus/parser/detail/lemon_parser.c"
        break;
      case 458: /* stmt_code_blk ::= PYTHON_CP */
#line 2956 "bcplus/parser/detail/lemon_parser.y"
{ CODE_BLK(yygotominor.yy376, yymsp[0].minor.yy0, Language::Feature::CODE_PYTHON_CP, PYTHONBlock);   }
#line 6263 "bcplus/parser/detail/lemon_parser.c"
        break;
      default:
      /* (1) statement_lst ::= */ yytestcase(yyruleno==1);
      /* (2) statement_lst ::= statement_lst error */ yytestcase(yyruleno==2);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  lemon_parserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  lemon_parserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  lemon_parserARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 202 "bcplus/parser/detail/lemon_parser.y"
 parser->_parse_error("Syntax error.");	
#line 6329 "bcplus/parser/detail/lemon_parser.c"
  lemon_parserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  lemon_parserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  lemon_parserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}


/*
** Handles a syntax error within the parser.
*/
static void yy_handle_err(yyParser* yypParser, int* yyerrorhit) {
      int yyact;
#ifdef YYERRORSYMBOL
  int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
	  yyact = YY_ERROR_ACTION;
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yypParser->yylookmajor,yypParser->yylookminor);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || *yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yypParser->yylookmajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yypParser->yylookmajor,&yypParser->yylookminor);
        yypParser->yylookmajor = YYNOCODE;
        yypParser->yylookminor = yyzerominor;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yypParser->yylookmajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yypParser->yylookmajor,&yypParser->yylookminor);
          yy_parse_failed(yypParser);
          yypParser->yylookmajor = YYNOCODE;
          yypParser->yylookminor = yyzerominor;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      *yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yypParser->yylookmajor,yypParser->yylookminor);
      yy_destructor(yypParser,(YYCODETYPE)yypParser->yylookmajor,&yypParser->yylookminor);
      yypParser->yylookmajor = YYNOCODE;
      yypParser->yylookminor = yyzerominor;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yypParser->yylookmajor,yypParser->yylookminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yypParser->yylookmajor,&yypParser->yylookminor);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yypParser->yylookmajor = YYNOCODE;
      yypParser->yylookminor = yyzerominor;
#endif
      yypParser->yysyntaxerr = 0;
}


/*
** Prepares the parser to accept tokens injected at the current
** location by extracting the lookahead token so that it can be
** reintroduced into the stream.
** Also pops the latest symbol off the parser's stack if the pop
** option is asserted.
** 
** returns the major type of the lookahead token that has been 
** cleared from the parser or YYNOCODE and sets the lookahead minor
** type appropriately.
*/
int lemon_parserPreInject(void* yyp, int pop, lemon_parserTOKENTYPE* lookahead) {
	yyParser* pParser = (yyParser*)yyp;
	int code = pParser->yylookmajor;
	if (pop && pParser->yyidx) yy_pop_parser_stack(pParser);
	if (code != YYNOCODE) {
		*lookahead = pParser->yylookminor.yy0;
		pParser->yylookmajor = YYNOCODE;
	    pParser->yylookminor = yyzerominor;
		return code;
	} else {
		*lookahead = yyzerominor.yy0;
		return 0;

	}
}

/*
** Gets the name of the provided token.
** Primarily for debugging purposes.
**
*/
char const* lemon_parserTokenName(int tok) {
	if (tok < 1) return "<INVALID_TOKEN>";
	else if (tok == YYNOCODE) return "<NOCODE_TOKEN>";
#ifdef YYERRORSYMBOL
	else if (tok == YYERRORSYMBOL) return "<ERROR_TOKEN>";
#endif
	return yyTokenName[tok];
}


/*
** Checks to see if there is a next-token independent reduction rule
** and executes it.
*/
void lemon_parserAttemptReduce(void* yyp lemon_parserARG_PDECL) {
	yyParser* yypParser = (yyParser*)yyp;
	lemon_parserARG_STORE;
	int act = 0;
	int yyerrorhit = 0;
	do {
		yypParser->yysyntaxerr = 0;
		act = yy_find_reduce_action(yypParser->yystack[yypParser->yyidx].stateno, YYNOCODE);
		if (act >= YYNSTATE && act < YYNSTATE + YYNRULE) {
			// There is a reduce action. Do it.
			yy_reduce(yypParser, act-YYNSTATE);	
		}

		if (yypParser->yysyntaxerr) {
			yy_handle_err(yypParser, &yyerrorhit);
		}

	} while (act >= YYNSTATE && act < YYNSTATE + YYNRULE);
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "lemon_parserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void lemon_parser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  lemon_parserTOKENTYPE yyminor       /* The value for the token */
  lemon_parserARG_PDECL               /* Optional %extra_argument parameter */
){
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yypParser->yylookmajor = YYNOCODE;
      yypParser->yylookminor = yyzerominor;
      yyStackOverflow(yypParser/*, &yyminorunion */);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yypParser->yylookmajor = yymajor;
  yypParser->yylookminor.yy0 = yyminor;
  yyendofinput = (yypParser->yylookmajor==0);
  lemon_parserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yypParser->yylookmajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yypParser->yylookmajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yypParser->yylookmajor,&yypParser->yylookminor);
      yypParser->yyerrcnt--;
      yypParser->yylookmajor = YYNOCODE;
      yypParser->yylookminor = yyzerominor;
    }else if( yyact < YYNSTATE + YYNRULE ) {
      yy_reduce(yypParser,yyact-YYNSTATE);
    } else {
      	assert( yyact == YY_ERROR_ACTION );
		yypParser->yysyntaxerr = 1; 
	}
	
	if (yypParser->yysyntaxerr) {
		yy_handle_err(yypParser, &yyerrorhit);
    }
  }while( yypParser->yylookmajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
