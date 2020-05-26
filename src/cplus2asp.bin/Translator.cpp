#include "Translator.h"

#include <string>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/DomainType.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/AttributeSymbol.h"
#include "bcplus/symbols/ConstantSymbol.h"
#include "bcplus/symbols/NumberRangeSymbol.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/declarations.h"
#include "bcplus/statements/blocks.h"
#include "bcplus/statements/QueryStatement.h"
#include "bcplus/statements/laws.h"
#include "bcplus/parser/Token.h"
#include "bcplus/elements/formulas.h"
#include "bcplus/elements/terms.h"

#include "Configuration.h"
#include "Context.h"
#include "ObjectData.h"
#include "SortData.h"
#include "ConstantData.h"
#include "RangeData.h"
namespace cplus2asp {
namespace cplus2asp_bin {

using namespace bcplus;
namespace st = bcplus::statements;
namespace sy = bcplus::symbols;
namespace el = bcplus::elements;
namespace u = babb::utils;


#define HANDLE_ARGS(id, context, out, parens, comma)\
	if (id->arity()) {										\
		if (parens) out << "(";								\
		bool first = true;									\
															\
		BOOST_FOREACH(el::Term const* t, *id) {				\
			if (!first) out << ", ";						\
			first = false;									\
			translate(t, context, out);						\
		}													\
		if (parens) out << ")";								\
		if (comma) out << ", ";								\
	}														\

#define HANDLE_SORT_ARGS(id, context, out, parens, comma)	\
	HANDLE_SORT_ARGS_6(id, context, out, parens, comma, (ClauseList const*)NULL)

#define HANDLE_SORT_ARGS_6(id, context, out, parens, comma, setArgs)\
	{																\
			if (id->arity()) {										\
				if (parens) out << "(";								\
				std::stringstream tmp;								\
				bool first = true;									\
				ClauseList::const_iterator it;						\
				if (setArgs) it = (setArgs)->begin();				\
				BOOST_FOREACH(sy::SortSymbol const* sort, *id) {	\
					if (!first) out << ", ";						\
					first = false;									\
																	\
					if (setArgs && it != (setArgs)->end()) {		\
						out << *it++;								\
					} else {										\
																	\
						out << *newVar(sort, context);				\
																	\
					}												\
				}													\
				if (parens) out << ")";								\
				if (comma) out << ", ";								\
			}														\
	}
#define HANDLE_PRESTMTS 											\
	{																\
		IPart::type ipart = _ipart;									\
		BOOST_FOREACH(TranslationStatement& stmt, *preStmts) {		\
			assertIPart(stmt.second);								\
			config()->out() << stmt.first << std::endl;				\
		}															\
		preStmts->clear();											\
		assertIPart(ipart);											\
	}

#define HANDLE_CLAUSES(out, arrow, period) 							\
	HANDLE_CLAUSES_4(out, arrow, period, period)

#define HANDLE_CLAUSES_4(out, arrow, period, newline) 				\
	HANDLE_CLAUSES_5(out, arrow, period, newline, " & ")

#define HANDLE_CLAUSES_5(out, arrow, period, newline, sep) 			\
	HANDLE_CLAUSES_6(out, arrow, period, newline, sep, !arrow)

#define HANDLE_CLAUSES_6(out, arrow, period, newline, sep, sepfirst)\
	/* handle clauses */											\
	if (extraClauses->size()) {										\
		bool first = !(sepfirst);									\
		if (arrow) out << " <- ";									\
		BOOST_FOREACH(std::string& cl, *extraClauses) {				\
			if (!first) out << sep;									\
			first = false;											\
			out << cl;												\
		}															\
		extraClauses->clear();										\
	}																\
	if (period) out  << ".";										\
	if (newline) out << std::endl;



void Translator::SymbolMetadataInitializer::initMetadata(sy::Symbol* sym) const {

	if (!sym->metadata()) {
		switch (sym->type()) {
		case sy::Symbol::Type::SORT:
			sym->metadata(new SortData((sy::SortSymbol*)sym));
			break;
		case sy::Symbol::Type::CONSTANT:
			sym->metadata(new ConstantData((sy::ConstantSymbol*)sym));
			break;
		case sy::Symbol::Type::OBJECT:
			sym->metadata(new ObjectData((sy::ObjectSymbol*)sym));
			break;
		
		case sy::Symbol::Type::RANGE:
			sym->metadata(new RangeData((sy::NumberRangeSymbol*)sym));
			break;

		default:
			break;
		}

	}
}


Translator::Translator(Configuration const* config, sy::SymbolTable* symtab)
	: _config(config), _symtab(symtab), _ipart(IPart::NONE), _constcount(1), _stmtcount(0),
	  _noconcurrency(false), _strong_noconcurrency(false), _computed(true) {
	/* Intentionally left blank */
}

Translator::~Translator() {
	/* Intentionally left blank */
}

void Translator::prologue() {


	assertIPart(IPart::BASE);

	config()->out()		<< std::endl
						<< "% -------------------------------------------------------------------------" 	<< std::endl
						<< "% preamble ----------------------------------------------------------------" 	<< std::endl
						<< "% -------------------------------------------------------------------------" 	<< std::endl
						<< std::endl;



	if (config()->outputLanguage() == Configuration::Output::STATIC) {

		if (config()->inputLanguage() != Configuration::Input::MVPF) {
			config()->out() << "step(0..maxstep)." 																<< std::endl
							<< "astep(1..maxstep)."																<< std::endl
							<< "#domain step(" << *config()->ts(Configuration::TS::STATIC) << ")."				<< std::endl
							<< "#domain astep(" << *config()->ts(Configuration::TS::DYNAMIC) << ")."			<< std::endl;
			}
	}



/*
	config()->out() 	<< "constant(VAR) <- fluent(VAR)."													<< std::endl
						<< "constant(VAR) <- action(VAR)."													<< std::endl
						<< "constant(VAR) <- static(VAR)."													<< std::endl
	
						<< "fluent(VAR) <- simpleFluent(VAR)."												<< std::endl
						<< "fluent(VAR) <- inertialFluent(VAR)."											<< std::endl
						<< "fluent(VAR) <- sdFluent(VAR)."													<< std::endl
						<< "fluent(VAR) <- externalFluent(VAR)."											<< std::endl
						<< "simpleFluent(VAR) <- inertialFluent(VAR)."										<< std::endl

						<< "action(VAR) <- exogenousAction(VAR)."											<< std::endl
						<< "action(VAR) <- abAction(VAR)."													<< std::endl
						<< "action(VAR) <- attribute(VAR)."													<< std::endl
						<< "action(VAR) <- externalAction(VAR)."											<< std::endl

						<< "object(VAR) <- computed(VAR)."													<< std::endl

						<< "constant_object(C,V) <- constant_sort(C,S) & sort_object(S,V) & constant(C) & sort(S) & object(V)." << std::endl
*/
	config()->out()		<< std::endl
						<< "% -------------------------------------------------------------------------" 	<< std::endl
						<< "% main body ---------------------------------------------------------------" 	<< std::endl
						<< "% -------------------------------------------------------------------------" 	<< std::endl
						<< std::endl;







}


void Translator::epilogue() {




	// handle noconcurrency clauses...
	bool snc = _strong_noconcurrency || config()->inputLanguage() == Configuration::Input::BCPLUS || config()->inputLanguage() == Configuration::Input::BC;
	if (_noconcurrency || _strong_noconcurrency) {

		config()->out() << std::endl
						<< "% -------------------------------------------------------------------------" << std::endl
						<< "% noconcurrency -----------------------------------------------------------" << std::endl
						<< "% -------------------------------------------------------------------------" << std::endl
						<< std::endl;
		
		std::stringstream tmpout;
		u::ref_ptr<StatementList> preStmts = new StatementList();
		u::ref_ptr<ClauseList> extraClauses = new ClauseList(); 
		u::ref_ptr<Context> c = new Context(config(), Context::Position::BODY, IPart::CUMULATIVE, preStmts, extraClauses, 
			NULL, true, config()->ts(Configuration::TS::ACTION));

		u::ref_ptr<ClauseList> args1 = new ClauseList(); 
		u::ref_ptr<ClauseList> args2 = new ClauseList(); 

		std::string val, val2;

		for(sy::SymbolTable::const_iterator it = symtab()->begin(sy::Symbol::Type::CONSTANT); it != symtab()->end(sy::Symbol::Type::CONSTANT); ++it) {

			bool snc_1 = false;
			u::ref_ptr<const sy::ConstantSymbol> c1 = (sy::ConstantSymbol const*)(it->get());

			if (!(c1->constType() & sy::ConstantSymbol::Type::M_REGULAR_ACTIONS)) continue;
	


			


			if (c1->sort()->domainType() == bcplus::DomainType::BOOLEAN) val = "o_true";
			else if (snc) { val = *newVar(c1->sort(), c); snc_1 = true; }
			else continue;

			for (sy::SymbolTable::const_iterator it2 = it; it2 != symtab()->end(sy::Symbol::Type::CONSTANT); ++it2) {
				_stmtcount++;

				bool snc_2 = false;
				u::ref_ptr<const sy::ConstantSymbol> c2 = (sy::ConstantSymbol const*)(it2->get());

				if (!(c2->constType() & sy::ConstantSymbol::Type::M_REGULAR_ACTIONS)) continue; 
				if (c1 == c2 && !c1->arity()) continue;
			
				// setup controlled arguments for the first constant
				args1->clear();
				for (sy::ConstantSymbol::const_iterator cit = c1->begin(); cit != c1->end(); cit++) {
					args1->push_back(*newVar(*cit, c));
				}

				// setup controlled arguments for second constant
				args2->clear();
				for (sy::ConstantSymbol::const_iterator cit = c2->begin(); cit != c2->end(); cit++) {
					args2->push_back(*newVar(*cit, c));
				}

				if (c2->sort()->domainType() == bcplus::DomainType::BOOLEAN) val2 = "o_true";
				else if (snc) { val2 = *newVar(c2->sort(), c); snc_2 = true; }
				else continue;

				tmpout << "<- ";
				translate_eq(c1, val, c, tmpout, args1);
				tmpout << " & ";
				translate_eq(c2, val2, c, tmpout, args2);
				if (snc_1) tmpout << " & " << val << " != o_none";
				if (snc_2) tmpout << " & " << val2 << " != o_none";

				if (c1 == c2) {
					bool first = true;
					tmpout << " & (";
					// same action constant, make sure the arguments differ
					ClauseList::iterator cit2 = args2->begin();
					for (ClauseList::iterator cit1 = args1->begin(); cit1 != args1->end(); ++cit1) {
						if (first) first = false;
						else tmpout << " | ";
						tmpout << *cit1 << " != " << *cit2;
						++cit2;
					}
					tmpout << ")";
				}


				HANDLE_CLAUSES_6(tmpout, true, true, true, " & ", true);							
				HANDLE_PRESTMTS;
				assertIPart(IPart::CUMULATIVE);
				config()->out() << tmpout.str();
				_stmtcount++;
				tmpout.str("");
			}
		}


	}



	size_t cmask = symtab()->cmask();
	std::ostream& out = config()->out();
	#define syct sy::ConstantSymbol::Type

	assertIPart(IPart::BASE);

	out																										<< std::endl
					<< "% -------------------------------------------------------------------------"		<< std::endl
					<< "% epilogue ----------------------------------------------------------------" 		<< std::endl
					<< "% -------------------------------------------------------------------------" 		<< std::endl
																											<< std::endl;



	if (config()->inputLanguage() != Configuration::Input::MVPF) {
		out << "query_label(query)."							<< std::endl;
	}


	if (config()->outputLanguage() != Configuration::Output::STATIC) {
		assertIPart(IPart::VOLATILE);
		out << "false <- " << *config()->ts(Configuration::TS::STATIC) <<" < minstep.";
	}


	out 																									<< std::endl 
					<< "% -------------------------------------------------------------------------" 		<< std::endl
																											<< std::endl;


	if (config()->outputLanguage() == Configuration::Output::STATIC
		&& config()->inputLanguage() != Configuration::Input::MVPF) {
													out << "#hide step/1." 									<< std::endl
														<< "#hide astep/1." 								<< std::endl;
	}

		if (cmask & syct::M_ADDITIVE)				out << "#hide additiveconst_action/1."					<< std::endl;
	


	if (_computed)									out << "#hide s_computed/1."								<< std::endl;

	if (config()->inputLanguage() != Configuration::Input::MVPF) {
		out << "#hide query_label/1."		<< std::endl;
	}


}



bool Translator::translate(bcplus::statements::Statement const* stmt) {


	_stmtcount++;
	config()->ostream(Verb::DETAIL) << "Got statement #" << _stmtcount << " of type: " << stmt->typeString() << std::endl;



	bool ret = true;
	std::stringstream tmpout;
	u::ref_ptr<StatementList> preStmts = new StatementList();
	u::ref_ptr<ClauseList> extraClauses = new ClauseList(); 
	u::ref_ptr<Context> context = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses, NULL, false);




	switch (stmt->type()) {
     case st::Statement::Type::INCLUDE:
     case st::Statement::Type::MACROS:
		// already handled.
		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::CONSTANTS:
		{

			config()->out() << std::endl 
							<< "% Constant Declarations -----------------------------------------------" << std::endl
							<< std::endl;


			u::ref_ptr<const st::ConstantDeclaration> decl = (st::ConstantDeclaration const*) stmt;
			u::ref_ptr<Context> c = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses);


			BOOST_FOREACH(sy::ConstantSymbol const* sym, *decl) {
				translateConstDeclaration(sym, c);

			}
			HANDLE_PRESTMTS;
		
			config()->out() << std::endl 
							<< "% ---------------------------------------------------------------------" << std::endl 
							<< std::endl;
		}
		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::OBJECTS:

		{
			config()->out() << std::endl
							<< "% Object Declarations -------------------------------------------------" << std::endl
							<< std::endl;

			u::ref_ptr<const st::ObjectDeclaration> decl = (st::ObjectDeclaration const*) stmt;
			u::ref_ptr<Context> c = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses);


			BOOST_FOREACH(st::ObjectDeclaration::Element const* bind, *decl) {

				// handle dynamic sort declarations
				translateSortDeclaration(bind->sort(), c);

				BOOST_FOREACH(sy::Symbol const* sym, *bind) {

					switch(sym->type()) {
					case sy::Symbol::Type::OBJECT:
						// handle dynamic sort declarations
						BOOST_FOREACH(sy::SortSymbol const* sort, *(sy::ObjectSymbol const*)sym) {
							translateSortDeclaration(sort, c);
						}

						ret = translateObjectDeclaration((sy::ObjectSymbol const*)sym, bind->sort(), c) && ret;	
						break;
					case sy::Symbol::Type::RANGE:
						ret = translateRangeDeclaration((sy::NumberRangeSymbol const*)sym, bind->sort(), c) && ret;	
						break;
					default:
						// not used
						break;
					}


				}
			}

			HANDLE_PRESTMTS;


			config()->out() << std::endl 
							<< "% ---------------------------------------------------------------------" << std::endl 
							<< std::endl;
		}
			


		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::SORTS:
		{
			config()->out() << std::endl
							<< "% Sort Declarations ---------------------------------------------------" << std::endl
							<< std::endl;

			u::ref_ptr<const st::SortDeclaration> decl = (st::SortDeclaration const*) stmt;
			u::ref_ptr<Context> c = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses);

			BOOST_FOREACH(sy::SortSymbol const* sort, *decl) {
				translateSortDeclaration(sort, c);
			}
			HANDLE_PRESTMTS;
			
			config()->out() << std::endl 
							<< "% ---------------------------------------------------------------------" << std::endl 
							<< std::endl;

		}
		break;
		
		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::VARIABLES:
		{
			config()->out() << std::endl 
							<< "% Variable Declarations -----------------------------------------------" << std::endl
							<< std::endl;

			u::ref_ptr<const st::VariableDeclaration> decl = (st::VariableDeclaration const*) stmt;
			u::ref_ptr<Context> c = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses);

			BOOST_FOREACH(sy::VariableSymbol const* var, *decl) {
				assertIPart(IPart::BASE);
				// handle dynamic sort declarations
				translateSortDeclaration(var->sort(), c);
				HANDLE_PRESTMTS;

				config()->out() << "#domain ";
				translate(var->sort(), config()->out());
				config()->out() << "(";
				translate(var, config()->out());
				config()->out() << ")." << std::endl;
			}
			
			config()->out() << std::endl 
							<< "% ---------------------------------------------------------------------" << std::endl 
							<< std::endl;


		}
		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::COMMENTS:

		{	
			u::ref_ptr<const st::CommentBlock> decl = (st::CommentBlock const*) stmt;

			BOOST_FOREACH(parser::Token const* cmt, *decl) {
				config()->out() << *cmt->str() << std::endl;
			}

		}
		break;
		
		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::F2LP:
		{	
			assertIPart(IPart::BASE);
			config()->out() << "% --------- begin F2LP Block --------- %" << std::endl; 
			config()->out() << *((st::F2LPBlock const*)stmt)->value()->str() << std::endl;
			config()->out() << "% --------- end F2LP Block --------- %" << std::endl; 

		}
		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::LUA:
		{	
			assertIPart(IPart::BASE);
			config()->out() << "% --------- begin LUA Block --------- %" << std::endl; 
			config()->out() << "#begin_lua" << std::endl; 
			config()->out() << *((st::LUABlock const*)stmt)->value()->str() << std::endl;
			config()->out() << "#end_lua." << std::endl; 
			config()->out() << "% --------- end LUA Block --------- %" << std::endl; 

		}
		break;

     case st::Statement::Type::PYTHON:
		{	
			assertIPart(IPart::BASE);
			config()->out() << "% --------- begin Python Block --------- %" << std::endl; 
			config()->out() << "#begin_python" << std::endl; 
			config()->out() << *((st::LUABlock const*)stmt)->value()->str() << std::endl;
			config()->out() << "#end_python." << std::endl; 
			config()->out() << "% --------- end LUA Block --------- %" << std::endl; 

		}
		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::ASP:
		{	
			assertIPart(IPart::BASE);
			config()->out() << "% --------- begin ASP Block --------- %" << std::endl; 
			config()->out() << "#begin_asp" << std::endl; 
			config()->out() << *((st::ASPBlock const*)stmt)->value()->str() << std::endl;
			config()->out() << "#end_asp." << std::endl; 
			config()->out() << "% --------- end ASP Block --------- %" << std::endl; 

		}
		break;
		
		// --------------------------------------------------------------------------------------------



#define HANDLE_SHOW_HIDE_STMT(type, showhide)																								\
			u::ref_ptr<const type> decl = (type const*) stmt;																				\
			u::ref_ptr<Context> cs = new Context(config(), Context::Position::AGGR, IPart::BASE, preStmts, extraClauses);								\
			u::ref_ptr<Context> cf = new Context(config(), Context::Position::AGGR, IPart::CUMULATIVE, preStmts, extraClauses);						\
			u::ref_ptr<Context> ca = new Context(config(), Context::Position::AGGR, IPart::CUMULATIVE, preStmts, extraClauses, NULL, false, config()->ts(Configuration::TS::ACTION));\
																																			\
			BOOST_FOREACH(el::AtomicFormula const* af, *decl) {																				\
																																			\
				switch (af->c()->symbol()->constType()) {																					\
				case sy::ConstantSymbol::Type::RIGID:																						\
					assertIPart(IPart::BASE);																								\
					if (translate(af, cs, tmpout)) {																						\
						HANDLE_PRESTMTS;																									\
						config()->out() << showhide << tmpout.str();																		\
						HANDLE_CLAUSES_5(config()->out(), true, true, true, " : ");																\
						tmpout.str("");																										\
					} else ret = false;																										\
					break;																													\
																																			\
				case sy::ConstantSymbol::Type::ADDITIVEFLUENT:																				\
				case sy::ConstantSymbol::Type::EXTERNALFLUENT:																				\
				case sy::ConstantSymbol::Type::INERTIALFLUENT:																				\
				case sy::ConstantSymbol::Type::SDFLUENT:																					\
				case sy::ConstantSymbol::Type::SIMPLEFLUENT:																				\
					assertIPart(IPart::BASE);																								\
					if (translate(af, cs, tmpout)) {																						\
						HANDLE_PRESTMTS;																									\
						config()->out() << showhide << tmpout.str();																		\
						HANDLE_CLAUSES_5(config()->out(), true, true, true, " : ");																\
						tmpout.str("");																										\
					} else ret = false;																										\
																																			\
					assertIPart(IPart::CUMULATIVE);																							\
					if (translate(af, cf, tmpout)) {																						\
						HANDLE_PRESTMTS;																									\
						config()->out() << showhide << tmpout.str();																		\
						HANDLE_CLAUSES_5(config()->out(), true, true, true, " : ");																\
						tmpout.str("");																										\
					} else ret = false;																										\
					break;																													\
																																			\
				case sy::ConstantSymbol::Type::ABACTION:																					\
				case sy::ConstantSymbol::Type::ACTION:																						\
				case sy::ConstantSymbol::Type::ADDITIVEACTION:																				\
				case sy::ConstantSymbol::Type::ATTRIBUTE:																					\
				case sy::ConstantSymbol::Type::EXTERNALACTION:																				\
				case sy::ConstantSymbol::Type::EXOGENOUSACTION:																				\
																																			\
					assertIPart(IPart::CUMULATIVE);																							\
					if (translate(af, ca, tmpout)) {																						\
						HANDLE_PRESTMTS;																									\
						config()->out() << showhide << tmpout.str();																		\
						HANDLE_CLAUSES_5(config()->out(), true, true, true, " : ");																\
						tmpout.str("");																										\
					} else ret = false;																										\
					break;																													\
				default: break;																												\
				}																															\
			}


     case st::Statement::Type::SHOW:
		{
			HANDLE_SHOW_HIDE_STMT(st::ShowStatement, "#show ");
		}
		break;


		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::SHOW_ALL:
		{

			u::ref_ptr<Context> bc = new Context(config(), Context::Position::AGGR, IPart::BASE, preStmts, extraClauses, NULL, true, config()->ts(Configuration::TS::ZERO));
			u::ref_ptr<Context> sc = new Context(config(), Context::Position::AGGR, IPart::BASE, preStmts, extraClauses, NULL, true, config()->ts(Configuration::TS::STATIC));
			u::ref_ptr<Context> ac = new Context(config(), Context::Position::AGGR, IPart::BASE, preStmts, extraClauses, NULL, true, config()->ts(Configuration::TS::ACTION));
			std::string var = "VAR";

			for (sy::SymbolTable::const_iterator it = symtab()->begin(sy::Symbol::Type::CONSTANT); it != symtab()->end(sy::Symbol::Type::CONSTANT); it++) {
				u::ref_ptr<const sy::ConstantSymbol> c = (sy::ConstantSymbol const*) it->get();
				if (c->constType() & sy::ConstantSymbol::Type::M_FLUENTS || c->constType() == sy::ConstantSymbol::Type::RIGID) {
					translate_eq(c, var, bc, tmpout);
					HANDLE_PRESTMTS;
					assertIPart(IPart::BASE);
					config()->out() << "#show ";
					config()->out() << tmpout.str();
					tmpout.str("");
					config()->out() << " : ";
					translate(c->sort(), config()->out());
					config()->out() << "(VAR)";
					HANDLE_CLAUSES_6(config()->out(), false, true, true, " : ", true);
				}
			}

			for (sy::SymbolTable::const_iterator it = symtab()->begin(sy::Symbol::Type::CONSTANT); it != symtab()->end(sy::Symbol::Type::CONSTANT); it++) {
				u::ref_ptr<const sy::ConstantSymbol> c = (sy::ConstantSymbol const*) it->get();
				bool action = c->constType() & sy::ConstantSymbol::Type::M_ACTIONS;
				if (action || c->constType() & sy::ConstantSymbol::Type::M_FLUENTS) {
					translate_eq(c, var, (action ? ac : sc), tmpout);
					HANDLE_PRESTMTS;
					assertIPart(IPart::CUMULATIVE);
					config()->out() << "#show ";
					config()->out() << tmpout.str();
					tmpout.str("");
					config()->out() << " : ";
					translate(c->sort(), config()->out());
					config()->out() << "(VAR)";
					HANDLE_CLAUSES_6(config()->out(), false, true, true, " : ", true);
				}
			}


		}
		break;
   
		// --------------------------------------------------------------------------------------------
 
	case st::Statement::Type::HIDE:
		{
			
			HANDLE_SHOW_HIDE_STMT(st::HideStatement, "#hide ");
		}
		break;
		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::HIDE_ALL:
		{
			config()->out() << "#hide." << std::endl;
/*
			assertIPart(IPart::BASE);
			config()->out() << "#show h(eql(C, V))        : rigid(C)  : constant_object(C, V)." << std::endl;
			config()->out() << "#show h(eql(C, V), 0)     : fluent(C) : constant_object(C, V)." << std::endl;
			assertIPart(IPart::CUMULATIVE);
			config()->out() << "#show h(eql(C, V), " << *config()->ts(Configuration::TS::STATIC) << ")     : fluent(C) : constant_object(C, V)." << std::endl;
			config()->out() << "#show occ(eql(C, V), " << *config()->ts(Configuration::TS::STATIC) << "-1) : action(C) : constant_object(C, V)." << std::endl;
*/
		}
		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::NOCONCURRENCY:
		{
			config()->out() << "% noconcurrency." << std::endl;
			_noconcurrency = true;
		}
		break;

		// --------------------------------------------------------------------------------------------

     case st::Statement::Type::STRONG_NOCONCURRENCY:
		{
			config()->out() << "% strong_noconcurrency." << std::endl;
			_strong_noconcurrency = true;
		}
		break;

		// --------------------------------------------------------------------------------------------
     case st::Statement::Type::MAXAFVALUE:
		{
			u::ref_ptr<const st::MaxAFValueStatement> s = (st::MaxAFValueStatement const*)stmt;
			assertIPart(IPart::BASE);
			config()->out() << "#const maxAdditive=" << s->value() << "." << std::endl;
			symtab()->setData("maxAdditive", new ReferencedString(boost::lexical_cast<std::string>(s->value())), true);
		}
		break;
     case st::Statement::Type::MAXADDITIVE:
		{
			u::ref_ptr<const st::MaxAdditiveStatement> s = (st::MaxAdditiveStatement const*)stmt;
			assertIPart(IPart::BASE);
			config()->out() << "#const maxAdditive=" << s->value() << "." << std::endl;
			symtab()->setData("maxAdditive", new ReferencedString(boost::lexical_cast<std::string>(s->value())), true);
		}
		break;

     case st::Statement::Type::QUERY:
		{	
			u::ref_ptr<const st::QueryStatement> q = (st::QueryStatement const*) stmt;
			u::ref_ptr<Context> c = new Context(config(), Context::Position::DECL, IPart::VOLATILE, preStmts, extraClauses, NULL, true);
			u::ref_ptr<Context> bc = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses, NULL, true);

			// metadata
			config()->out() << "% Query: " << *(q->symbol()->base()) << std::endl;
			config()->out() << "% Maxstep: ";
			if (q->minmax() >= 0) config()->out() << q->minmax();
			config()->out() << "..";
			if (q->maxmax() >= 0) config()->out() << q->maxmax();
			config()->out() << std::endl;

			// rules
			BOOST_FOREACH(el::Formula const* f, *q) {

				// put formulas of the form 0:F in the base part
				// TODO: Better differentiation
				if (f->subType() == el::Formula::Type::BINDING 
					&& ((el::BindingFormula const*)f)->step()->subType() == el::Term::Type::OBJECT
					&& ((el::Object const*)(((el::BindingFormula const*)f)->step()))->symbol() == symtab()->resolve(sy::Symbol::Type::OBJECT, "0")) {
					assertIPart(IPart::BASE);
					bindAndTranslate(f, bc, tmpout);




				} else {
					if (config()->outputLanguage() != Configuration::Output::STATIC && !(f->cmask() & bcplus::symbols::ConstantSymbol::Type::M_ACTIONS)) {
						assertIPart(IPart::BASE);
						bindAndTranslate(f, bc, tmpout);
					
						HANDLE_PRESTMTS
						config()->out() << "<- ";
						config()->out() << "not (" << tmpout.str();
						HANDLE_CLAUSES(config()->out(), false, false);
						config()->out() << ") & query_label(q_" << *(q->symbol()->base()) << ") & maxstep == 0." << std::endl;
						tmpout.str("");
					}
					
					assertIPart(IPart::VOLATILE);
					bindAndTranslate(f, c, tmpout);

				}
				

				HANDLE_PRESTMTS
				config()->out() << "<- ";
				config()->out() << "not (" << tmpout.str();
				HANDLE_CLAUSES(config()->out(), false, false);
				config()->out() << ") & query_label(q_" << *(q->symbol()->base()) << ")." << std::endl;
				tmpout.str("");
			}
		}
		break;




     case st::Statement::Type::LAW_BASIC:
		{
			u::ref_ptr<const st::BasicLaw> l = (st::BasicLaw const*)stmt;
			ret = translateCausalLaw(l->head(), l->ifbody(), l->ifcons(), l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_CAUSED:
		{
			u::ref_ptr<const st::CausedLaw> l = (st::CausedLaw const*)stmt;
			ret = translateCausalLaw(l->head(), l->ifbody(), l->ifcons(), l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_PCAUSED:
		{
			u::ref_ptr<const st::PossiblyCausedLaw> l = (st::PossiblyCausedLaw const*)stmt;
			u::ref_ptr<const el::Formula> newifcons;

			if (!l->ifcons()) newifcons = l->head();
			else {
				newifcons = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->ifcons(), l->head());
			}

			ret = translateCausalLaw(l->head(), l->ifbody(), newifcons, l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_IMPL:
		{
			u::ref_ptr<const st::ImplicationLaw> l = (st::ImplicationLaw const*)stmt;
			ret = translateCausalLaw(l->head(), l->body(), NULL, NULL, NULL, l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_CAUSES:
		{
			u::ref_ptr<const st::CausesLaw> l = (st::CausesLaw const*)stmt;

			// The body of the causes should contain an action constant
			if (l->body() && !(l->body()->cmask() & sy::ConstantSymbol::Type::M_ACTIONS)) {
				config()->error("The body of a \"causes\" law must be a single action \"a\" or atomic formula \"a=v\".", &l->body()->beginLoc());
				ret = false;
			} else {

				u::ref_ptr<const el::Formula> newafter;

				if (!l->body()) newafter = l->ifbody();
				else if (!l->ifbody()) newafter = l->body();
				else {
					newafter = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->body(), l->ifbody());
				}			

				ret = translateCausalLaw(l->head(), NULL, NULL, newafter, l->unless(), l->where(), preStmts, extraClauses, tmpout);
			}
		}
		break;

	 case st::Statement::Type::LAW_DECREMENTS:
		{
			u::ref_ptr<const st::DecrementsLaw> l = (st::DecrementsLaw const*)stmt;
			ret = translateIncrementalLaw(l->body(), l->head(), l->value(), l->ifbody(), l->unless(), l->where(), false, preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_INCREMENTS:
		{
			u::ref_ptr<const st::IncrementsLaw> l = (st::IncrementsLaw const*)stmt;
			ret = translateIncrementalLaw(l->body(), l->head(), l->value(), l->ifbody(), l->unless(), l->where(), true, preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_MCAUSE:
		{
			u::ref_ptr<const st::MayCauseLaw> l = (st::MayCauseLaw const*)stmt;

			// The body of the causes should contain an action constant
			if (l->body() && !(l->body()->cmask() & sy::ConstantSymbol::Type::M_ACTIONS)) {
				config()->error("The body of a \"may cause\" law must be a single action \"a\" or atomic formula \"a=v\".", &l->body()->beginLoc());
				ret = false;
			} else {

				u::ref_ptr<const el::Formula> newafter;

				if (!l->body()) newafter = l->ifbody();
				else if (!l->ifbody()) newafter = l->body();
				else {
					newafter = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->body(), l->ifbody());
				}			
				ret = translateCausalLaw(l->head(), NULL, l->head(), newafter, l->unless(), l->where(), preStmts, extraClauses, tmpout);
			}
		}
		break;

     case st::Statement::Type::LAW_ALWAYS:
		{
			u::ref_ptr<const st::AlwaysLaw> l = (st::AlwaysLaw const*)stmt;


			u::ref_ptr<const el::Formula> newhead  = new el::NullaryFormula(el::NullaryFormula::Operator::FALSE);
			u::ref_ptr<const el::Formula> newafter = new el::UnaryFormula(el::UnaryFormula::Operator::NOT, l->body(), l->body()->beginLoc(), l->body()->endLoc());
			if (l->after()) newafter = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->after(), newafter);

			ret = translateCausalLaw(newhead, NULL, NULL, newafter, l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_CONSTRAINT:
		{
			u::ref_ptr<const st::ConstraintLaw> l = (st::ConstraintLaw const*)stmt;
			if (l->body() && (l->body()->cmask() & sy::ConstantSymbol::Type::M_ACTIONS)) {
				config()->error("The body of a \"constraint\" law may not contain any actions. Consider using an \"always\" law instead.", &l->body()->beginLoc());
				ret = false;
			} else {
				u::ref_ptr<const el::Formula> newhead = new el::NullaryFormula(el::NullaryFormula::Operator::FALSE);
				u::ref_ptr<const el::Formula> newif;
				if (l->body()) newif = new el::UnaryFormula(el::UnaryFormula::Operator::NOT, l->body(), l->body()->beginLoc(), l->body()->endLoc());
				ret = translateCausalLaw(newhead, newif, NULL, l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);
			}
		}
		break;

     case st::Statement::Type::LAW_IMPOSSIBLE:
		{
			u::ref_ptr<const st::ImpossibleLaw> l = (st::ImpossibleLaw const*)stmt;


			u::ref_ptr<const el::Formula> newhead = new el::NullaryFormula(el::NullaryFormula::Operator::FALSE);

			ret = translateCausalLaw(newhead, l->body(), NULL, l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_NEVER:
		{
			u::ref_ptr<const st::NeverLaw> l = (st::NeverLaw const*)stmt;


			u::ref_ptr<const el::Formula> newhead = new el::NullaryFormula(el::NullaryFormula::Operator::FALSE);

			ret = translateCausalLaw(newhead, l->body(), NULL, l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_DEFAULT:
		{
			u::ref_ptr<const st::DefaultLaw> l = (st::DefaultLaw const*)stmt;
			u::ref_ptr<const el::Formula> newifcons;

			if (!l->ifcons()) newifcons = l->head();
			else newifcons = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->ifcons(), l->head());
			
			ret = translateCausalLaw(l->head(), l->ifbody(), newifcons, l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_EXOGENOUS:
		{
			u::ref_ptr<const st::ExogenousLaw> l = (st::ExogenousLaw const*)stmt;
			u::ref_ptr<const el::Formula> newhead;
			u::ref_ptr<const el::Formula> newifcons;


			ReferencedString const* varname = newVar(l->head()->symbol()->sort(), context);
			newhead = new el::AtomicFormula(l->head(), new el::AnonymousVariable(varname, l->head()->endLoc(), l->head()->endLoc()), l->head()->beginLoc(), l->head()->endLoc());

			if (!l->ifcons()) newifcons = newhead;
			else newifcons = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->ifcons(), newhead);

			ret = translateCausalLaw(newhead, l->ifbody(), newifcons, l->after(), l->unless(), l->where(), preStmts, extraClauses, tmpout);


		}
		break;

     case st::Statement::Type::LAW_INERTIAL:
		{
			u::ref_ptr<const st::InertialLaw> l = (st::InertialLaw const*)stmt;
			u::ref_ptr<const el::Formula> newhead;
			u::ref_ptr<const el::Formula> newifcons, newafter;


			ReferencedString const* varname = newVar(l->head()->symbol()->sort(), context);
			newhead = new el::AtomicFormula(l->head(), new el::AnonymousVariable(varname, l->head()->endLoc(), l->head()->endLoc()), l->head()->beginLoc(), l->head()->endLoc());

			if (!l->ifcons()) newifcons = newhead;
			else newifcons = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->ifcons(), newhead);
			
			if (!l->after()) newafter = newhead;
			else newafter = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->after(), newhead);


			ret = translateCausalLaw(newhead, l->ifbody(), newifcons, newafter, l->unless(), l->where(), preStmts, extraClauses, tmpout);
		}
		break;

     case st::Statement::Type::LAW_NONEXECUTABLE:
		{		
		u::ref_ptr<const st::NonexecutableLaw> l = (st::NonexecutableLaw const*)stmt;

			// The body of the causes should contain an action constant
			if (l->body() && (l->body()->cmask() & ~sy::ConstantSymbol::Type::M_ACTIONS)) {
				config()->error("The body of a \"nonexecutable\" law must be an action formula.", &l->body()->beginLoc());
				ret = false;
			} else {
			
				u::ref_ptr<const el::Formula> newhead = new el::NullaryFormula(el::NullaryFormula::Operator::FALSE);
				u::ref_ptr<const el::Formula> newafter;

				if (!l->body()) newafter = l->ifbody();
				else if (!l->ifbody()) newafter = l->body();
				else newafter = new el::BinaryFormula(el::BinaryFormula::Operator::AND, l->body(), l->ifbody());

				ret = translateCausalLaw(newhead, NULL, NULL, newafter, l->unless(), l->where(), preStmts, extraClauses, tmpout);
			}

		}
		break;

     case st::Statement::Type::LAW_RIGID:
		{
			u::ref_ptr<const st::RigidLaw> l = (st::RigidLaw const*)stmt;
			u::ref_ptr<const el::Formula> newhead;

			ReferencedString const* varname = newVar(l->head()->symbol()->sort(), context);
			newhead = new el::AtomicFormula(l->head(), new el::AnonymousVariable(varname, l->head()->endLoc(), l->head()->endLoc()), l->head()->beginLoc(), l->head()->endLoc());
			
			ret = translateCausalLaw(newhead, NULL, NULL, newhead, NULL, l->where(), preStmts, extraClauses, tmpout);

		}
		break;

     case st::Statement::Type::LAW_OBSERVED:
		{

			u::ref_ptr<const st::ObservedLaw> l = (st::ObservedLaw const*)stmt;
			u::ref_ptr<Context> c = new Context(config(), Context::Position::HEAD, IPart::EXTERNAL, preStmts, extraClauses, NULL, false,
				new ReferencedString(boost::lexical_cast<std::string>(l->at()->val())));


//			std::stringstream tmpout;
//			translate(l->at(), c, tmpout);
//			std::string step = tmpout.str();
//			assertIPart(IPart::EXTERNAL, &step);

			if (l->head()->c()->symbol()->constType() & ~ bcplus::symbols::ConstantSymbol::Type::M_EXTERNAL) {
				config()->error("Observational laws cannot contain non-external constants.", &l->head()->beginLoc());
				ret = false;
			} else {
				translate(l->head(),c, tmpout);
				HANDLE_PRESTMTS
				config()->out() << tmpout.str();
				HANDLE_CLAUSES(config()->out(), true, true);
			}
		}
		break;
     case st::Statement::Type::LAW_TEMPORAL_CONSTRAINT:
		{

			u::ref_ptr<const st::TemporalConstraintLaw> l = (st::TemporalConstraintLaw const*)stmt;
			u::ref_ptr<Context> c = new Context(config(), Context::Position::HEAD, IPart::EXTERNAL, preStmts, extraClauses, NULL, false,
				new ReferencedString(boost::lexical_cast<std::string>(l->at()->val())));


//			std::stringstream tmpout;
//			translate(l->at(), c, tmpout);
//			std::string step = tmpout.str();
//			assertIPart(IPart::EXTERNAL, &step);
						

			ret = bindAndTranslate(l->head(), c, tmpout) && ret;
				
			HANDLE_PRESTMTS
			config()->out() << "<- ";
			config()->out() << "not (" << tmpout.str();
			HANDLE_CLAUSES(config()->out(), false, false);
			config()->out() << ")." << std::endl;
			tmpout.str("");
		}
		break;

	default:

		config()->error("INTERNAL ERROR: Encountered unknown statement.");

	}

	config()->out() << std::endl;

	return ret;

}

void Translator::nextStmt(Context* c, std::string const& string, IPart::type ipart) {
	c->addPreStmt(string, ipart);
	_stmtcount++;
}
			

bool Translator::translateCausalLaw(el::Formula const* head, el::Formula const* ifbody, el::Formula const* ifcons, el::Formula const* after, el::AtomicFormula const* unless, el::Formula const* where, StatementList* preStmts, ClauseList* extraClauses, std::stringstream& tmpout) {

	

	/// The various types of law that this can be
	struct LawType {
		enum type {
			STATIC,
			ACTION_DYNAMIC,
			FLUENT_DYNAMIC,
			RIGID
		};
	};

	LawType::type lt = LawType::STATIC;

	// Ensure that forbidden constants aren't in the head of this law
	// These are: incremental and external constants
	if (head->cmask() & sy::ConstantSymbol::Type::M_ADDITIVE) {
		config()->error("Malformed law: Additive constants can only occur in the heads of \"increments\" laws.", &head->beginLoc());
		return false;
	}
	
	if (head->cmask() & sy::ConstantSymbol::Type::M_EXTERNAL) {
		config()->error("Malformed law: External constants cannot occur in the heads of any law within the main program.", &head->beginLoc());
		return false;
	}



	// figure out what type of law this is...

	// rigid
	if (head->cmask() & sy::ConstantSymbol::Type::RIGID) {
		lt = LawType::RIGID;	
	}

	// rigid constraints
	else if (
			!(head->cmask() & ~sy::ConstantSymbol::Type::RIGID)
			&& (!ifbody || !(ifbody->cmask() & ~sy::ConstantSymbol::Type::RIGID))
			&& (!ifcons || !(ifcons->cmask() & ~sy::ConstantSymbol::Type::RIGID))
			&& (!where  || !(where->cmask() & ~sy::ConstantSymbol::Type::RIGID))
			&& !after && !unless) {
		lt = LawType::RIGID;
	}

	// fluent dynamic
	else if (!(head->cmask() & sy::ConstantSymbol::Type::M_ACTIONS) && (after || unless)) {
		lt = LawType::FLUENT_DYNAMIC;
	}

	// action dynamic
	else if (head->cmask() & sy::ConstantSymbol::Type::M_ACTIONS) {
		lt = LawType::ACTION_DYNAMIC;
	}



	// Ensure integrity of the law according to law type
	switch (lt) {
	case LawType::RIGID:

		// Cannot contain an 'after' or 'unless' clause
		if (after || unless) {
			config()->error("Malformed law: Rigid constants cannot occur in the head of dynamic laws.", &head->beginLoc());
			return false;
		}
	

		// Cannot contain non-rigid constants in any other clause
		if (
			(ifbody && (ifbody->cmask() & ~sy::ConstantSymbol::Type::RIGID))
			|| (ifcons && (ifcons->cmask() & ~sy::ConstantSymbol::Type::RIGID))
			|| (where && (where->cmask() & ~sy::ConstantSymbol::Type::RIGID))
		) {
			config()->error("Malformed law: Rigid constants cannot occur in the head of a law containing non-rigid constants.", &head->beginLoc());
			return false;
		}

		break;
	case LawType::ACTION_DYNAMIC:
		
		// Cannot contain an 'after' or 'unless' clause
		if (after || unless) {
			config()->error("Malformed law: Action dynamic laws cannot contain \"after\" or \"unless\" clauses.", &head->beginLoc());
			return false;
		}

		break;

	case LawType::STATIC:
		// actions cannot occur in the if or ifcons body
		if (
				(ifbody && (ifbody->cmask() & sy::ConstantSymbol::Type::M_ACTIONS))
				|| (ifcons && (ifcons->cmask() & sy::ConstantSymbol::Type::M_ACTIONS))
			) {
			config()->error("Actions cannot occur in the \"if\" or \"ifcons\" clauses of static laws.", &head->beginLoc());
			return false;
		}
		break;

	case LawType::FLUENT_DYNAMIC:	
		// actions cannot occur in the if or ifcons body
		if (
				(ifbody && (ifbody->cmask() & sy::ConstantSymbol::Type::M_ACTIONS))
				|| (ifcons && (ifcons->cmask() & sy::ConstantSymbol::Type::M_ACTIONS))
			) {
			config()->error("Actions cannot occur in the \"if\" or \"ifcons\" clauses of fluent dynamic laws.", &head->beginLoc());
			return false;
		}
		break;
	}

	if (unless) {

		// ensure that the unless clause contains abActions only
		if (unless->cmask() & ~sy::ConstantSymbol::Type::ABACTION) {
			config()->error("The \"unless\" clause must only contain abActions.", &head->beginLoc());
			return false;
		}


		u::ref_ptr<Context> sc = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses);

		for (el::Element::ConstantSet::const_iterator it = unless->beginConstants(); it != unless->endConstants(); it++) {
			translateConstDeclaration(*it, sc);
		}
	}

#define TRANS_CLAUSE(out, hasbody, clause, context, notnot) 									\
	if (clause) {																				\
		if (hasbody) out << " & ";																\
		hasbody = true;																			\
		if (notnot) {																			\
			out << "not not ( ";																\
			bindAndTranslate(clause, context, out);												\
			out << ")";																			\
		} else {																				\
			translate(clause, context, out);													\
		}																						\
		HANDLE_CLAUSES(out, false, false);														\
}

#define TRANSLATE(head, ifbody, ifcons, after, unless, where, hc, bc, nc, ac, tmpout)	\
	/* translate the head */															\
	translate(head, hc, tmpout);														\
																						\
	/* everything else */																\
	if (ifbody || ifcons || after || unless || where) {									\
		tmpout << " <- ";																\
		bool hasbody = false;															\
																						\
		TRANS_CLAUSE(tmpout, hasbody, ifbody, bc, (config()->inputLanguage() == Configuration::Input::CPLUS && head->cmask()));\
		TRANS_CLAUSE(tmpout, hasbody, ifcons, nc, head->cmask());						\
		TRANS_CLAUSE(tmpout, hasbody, after, ac, false);								\
																						\
		TRANS_CLAUSE(tmpout, hasbody, unless, ac, false);								\
		TRANS_CLAUSE(tmpout, hasbody, where, bc, false);								\
	}																					\
	tmpout << "." << std::endl;															\
																						\
	HANDLE_PRESTMTS;																	\
	config()->out() << tmpout.str();

	// Figure out the right timesteps to use...
	u::ref_ptr<const ReferencedString> ts;
	if (head->cmask() & sy::ConstantSymbol::Type::M_ACTIONS) {
		ts = config()->ts(Configuration::TS::ACTION);
	} else if (after) {
		ts = config()->ts(Configuration::TS::DYNAMIC);
	} else {
		ts = config()->ts(Configuration::TS::STATIC);
	}
	

	// translate base
	if (lt == LawType::RIGID || lt == LawType::STATIC) {
		u::ref_ptr<Context> hc = new Context(config(), Context::Position::HEAD, IPart::BASE, preStmts, extraClauses);
		u::ref_ptr<Context> bc = new Context(config(), Context::Position::BODY, IPart::BASE, preStmts, extraClauses, NULL, !head->cmask());
		u::ref_ptr<Context> nc = new Context(config(), Context::Position::BODY, IPart::BASE, preStmts, extraClauses, NULL, true);

		assertIPart(IPart::BASE);
		TRANSLATE(head, ifbody, ifcons, after, unless, where, hc, bc, nc, NULL, tmpout);
	}


	// translate cumulative
	tmpout.str("");
	if (lt == LawType::STATIC || lt == LawType::ACTION_DYNAMIC || lt == LawType::FLUENT_DYNAMIC) {
		u::ref_ptr<Context> hc = new Context(config(), Context::Position::HEAD, IPart::CUMULATIVE, preStmts, extraClauses, NULL, false, ts);
		u::ref_ptr<Context> bc = new Context(config(), Context::Position::BODY, IPart::CUMULATIVE, preStmts, extraClauses, NULL, !head->cmask(), ts);
		u::ref_ptr<Context> nc = new Context(config(), Context::Position::BODY, IPart::CUMULATIVE, preStmts, extraClauses, NULL, true, ts);
		u::ref_ptr<Context> ac = new Context(config(), Context::Position::BODY, IPart::CUMULATIVE, preStmts, extraClauses, NULL, true, config()->ts(Configuration::TS::ACTION));

		assertIPart(IPart::CUMULATIVE);
		TRANSLATE(head, ifbody, ifcons, after, unless, where, hc, bc, nc, ac, tmpout);
	}


	// determine what kind of law this is

	return true;
}

bool Translator::translateIncrementalLaw(el::AtomicFormula const* body, el::Constant const* head, el::Term const* value, el::Formula const* ifbody, el::AtomicFormula const* unless, el::Formula const* where, bool positive, StatementList* preStmts, ClauseList* extraClauses, std::stringstream& tmpout) {
	assertIPart(IPart::CUMULATIVE);

	bool good = true;

	// The body should be an action
	if (body->cmask() & ~sy::ConstantSymbol::Type::M_ACTIONS) {
		config()->error("The body of an additive law (F in \"F " + (positive ? std::string("increments") : std::string("decrements")) + " G\") must only contain action constants.", &body->beginLoc());
		good = false;
	}

	// The head should be an additive constant
	if (head->cmask() & ~sy::ConstantSymbol::Type::M_ADDITIVE) {
		config()->error("The head of an additive law (G in \"F " + (positive ? std::string("increments") : std::string("decrements")) + " G\") must be an additive constant (additiveFluent or additiveAction).", &head->beginLoc());
		good=false;
	}

	// unless body may only contain abActions
	if (unless && (unless->cmask() & ~sy::ConstantSymbol::Type::ABACTION)) {
		config()->error("The \"unless\" clause must only contain abActions.", &head->beginLoc());
		good=false;
	}

	if (!good) return false;

	u::ref_ptr<Context> aac = new Context(config(), Context::Position::HEAD, IPart::BASE, preStmts, extraClauses, NULL, false, config()->ts(Configuration::TS::ZERO));
	u::ref_ptr<Context> hc = new Context(config(), Context::Position::HEAD, IPart::CUMULATIVE, preStmts, extraClauses, NULL, false, config()->ts(Configuration::TS::ACTION));
	u::ref_ptr<Context> bc = new Context(config(), Context::Position::BODY, IPart::CUMULATIVE, preStmts, extraClauses, NULL, false, config()->ts(Configuration::TS::ACTION));


	// register the additive constant's dependency on this action
	tmpout << "additiveconst_action(";
	translate(head->symbol(), aac, tmpout, false);
	// TOTOTOTO
	HANDLE_ARGS(head, aac, tmpout, true, false);
	tmpout << ", ";
	translate(body->c()->symbol(), aac, tmpout, false);
	HANDLE_ARGS(body->c(), aac, tmpout, true, false);
	tmpout << ")";
	HANDLE_CLAUSES(tmpout, true, true);
	aac->addPreStmt(tmpout.str(), IPart::BASE);
	tmpout.str("");



	// translate the head of the incremental law
	translate_contrib(body, head, value, positive, hc, tmpout);
	tmpout << " <- ";

	bool hasbody = false;	
	TRANS_CLAUSE(tmpout, hasbody, body, bc, false);
	TRANS_CLAUSE(tmpout, hasbody, ifbody, bc, false);

	if (unless) {
		

		u::ref_ptr<Context> sc = new Context(config(), Context::Position::DECL, IPart::BASE, preStmts, extraClauses);

		for (el::Element::ConstantSet::const_iterator it = unless->beginConstants(); it != unless->endConstants(); it++) {
			translateConstDeclaration(*it, sc);
		}
	}

	TRANS_CLAUSE(tmpout, hasbody, unless, bc, false);
	TRANS_CLAUSE(tmpout, hasbody, where, bc, false);
	tmpout << "." << std::endl;

	HANDLE_PRESTMTS;
	config()->out() << tmpout.str();

	return true;
}

bool Translator::translateConstDeclaration(sy::ConstantSymbol const* sym, Context* superc) {
	bool ret = true;
	ConstantData* md = sym->metadata<ConstantData>();

	u::ref_ptr<ClauseList> extraClauses = new ClauseList();
	u::ref_ptr<Context> c = superc->mkBinds(extraClauses, NULL);

	if (md->translated()) return true;
	md->translated(true);
		
	// handle dynamic sort declarations
	translateSortDeclaration(sym->sort(), c);
	BOOST_FOREACH(sy::SortSymbol const* sort, *sym) {
		translateSortDeclaration(sort, c);
	}




	// translate
	std::stringstream tmpout;


	ret = translate(sym, c, tmpout, true) && ret;
	std::string trans_c = tmpout.str();
	tmpout.str("");

	HANDLE_CLAUSES_4(tmpout, true, true, false);
	std::string clauses = tmpout.str();
	tmpout.str("");

	// BC assumes actions are exogenous
//	if (sym->constType() == sy::ConstantSymbol::Type::ACTION && config()->inputLanguage() == Configuration::Input::BC)
//		tmpout << sy::ConstantSymbol::Type::cstr(sy::ConstantSymbol::Type::EXOGENOUSACTION);
//	else 
//		tmpout << sym->constTypeString();


	// <constType>(<constant>)
//	tmpout << "(" << trans_c << ")" << clauses;
//	if (ret) {
//		nextStmt(c, tmpout.str(), IPart::BASE);
//	}
//	tmpout.str("");


	// constant_sort(<constant>, <sort>)
//	tmpout << "constant_sort(" << trans_c << ", ";
//	translate(sym->sort(), tmpout);
//	tmpout << ")" << clauses;
//	if (ret) {
//		nextStmt(c, tmpout.str(), IPart::BASE);
//	}
//	tmpout.str("");




	// UEC for the constants...
		
	const bool action = sym->constType() & sy::ConstantSymbol::Type::M_ACTIONS;
	const bool fluent = sym->constType() & sy::ConstantSymbol::Type::M_FLUENTS;
	const bool rigid  = sym->constType() & sy::ConstantSymbol::Type::RIGID;
	const bool additive  = sym->constType() & sy::ConstantSymbol::Type::M_ADDITIVE;

	if (fluent || rigid) {
		ReferencedString const* newvar = newVar(sym->sort(), c, false);
		u::ref_ptr<Context> bc = c->mkPos(Context::Position::AGGR, IPart::BASE, true);
		bc = bc->mkTime(config()->ts(Configuration::TS::ZERO));
		
		tmpout << "<- not 1{ ";
		ret = translate_eq(sym, *newvar, bc, tmpout) && ret;
		tmpout << " : ";
		translate(sym->sort(), tmpout);
		tmpout << "(" << *newvar << ")}1";
		HANDLE_CLAUSES_6(tmpout, true, true, false, " & ", true);

		if (ret) {
			nextStmt(c, tmpout.str(), IPart::BASE);
		}
		tmpout.str("");
	}


	if (!rigid) {
		ReferencedString const* newvar = newVar(sym->sort(), c, false);
		u::ref_ptr<Context> ic = c->mkPos(Context::Position::AGGR, IPart::CUMULATIVE, true);

		if (action) {
			ic = ic->mkTime(config()->ts(Configuration::TS::ACTION));
		} else {
			ic = ic->mkTime(config()->ts(Configuration::TS::DYNAMIC));
		}
		
		tmpout << "<- not 1{ ";
		ret = translate_eq(sym, *newvar, ic, tmpout) && ret;
		tmpout << " : ";
		translate(sym->sort(), tmpout);
		tmpout << "(" << *newvar << ")}1";
		HANDLE_CLAUSES_6(tmpout, true, true, false, " & ", true);

		if (ret) {
			nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
		}
		tmpout.str("");
	}

	// All actions in BC are exogenous...
	sy::ConstantSymbol::Type::type symtype = sym->constType();
	if (config()->inputLanguage() == Configuration::Input::BC &&
			(sym->constType() & sy::ConstantSymbol::Type::M_ACTIONS)) {
		symtype = sy::ConstantSymbol::Type::EXOGENOUSACTION;
	}


	// special handling for various types...
	switch (symtype) {

	case sy::ConstantSymbol::Type::ABACTION:
		{
			// abActions default to false (or none)...
			
			u::ref_ptr<Context> ic = c->mkPos(Context::Position::AGGR, IPart::CUMULATIVE, true);
			ic = ic->mkTime(config()->ts(Configuration::TS::ACTION));


			std::string val;
			if (sym->sort()->domainType() == bcplus::DomainType::BOOLEAN) {
				val = "o_false";
			} else {
				val = "o_none";
			}


			tmpout << "{";
			ret = translate_eq(sym, val, ic, tmpout) && ret;
			tmpout << "}";
			HANDLE_CLAUSES_4(tmpout, true, true, false);
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			}
			tmpout.str("");

			break;
		}
	
	case sy::ConstantSymbol::Type::EXTERNALFLUENT:
	case sy::ConstantSymbol::Type::EXTERNALACTION:
		{
			// external constants default to unknown


			u::ref_ptr<Context> ic = c->mkPos(Context::Position::AGGR, IPart::CUMULATIVE, false);
			ic = ic->mkTime(config()->ts((action ? Configuration::TS::ACTION : Configuration::TS::STATIC)));


			tmpout << "#external ";



			ret = translate_eq(sym, *newVar(sym->sort(), ic), ic, tmpout) && ret;
			HANDLE_CLAUSES_5(tmpout, false, true, false, " : ");
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			}
			tmpout.str("");

			tmpout << "{";
			ret = translate_eq(sym, "o_unknown", ic, tmpout) && ret;
			tmpout << "}";
			HANDLE_CLAUSES_4(tmpout, true, true, false);
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			}
			tmpout.str("");

			if (!action) {

				ic = c->mkPos(Context::Position::AGGR, IPart::BASE, false);
				ic = ic->mkTime(config()->ts(Configuration::TS::ZERO));
			
				tmpout << "#external ";
				ret = translate_eq(sym, *newVar(sym->sort(), ic), ic, tmpout) && ret;
				HANDLE_CLAUSES_5(tmpout, false, true, false, " : ");
				if (ret) {
					nextStmt(c, tmpout.str(), IPart::BASE);
				}

				tmpout << "{";
				ret = translate_eq(sym, "o_unknown", ic, tmpout) && ret;
				tmpout << "}";
				HANDLE_CLAUSES_4(tmpout, true, true, false);
				if (ret) {
					nextStmt(c, tmpout.str(), IPart::BASE);
				}
				tmpout.str("");
			}
			break;
		}
	case sy::ConstantSymbol::Type::ATTRIBUTE:

		{
				// attributes should be something that isn't "none" iff their parent is asserted.
				// TODO: attribute: Verify both are actions.

				u::ref_ptr<Context> ic = c->mkPos(Context::Position::AGGR, IPart::CUMULATIVE, true);
				ic = ic->mkTime(config()->ts(Configuration::TS::ACTION));

				sy::ConstantSymbol const* parent = ((sy::AttributeSymbol const*)sym)->attribOf();


				// setup shared variables for the parent and attribute
				u::ref_ptr<ClauseList> args = new ClauseList();
					
				BOOST_FOREACH(sy::SortSymbol const* sort, *parent) {
					args->push_back(*newVar(sort, ic));
				}

				// TODO: Use objects instead of string constants
				// figure out the right values to use
				std::string val = "o_none";
				std::string pval;

				if (parent->sort()->domainType() == bcplus::DomainType::BOOLEAN) {
					pval = "o_false";
				} else {
					pval = "o_none";
				}

				tmpout << "false <- 1{";
				ret = translate_eq(sym, val, ic, tmpout, args) && ret;
				tmpout << ", ";
				ret = translate_eq(parent, pval, ic, tmpout, args) && ret;
				tmpout << "}1";
				HANDLE_CLAUSES_4(tmpout, true, true, false);

				if (ret) {
					nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
				}
				tmpout.str("");
				/* no break */
		}
	case sy::ConstantSymbol::Type::EXOGENOUSACTION:
		{
			u::ref_ptr<Context> ic = c->mkPos(Context::Position::AGGR, IPart::CUMULATIVE, false);
			ic = ic->mkTime(config()->ts(Configuration::TS::ACTION));

			tmpout << "{";
			ret = translate_eq(sym, *newVar(sym->sort(), ic), ic, tmpout) && ret;
			tmpout << "}";
			HANDLE_CLAUSES_4(tmpout, true, true, false);
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			}
			tmpout.str("");
			break;
		}
	case sy::ConstantSymbol::Type::ADDITIVEACTION:
	case sy::ConstantSymbol::Type::ADDITIVEFLUENT:
		{
			// hide contributions...
			tmpout << "#hide contrib_" << *sym->base() << "/4.";
			nextStmt(c, tmpout.str(), IPart::NONE);
			tmpout.str("");


			// UEC and default for contributions...
			u::ref_ptr<ClauseList> args = new ClauseList();
			BOOST_FOREACH(sy::SortSymbol const* sort, *sym) {
				args->push_back(*newVar(sort, c));
			}

			tmpout << "{contrib_" << *sym->base() << "(";
			HANDLE_SORT_ARGS_6(sym, c, tmpout, false, true, args);
			tmpout << "ACTION, 0, " << *config()->ts(Configuration::TS::ACTION) << ")}";
			tmpout << " <- additiveconst_action(";
			translate((sy::ConstantSymbol const*)sym, c, tmpout, true, args);
			tmpout << ", ACTION)";

			HANDLE_CLAUSES_6(tmpout, false, true, false, " & ", true);
			nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			tmpout.str("");
			
			tmpout << "<- not 1{contrib_" << *sym->base() << "(";
			HANDLE_SORT_ARGS_6(sym, c, tmpout, false, true, args);
			tmpout << "ACTION, VAR, " << *config()->ts(Configuration::TS::ACTION) << ") : ";
			translate(symtab()->bsort(sy::SymbolTable::BuiltinSort::ADDITIVE), tmpout);
			tmpout << "(VAR) }1 & additiveconst_action(";
			translate((sy::ConstantSymbol const*)sym, c, tmpout, true, args);
			tmpout << ", ACTION)";

			HANDLE_CLAUSES_6(tmpout, false, true, false, " & ", true);
			nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			tmpout.str("");

			args = new ClauseList();
			BOOST_FOREACH(sy::SortSymbol const* sort, *sym) {
				args->push_back(*newVar(sort, c));
			}

			u::ref_ptr<Context> hc = c->mkPos(Context::Position::HEAD, IPart::CUMULATIVE, false);
			hc = hc->mkTime((action ? config()->ts(Configuration::TS::ACTION) : config()->ts(Configuration::TS::DYNAMIC)));

			std::string var;
			if (fluent) var = "VAR1+VAR2";
			else var = "VAR1";

			translate_eq(sym, var, hc, tmpout, args);
			tmpout << " <- VAR1 = #sum[ contrib_" << *sym->base() << "(";
			HANDLE_SORT_ARGS_6(sym, c, tmpout, false, true, args);
//			tmpout << "ACTION, VAR3, " << *config()->ts(Configuration::TS::ACTION) << ") : action(ACTION) : ";
			tmpout << "ACTION, VAR3, " << *config()->ts(Configuration::TS::ACTION) << ") : additiveconst_action(";
			translate((sy::ConstantSymbol const*)sym, c, tmpout, true, args);
			tmpout << ", ACTION) : ";
			
			translate(symtab()->bsort(sy::SymbolTable::BuiltinSort::ADDITIVE), tmpout);
			tmpout << "(VAR3) = VAR3 ]";

			// inertia...
			if (fluent) {
				u::ref_ptr<Context> ac = hc->mkTime(config()->ts(Configuration::TS::ACTION));
				tmpout << " & ";
				translate_eq(sym, "VAR2", ac, tmpout, args);
			}


			HANDLE_CLAUSES_6(tmpout, false, true, false, " & ", true);
			nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			tmpout.str("");

			break;
		}
	case sy::ConstantSymbol::Type::INERTIALFLUENT:
		{
			//  inertial fluents are inertial...
			
			// generate a uniform argument list
			u::ref_ptr<ClauseList> args = new ClauseList();
			BOOST_FOREACH(sy::SortSymbol const* sort, *sym) {
				args->push_back(*newVar(sort, c));
			}
			
			u::ref_ptr<Context> hc = c->mkPos(Context::Position::AGGR, IPart::CUMULATIVE, false);
			hc = hc->mkTime(config()->ts(Configuration::TS::DYNAMIC));
			
			u::ref_ptr<Context> bc = c->mkPos(Context::Position::AGGR, IPart::CUMULATIVE, false);
			bc = bc->mkTime(config()->ts(Configuration::TS::ACTION));

			std::string var = *newVar(sym->sort(), c);

			tmpout << "{";
			ret = translate_eq(sym, var, hc, tmpout, args) && ret;
			tmpout << "}";
			tmpout << " <- ";
			ret = translate_eq(sym, var, bc, tmpout, args) && ret;
			HANDLE_CLAUSES_4(tmpout, false, true, false);
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			}
			tmpout.str("");

		}
		break;
	default:
		break;
	}

	if (sym->constType() & sy::ConstantSymbol::Type::M_SIMPLEFLUENTS) {
			// simple fluents are exogenous at 0
			u::ref_ptr<Context> ic = c->mkPos(Context::Position::AGGR, IPart::BASE, false);
			ic = ic->mkTime(config()->ts(Configuration::TS::ZERO));

			tmpout << "{";
			ret = translate_eq(sym, *newVar(sym->sort(), ic), ic, tmpout) && ret;
			tmpout << "}";
			HANDLE_CLAUSES_4(tmpout, true, true, false);
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::BASE);
			}
			tmpout.str("");
	}





	// Constant domain restriction (if applicable)
	if ((!additive && config()->enforceDomain()) || (additive && config()->enforceAdditiveDomain())) {


		u::ref_ptr<ClauseList> args = new ClauseList();
		BOOST_FOREACH(sy::SortSymbol const* sort, *sym) {
			args->push_back(*newVar(sort, c));
		}

		if (fluent || rigid) {
			u::ref_ptr<Context> ic = c->mkPos(Context::Position::BODY, IPart::BASE, true);
			ic = ic->mkTime(config()->ts(Configuration::TS::ZERO));

			tmpout << "false <- ";
			ret = translate_eq(sym, "VAR", ic, tmpout, args) && ret;
//			tmpout << " & not constant_object(";
//			translate(sym, c, tmpout, false);
//			HANDLE_SORT_ARGS_6(sym, c, tmpout, true, false, args);
//			tmpout << ", VAR)";
			tmpout << " & not ";
			translate(sym->sort(), tmpout);
			tmpout << "(VAR)";
			HANDLE_CLAUSES_6(tmpout, false, true, false, " & ", true);
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::BASE);
			}
			tmpout.str("");
		}

		if (fluent || action) {
			u::ref_ptr<Context> ic = c->mkPos(Context::Position::BODY, IPart::BASE, true);
			ic = ic->mkTime(config()->ts(fluent ? Configuration::TS::STATIC : Configuration::TS::ACTION));


			tmpout << "false <- ";
			ret = translate_eq(sym, "VAR", ic, tmpout, args) && ret;
//			tmpout << " & not constant_object(";
//			translate(sym, c, tmpout, true, args);
//			tmpout << ", VAR)";
			tmpout << " &  not ";
			translate(sym->sort(), tmpout);
			tmpout << "(VAR)";

			HANDLE_CLAUSES_6(tmpout, false, true, false, " & ", true);
			if (ret) {
				nextStmt(c, tmpout.str(), IPart::CUMULATIVE);
			}
			tmpout.str("");
		}
	}


	return ret;
}

bool Translator::translate(sy::NumberRangeSymbol const* r, Context* c, std::ostream& out) {
	bool ret = true;
	out << "(";
	ret = translate(r->min(), c, out) && ret;
	out << ")..(";
	ret = translate(r->max(), c, out) && ret;
	out << ")";
	return ret;

}

bool Translator::translateObjectDeclaration(sy::ObjectSymbol const* obj, sy::SortSymbol const* sort, Context* c) {
	bool ret = true;	
	std::stringstream tmpout;
	u::ref_ptr<ClauseList> extraClauses = new ClauseList();
	u::ref_ptr<Context> subc = c->mkBinds(extraClauses, NULL);

	ObjectData* md = obj->metadata<ObjectData>();

	if (translate(obj, subc, tmpout, true)) {
		assertIPart(IPart::BASE);

		std::string obj = tmpout.str();
		tmpout.str("");
		HANDLE_CLAUSES_4(tmpout, true, true, false);
		std::string clauses = tmpout.str();
		tmpout.str("");

		// object(<object>)
//		if (!md->translated()) {
//			md->translated(true);
//			tmpout << "object(" << obj << ").";
//			nextStmt(subc, tmpout.str(), IPart::BASE);
//			tmpout.str("");
//		}


		if (!md->translatedSort(sort)) {

			md->addTranslatedSort(sort);

			// <sort>(<object>)
			translate(sort, tmpout);
			tmpout << "(" << obj << ")" << clauses;
			nextStmt(subc, tmpout.str(), IPart::BASE);
			tmpout.str("");			
		

		}
						
	} else ret = false;

	return ret;

}

bool Translator::translateRangeDeclaration(sy::NumberRangeSymbol const* range, sy::SortSymbol const* sort, Context* c) {
	bool ret = true;	
	std::stringstream tmpout;
	u::ref_ptr<ClauseList> extraClauses = new ClauseList();
	u::ref_ptr<Context> subc = c->mkBinds(extraClauses, NULL);

	RangeData* md = range->metadata<RangeData>();


	bool good = translate(range->min(), subc, tmpout);
	tmpout << "..";
	good = translate(range->max(), subc, tmpout) && good;

	if (good) {
		assertIPart(IPart::BASE);

		std::string obj = tmpout.str();
		tmpout.str("");
		HANDLE_CLAUSES_4(tmpout, true, true, false);
		std::string clauses = tmpout.str();
		tmpout.str("");

		// object(<range>)
//		if (!md->translated()) {
//			md->translated(true);
//			tmpout << "object(" << obj << ").";
//			nextStmt(subc, tmpout.str(), IPart::BASE);
//			tmpout.str("");
//		}


		if (!md->translatedSort(sort)) {

			md->addTranslatedSort(sort);

			// <sort>(<range>)
			translate(sort, tmpout);
			tmpout << "(" << obj << ")" << clauses;
			nextStmt(subc, tmpout.str(), IPart::BASE);
			tmpout.str("");			
		

		}
						
	} else ret = false;

	return ret;

}


bool Translator::translateSortDeclaration(sy::SortSymbol const* sort, Context* c) {
		bool changed = false;
		SortData* md = sort->metadata<SortData>();
		std::stringstream tmpout;

		translate(sort, tmpout);
		std::string sortname = tmpout.str();
		tmpout.str("");


		if (!md->translated()) {
//			tmpout << "sort(";
//			translate(sort, tmpout);
//			tmpout << ").";
//			nextStmt(c, tmpout.str(), IPart::BASE);
//			tmpout.str("");

//			tmpout << "sort_element(" << sortname << ", VAR) <- " << sortname << "(VAR).";
//			nextStmt(c, tmpout.str(), IPart::BASE);
//			tmpout.str("");

			changed = true;
			md->translated(true);

			// translate objects (if it has any at the moment...)
			u::ref_ptr<ClauseList> extraClauses = new ClauseList();
			u::ref_ptr<Context> sc = c->mkBinds(extraClauses, NULL);
			BOOST_FOREACH(sy::ObjectSymbol const* obj, *sort) {
				translateObjectDeclaration(obj, sort, c);

			}
	
			for (sy::SortSymbol::RangeList::const_iterator it = sort->beginRanges(); 
					it != sort->endRanges(); it++) {
				translateRangeDeclaration((sy::NumberRangeSymbol const*)it->get(), sort, c);

			}



			// sort_object(<sort>, <object>)
//			tmpout << "sort_object(";
//			translate(sort, tmpout);
//			tmpout << ", VAR) <- ";
//			translate(sort, tmpout);
//			tmpout << "(VAR).";
//			nextStmt(c, tmpout.str(), IPart::BASE);
//			tmpout.str("");
	
			// Hide the extent of this sort...
			tmpout << "#hide " << sortname << "/1.";
			nextStmt(c, tmpout.str(), IPart::BASE);
			tmpout.str("");


			// special case for afValue translation... should be -maxAdditive..maxAdditive

			if (sort == symtab()->bsort(sy::SymbolTable::BuiltinSort::ADDITIVE)) {
				tmpout << sortname << "(-maxAdditive..maxAdditive).";
				nextStmt(c, tmpout.str(), IPart::BASE);
				tmpout.str("");
				
//				tmpout << "sort_object(" << sortname << ", -maxAdditive..maxAdditive).";
//				nextStmt(c, tmpout.str(), IPart::BASE);
//				tmpout.str("");
			}


		}


		// supersort relationships
		for (sy::SortSymbol::SortList::const_iterator it = sort->beginSuperSorts(); it != sort->endSuperSorts(); it++) {

			if (!md->translatedSubSort(*it)) {
		
				translate(*it, tmpout);
				tmpout << "(VAR) <- " << sortname << "(VAR).";
				nextStmt(c, tmpout.str(), IPart::BASE);
				tmpout.str("");

				md->addTranslatedSubSort(*it);
				
				changed = true;

				// recurse to ensure the other sort is completely declared
				translateSortDeclaration(*it, c);

			}
		}

		// subsort relationships
		for (sy::SortSymbol::SortList::const_iterator it = sort->beginSubSorts(); it != sort->endSubSorts(); it++) {

			if (!md->translatedSuperSort(*it)) {

				tmpout << sortname << "(VAR) <- ";
				translate(*it, tmpout);
				tmpout << "(VAR).";
				nextStmt(c, tmpout.str(), IPart::BASE);
				tmpout.str("");

				md->addTranslatedSuperSort(*it);
				
				changed = true;

				// recurse to ensure the other sort is completely declared
				translateSortDeclaration(*it, c);
			}
		}




	return changed;
}


		

bool Translator::translate(sy::SortSymbol const* sort, std::ostream& out) {

	out << "s_" << *sort->base();
	return true;
}

bool Translator::translate(sy::VariableSymbol const* var, std::ostream& out) {

	out << "V_" << *var->base();
	return true;
}

bool Translator::translate(sy::ConstantSymbol const* sym, Context* c, std::ostream& out, bool args, ClauseList const* setArgs) {

	if (sym->constType() & sy::ConstantSymbol::Type::M_EXTERNAL) out << "e_";
	else out << "c_";

	if (sym->constType() == sy::ConstantSymbol::Type::RIGID) out << "r_";
	else if (sym->constType() & sy::ConstantSymbol::Type::M_ACTIONS) out << "a_";
	else out << "f_";
	out << sym->arity() << "_" << *sym->base();
	if (args) {
		HANDLE_SORT_ARGS_6(sym, c, out, true, false, setArgs);
	}
	return true;
}

bool Translator::translate(sy::ObjectSymbol const* sym, Context* c, std::ostream& out, bool args) {

	DomainType::type dt = sym->domainType();
	if (dt != DomainType::INTEGRAL) out << "o_";
	out << *sym->base();
	if (args) {
		HANDLE_SORT_ARGS(sym, c, out, true, false);
	}
	return true;
}

bool Translator::translate(el::Formula const* f, Context* c, std::ostream& out) {


	if (f->parens()) out << "(";

	switch (f->subType()) {
	case el::Formula::Type::BINARY:
		{
			u::ref_ptr<const el::BinaryFormula> bf = (el::BinaryFormula const*) f;
			switch (bf->op()) {
			case el::BinaryFormula::Operator::AND:			
				translate(bf->left(), c, out);
				out << " & ";
				translate(bf->right(), c, out);
				break;
		
			case el::BinaryFormula::Operator::OR:
				bindAndTranslate(bf->left(), c, out);
				out << " | ";
				bindAndTranslate(bf->right(), c, out);
				break;

			case el::BinaryFormula::Operator::EQUIV:
				bindAndTranslate(bf->left(), c, out);
				out << " <-> ";
				bindAndTranslate(bf->right(), c, out);
				break;

			case el::BinaryFormula::Operator::IMPL:
				bindAndTranslate(bf->left(), c, out);
				out << " -> ";
				bindAndTranslate(bf->right(), c, out);
				break;

			case el::BinaryFormula::Operator::REV_IMPL:
				bindAndTranslate(bf->right(), c, out);
				out << " -> ";
				bindAndTranslate(bf->left(), c, out);
				break;

			default:
				out << "<unknown_op>";
				config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown binary formula operator." << std::endl;
				break;
			}

		}
		break;

		// -------------------------------------------------------------------------

	case el::Formula::Type::COMPARISON:
		{
			u::ref_ptr<const el::ComparisonFormula> cf = (el::ComparisonFormula const*) f;

			translate(cf->left(), c, out);
			switch (cf->op()) {
			case el::ComparisonFormula::Operator::EQ:				out << "==";	break;
			case el::ComparisonFormula::Operator::NEQ:				out << "!=";	break;		// TODO correct?
			case el::ComparisonFormula::Operator::LTHAN:			out << "<";		break;
			case el::ComparisonFormula::Operator::GTHAN:			out << ">";		break;
			case el::ComparisonFormula::Operator::LTHAN_EQ:		out << "<=";	break;
			case el::ComparisonFormula::Operator::GTHAN_EQ:		out << ">=";	break;
			default:
				out << "<unknown_op>";
				config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown comparison formula operator." << std::endl;
				break;

			}
			translate(cf->right(), c, out);
		}
		break;

		// -------------------------------------------------------------------------

	case el::Formula::Type::UNARY:
		{

			u::ref_ptr<const el::UnaryFormula> uf = (el::UnaryFormula const*)f;

			switch (uf->op()) {
			case el::UnaryFormula::Operator::NOT:
				if (uf->sub()->subType() == el::Formula::Type::ATOMIC
					&& c->neg()
					&& ((el::AtomicFormula const*)uf->sub())->c()->domainType() == bcplus::DomainType::BOOLEAN) {
					// special case optimization. not f=true becomes f=false when occurring negatively
					u::ref_ptr<const el::AtomicFormula> af = (el::AtomicFormula const*)uf->sub();
					translate(af, c, out, true);

				} else {
					out << "not ";
					bindAndTranslate(uf->sub(), c, out);
				}
				break;
			default:
				out << "<unknown_op>";
				config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown unary formula operator (#" << uf->op() << ")." << std::endl;
				break;
			}

		}
		break;
		
		// -------------------------------------------------------------------------

	case el::Formula::Type::BINDING:
		{
			u::ref_ptr<const el::BindingFormula> bf = (el::BindingFormula const*)f;

			// translate the new step
			std::stringstream tmp;
			translate(bf->step(), c, tmp);
			u::ref_ptr<Context> subc = c->mkTime(new ReferencedString(tmp.str()));

			// translate the formula
			translate(bf->element(), subc, out);
			
		}
		break;

		// -------------------------------------------------------------------------

	case el::Formula::Type::QUANTIFIER:
		{

			u::ref_ptr<const el::QuantifierFormula> qf = (el::QuantifierFormula const*)f;
			int i = 0;
			BOOST_FOREACH(el::QuantifierFormula::Quantifier const& q, *qf) {

				switch (q.first) {
				case el::QuantifierFormula::Operator::CONJ:			out << "!";			break;
				case el::QuantifierFormula::Operator::DISJ:			out << "?";			break;
				default:
					out << "<unknown_op>";
					config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown quantifier operator." << std::endl;
					break;
				}
				out << "[";
				translate(q.second->symbol(), out);
				out << "]:(";
				i++;
			}

			bindAndTranslate(qf->subformula(), c, out, false);

			for (int j = 0; j < i; j++) {
				out << ")";
			}
		}
		break;

		// -------------------------------------------------------------------------

	case el::Formula::Type::ATOMIC:
		{
			u::ref_ptr<const el::AtomicFormula> af = (el::AtomicFormula const*)f;
			translate(af, c, out);
		}
		break;

		// -------------------------------------------------------------------------

	case el::Formula::Type::NULLARY:
		{
			u::ref_ptr<const el::NullaryFormula> nf = (el::NullaryFormula const*)f;
			switch (nf->op()) {
			case el::NullaryFormula::Operator::TRUE:		out << "true";	break;
			case el::NullaryFormula::Operator::FALSE:		out << "false";	break;
			default:
					out << "<unknown_op>";
					config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown nullary operator." << std::endl;
					break;
			
			}
		}
		break;

		// -------------------------------------------------------------------------

	case el::Formula::Type::CARDINALITY:
		{
			std::stringstream tmp;
			u::ref_ptr<const el::CardinalityFormula> cf = (el::CardinalityFormula const*)f;
			
			// min bound
			if (cf->min()) translate(cf->min(), c, out);
			out << "{ ";
					

			// create a substitution for each local variable
			VariableSubstitutionList subs;
			BOOST_FOREACH(sy::VariableSymbol const* vs, *cf) {
				ReferencedString const* newvar = newVar(vs->sort(), c, false);
				subs[vs] = newvar;
	
				
			}
			


			if (c->pos() == Context::Position::HEAD || cf->formula()->subType() == el::Formula::Type::ATOMIC) {
					u::ref_ptr<Context> subc = c->mkVarSubstitutions(subs);

					// inline translation
					subc = subc->mkPos(Context::Position::AGGR);
					
				
					// translate!
					bindAndTranslate(cf->formula(), subc, out);

			} else {
				u::ref_ptr<Context> subc = c->mkPos(Context::Position::BODY);


				// non-inline translate. Make a new constant and add its definition to the prereqs.
				tmp.str("");
				std::string constname = newConst("AGGR");
				tmp << constname;

				bool first = true;
				bool include_time = ((c->ipart() == IPart::CUMULATIVE || c->ipart() == IPart::VOLATILE) && (cf->formula()->cmask() & sy::ConstantSymbol::Type::M_NON_RIGID));
				for (el::Element::VariableSet::const_iterator it = cf->formula()->beginFreeVariables(); it != cf->formula()->endFreeVariables(); it++) {
					if (!first) {
						tmp << ", ";
					} else {
						tmp << "(";
						first = false;
					}
					translate((*it), tmp);
				}

					
				if (include_time) {
					if (!first) {
						tmp << ", ";
					} else {
						tmp << "(";
						first = false;
					}
					tmp << *c->ts();
				}


				if (!first) {
					tmp << ")";
				}



				// translate the new rule and add it to the prereq statements list
				tmp << " <- ";
				bindAndTranslate(cf->formula(), subc, tmp);
				tmp << ".";
				nextStmt(subc, tmp.str(), subc->ipart());
				


				// translate the inside of the aggregate as the generated constant using the local variables...
				tmp.str("");
				tmp << constname;				
				first = true;
				for (el::Element::VariableSet::const_iterator it = cf->formula()->beginFreeVariables(); it != cf->formula()->endFreeVariables(); it++) {
					if (!first) {
						tmp << ", ";
					} else {
						tmp << "(";
						first = false;
					}

					VariableSubstitutionList::const_iterator it2 = subs.find(*it); 
					std::string const* sub = (it2 != subs.end() ? (it2->second) : NULL);
					if (sub) tmp << *sub;
					else translate((*it), tmp);
				}
				
				if (include_time) {
					if (!first) {
						tmp << ", ";
					} else {
						tmp << "(";
						first = false;
					}
					tmp << *c->ts();
				}

				if (!first) {
					tmp << ")";
				}

				out << tmp.str();
			}
					

			BOOST_FOREACH(VariableSubstitutionList::value_type const& vt, subs) {
				out << " : ";
				translate(vt.first->sort(), out);
				out << "(" << *vt.second << ")";
			}

			// max bound
			out << " }";
			if (cf->max()) translate(cf->max(), c, out);


		}
		break;

	}


	if (f->parens()) out << ")";


	return true;
}

bool Translator::translate(el::AtomicFormula const* af, Context* c, std::ostream& out, bool negative) {
	std::stringstream tmp;

	u::ref_ptr< const el::Term> val = af->v();

	if (negative) {
		if (af->c()->domainType() == bcplus::DomainType::BOOLEAN) {
			// invert the boolean value
			if (val->subType() == el::Term::Type::OBJECT) {
				if (((void*)((el::Object const*)val.get())->symbol()) ==
					((void*)symtab()->bobj(sy::SymbolTable::BuiltinObject::TRUE))) {

					val = new el::Object((sy::ObjectSymbol const*)symtab()->bobj(sy::SymbolTable::BuiltinObject::FALSE));

				} else if (((void*)((el::Object const*)val.get())->symbol()) ==
					((void*)(symtab()->bobj(sy::SymbolTable::BuiltinObject::FALSE)))) {
					
					val = new el::Object((sy::ObjectSymbol const*)symtab()->bobj(sy::SymbolTable::BuiltinObject::TRUE));

				} else {
					out << "<error_neg_af>";
					return false;
				}
			} else {
					out << "<error_neg_af>";
					return false;
			}

		} else {
			out << "<error_neg_af>";
			return false;
		}
	}


	translate(val, c, tmp);
	translate_eq(af->c(), tmp.str(), c, out);
	return true;
}


bool Translator::translate(el::Term const* t, Context* c, std::ostream& out) {

	if (t->parens()) out << "(";

	switch (t->subType()) {
		case el::Term::Type::BINARY:
			{
				u::ref_ptr<const el::BinaryTerm> bt = (el::BinaryTerm const*) t;
				translate(bt->left(), c, out);
				switch (bt->op()) {
				case el::BinaryTerm::Operator::PLUS:			out << "+";			break;
				case el::BinaryTerm::Operator::MINUS:			out << "-";			break;
				case el::BinaryTerm::Operator::TIMES:			out << "*";			break;
				case el::BinaryTerm::Operator::DIVIDE: 			out << "/";			break;
				case el::BinaryTerm::Operator::MOD:				out << " #mod ";	break;
				default:
					out << "<unknown_op>";
					config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown binary term operator." << std::endl;
					break;
				}
				translate(bt->right(), c, out);
			}
			break;

			// --------------------------------------------------------

		case el::Term::Type::UNARY:
			{
				u::ref_ptr<const el::UnaryTerm> ut = (el::UnaryTerm const*) t;
				switch (ut->op()) {
				case el::UnaryTerm::Operator::NEGATIVE:		out << "-";			break;
				case el::UnaryTerm::Operator::ABS:			out << "#abs";		break;
				default:
					out << "<unknown_op>";
					config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown unary term operator." << std::endl;
					break;
				}
				translate(ut->sub(), c, out);
			}
			break;

			// --------------------------------------------------------

		case el::Term::Type::OBJECT:
			{
				u::ref_ptr<const el::Object> o = (el::Object const*)t;
				translate(o->symbol(), c, out, false);
				HANDLE_ARGS(o, c, out, true, false);
			}
			break;

			// --------------------------------------------------------

		case el::Term::Type::VARIABLE:
			{
				u::ref_ptr<const el::Variable> v = (el::Variable const*)t;
				std::string const* s = c->getVarSubstitution(v->symbol());
				if (s) out << *s;
				else translate(v->symbol(), out);
			}
			break;

			// --------------------------------------------------------

		case el::Term::Type::CONSTANT:
			{
				u::ref_ptr<const el::Constant> constant = (el::Constant const*)t;

				// TODO: variable bindings in extraClauses should actually be translated to existential operators...				
				// translated as a variable with the variable bound later
				std::stringstream tmp;
				ReferencedString const* var = newVar(constant->symbol()->sort(), c);
				out << *var;

				// bind the variable to the constant
				translate_eq(constant, *var, c, tmp);
				c->addClause(tmp.str());
				c->addExists(*var);
			}
			break;

			// --------------------------------------------------------

		case el::Term::Type::LUA:
			{
				std::stringstream tmpout;

				u::ref_ptr<const el::LuaTerm> lt = (el::LuaTerm const*)t;
				
				tmpout << "@" << *lt->base();
				tmpout << "(";
				HANDLE_ARGS(lt, c, tmpout, false, false);
				tmpout << ")";
				std::string luaobj = tmpout.str();
				tmpout.str("");

				_computed = true;
				tmpout << "#spatom{s_computed(" << luaobj << ")}.";
				c->addPreStmt(tmpout.str(), IPart::BASE);

				out << "#spatom{" << luaobj << "}";
			}
			break;

		case el::Term::Type::PYTHON:
			{
				std::stringstream tmpout;

				u::ref_ptr<const el::PyTerm> lt = (el::PyTerm const*)t;
				
				tmpout << "@" << *lt->base();
				tmpout << "(";
				HANDLE_ARGS(lt, c, tmpout, false, false);
				tmpout << ")";
				std::string luaobj = tmpout.str();
				tmpout.str("");

				_computed = true;
				tmpout << "#spatom{s_computed(" << luaobj << ")}.";
				c->addPreStmt(tmpout.str(), IPart::BASE);

				out << "#spatom{" << luaobj << "}";

			}
		break;

		case el::Term::Type::ANON_VAR:
			{
				u::ref_ptr<const el::AnonymousVariable> lt = (el::AnonymousVariable const*)t;
				out << *lt->base();
			}
			break;

	

			// --------------------------------------------------------

		case el::Term::Type::NULLARY:
			{
				u::ref_ptr<const el::NullaryTerm> nt = (el::NullaryTerm const*)t;
				switch (nt->op()) {
				case el::NullaryTerm::Operator::MAXSTEP:		
					switch (config()->inputLanguage()) {
					case (Configuration::Input::MVPF):
						out << "maxstep";
						break;
					default:
						if (c->ipart() == IPart::BASE) out << "0";
						else out << *config()->ts(Configuration::TS::MAXSTEP);	
						break;
					}
					break;
				case el::NullaryTerm::Operator::MAXADDITIVE:	out << "maxAdditive";								break;
				case el::NullaryTerm::Operator::MAXAFVALUE:		out << "maxAdditive";								break;
				default:
					out << "<unknown_op>";
					config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered an unknown nullary term operator." << std::endl;
					break;
				}
			}
			break;

			// --------------------------------------------------------
	case el::Term::Type::BINDING:
		{
			u::ref_ptr<const el::BindingTerm> bt = (el::BindingTerm const*)t;

			// translate the new step
			std::stringstream tmp;
			translate(bt->step(), c, tmp);
			u::ref_ptr<Context> subc = c->mkTime(new ReferencedString(tmp.str()));

			// translate the formula
			translate(bt->element(), subc, out);
			
		}
		break;

		default:
			out << "<unknown_term>";
			config()->ostream(Verb::ERROR) << "ERROR: INTERNAL ERROR: Encountered unknown term type." << std::endl;
			break;

	}

	if (t->parens()) out << ")";

	return true;
}

bool Translator::bindAndTranslate(bcplus::elements::Formula const* f, Context* c, std::ostream& out, bool parens) {
	u::ref_ptr<ClauseList> extraClauses = new ClauseList();
	u::ref_ptr<ClauseList> existVars = new ClauseList();

	u::ref_ptr<Context> subc = c->mkBinds(extraClauses, existVars);
	std::stringstream tmpout;

	
	if (parens && c->pos() != Context::Position::AGGR) tmpout << "(";
	bool ret = translate(f, subc, tmpout);
	HANDLE_CLAUSES(tmpout, false, false);
	if (parens && c->pos() != Context::Position::AGGR) tmpout << ")";

	if (existVars->size() && c->pos() != Context::Position::AGGR) {
		out << "?[";
		bool first = true;
		BOOST_FOREACH(std::string const& var, * existVars) {
			if (first) first = false;
			else out << ", ";
			out << var;
		}
		out << "]:";
		if (!parens) out << "(";
		out << tmpout.str();
		if (!parens) out << ")";

	} else out << tmpout.str();


	return ret;
}


bool Translator::translate_eq(el::Constant const* constant, std::string const& value, Context* c, std::ostream& out) {

//	if (c->pos() != Context::Position::AGGR) out << "#spatom{";

/*
	switch (constant->symbol()->constType()) {
	case sy::ConstantSymbol::Type::RIGID:
	case sy::ConstantSymbol::Type::ADDITIVEFLUENT:
	case sy::ConstantSymbol::Type::EXTERNALFLUENT:
	case sy::ConstantSymbol::Type::INERTIALFLUENT:
	case sy::ConstantSymbol::Type::SDFLUENT:
	case sy::ConstantSymbol::Type::SIMPLEFLUENT:
		out << "h(eql(";
		break;
	case sy::ConstantSymbol::Type::ABACTION:
	case sy::ConstantSymbol::Type::ACTION:
	case sy::ConstantSymbol::Type::ADDITIVEACTION:
	case sy::ConstantSymbol::Type::ATTRIBUTE:
	case sy::ConstantSymbol::Type::EXTERNALACTION:
	case sy::ConstantSymbol::Type::EXOGENOUSACTION:
		out << "occ(eql(";
		break;
	} 
*/

	translate(constant->symbol(), c, out, false);
	out << "(";
	HANDLE_ARGS(constant, c, out, false, true);
	out << value;
	if (constant->symbol()->constType() != sy::ConstantSymbol::Type::RIGID) {
		out << ", " << *(c->ts());
	} // else out << ")";
	out << ")";
//	if (c->pos() != Context::Position::AGGR) out << "}";

	return true;
}


bool Translator::translate_eq(sy::ConstantSymbol const* constant, std::string const& value, Context* c, std::ostream& out, ClauseList const* setArgs) {

//	if (c->pos() != Context::Position::AGGR) out << "#spatom{";

/*
	switch (constant->symbol()->constType()) {
	case sy::ConstantSymbol::Type::RIGID:
	case sy::ConstantSymbol::Type::ADDITIVEFLUENT:
	case sy::ConstantSymbol::Type::EXTERNALFLUENT:
	case sy::ConstantSymbol::Type::INERTIALFLUENT:
	case sy::ConstantSymbol::Type::SDFLUENT:
	case sy::ConstantSymbol::Type::SIMPLEFLUENT:
		out << "h(eql(";
		break;
	case sy::ConstantSymbol::Type::ABACTION:
	case sy::ConstantSymbol::Type::ACTION:
	case sy::ConstantSymbol::Type::ADDITIVEACTION:
	case sy::ConstantSymbol::Type::ATTRIBUTE:
	case sy::ConstantSymbol::Type::EXTERNALACTION:
	case sy::ConstantSymbol::Type::EXOGENOUSACTION:
		out << "occ(eql(";
		break;
	} 
*/

	translate(constant, c, out, false);
	out << "(";
	HANDLE_SORT_ARGS_6(constant, c, out, false, true, setArgs);
	out << value;
	if (constant->constType() != sy::ConstantSymbol::Type::RIGID) {
		out << ", " << *(c->ts());
	} // else out << ")";
	out << ")";
//	if (c->pos() != Context::Position::AGGR) out << "}";

	return true;
}

bool Translator::translate_contrib(el::AtomicFormula const* body, el::Constant const* head, el::Term const* value, bool positive, Context* c, std::ostream& out) {

//	if (c->pos() != Context::Position::AGGR) out << "#spatom{";
	out << "contrib_" << *head->symbol()->base() << "(";

	if (head->arity()) {
		HANDLE_ARGS(head, c, out, false, false);
		out << ", ";
	}


	translate(body->c()->symbol(), c, out, false);
	HANDLE_ARGS(body->c(), c, out, true, false);

	out << ", ";
	if (!positive) out << " -(";
	translate(value, c, out);
	if (!positive) out << ")";
	out << ", " << *(c->ts()) << ")";
//	if (c->pos() != Context::Position::AGGR) out << "}";
	return true;
}




ReferencedString const* Translator::newVar(sy::SortSymbol const* sort, Context* c, bool global) {
	u::ref_ptr<const ReferencedString> ret;

	varmap_t* map = (global ? &_global_varmap : &_local_varmap);

	// See if we can find a new variable that we haven't seen in this statement...
	std::list<varinfo_t> &l = (*map)[sort];
	BOOST_FOREACH(varinfo_t& vi, l) {
		if (!vi.used(_stmtcount)) {
			ret = vi.name();
	
			// make sure we remember we used this variable for this statement
			vi.mark(_stmtcount);
			break;
		}
	}

	if (!ret) {
		std::stringstream tmp;
		tmp << (global ? "G" : "L") << "VAR_" << *sort->base() << "_" << l.size()+1;

		// create a new variable and register it
		ret = new ReferencedString(tmp.str());
		l.push_back(varinfo_t(ret, _stmtcount));
		tmp.str("");

		// domain the variable if it's global
		if (global) {
			tmp << "#domain ";
			translate(sort, tmp);
			tmp << "(" << *ret << ").";
			c->addPreStmt(tmp.str(), IPart::NONE);

		}
	}

	return ret.release();
}

std::string Translator::newConst(std::string const& descr) {
	return "const_" + boost::lexical_cast<std::string>(_constcount++) + "_" + descr;
}

void Translator::assertIPart(IPart::type ipart, std::string const* step) {
	static size_t oldstephash = 0;

	if (config()->outputLanguage() != Configuration::Output::INCREMENTAL) return;
	if ((oldstephash == (size_t)step) && (_ipart == ipart || ipart == IPart::NONE)) return;
	oldstephash = (size_t)step;

	switch (ipart) {
	case IPart::BASE:
		config()->out() << "#base." << std::endl;
		break;

	case IPart::CUMULATIVE:
		config()->out() << "#cumulative " << (step ? *step : *config()->ts(Configuration::TS::STATIC)) << "." << std::endl;
		break;

	case IPart::VOLATILE:
		config()->out() << "#volatile " << (step ? *step : *config()->ts(Configuration::TS::STATIC)) << "." << std::endl;
		break;

	case IPart::EXTERNAL:
		config()->out() << "#external " << (step ? *step : *config()->ts(Configuration::TS::ZERO)) << "." << std::endl;
		break;	


	default:
		break;
	}

	_ipart = ipart;

}


}}


