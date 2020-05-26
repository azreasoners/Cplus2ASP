/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "bcplus/parser/detail/lemon_macro.y"

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


		
#line 178 "bcplus/parser/detail/lemon_macro.y"

	typedef ReferencedList<ref_ptr<Token> >::type TokenList;
	typedef ReferencedWrapper<std::pair<ref_ptr<TokenList>, ref_ptr<Macro::ArgumentList> > > CommaArgumentList;
#line 43 "bcplus/parser/detail/lemon_macro.c"
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
**    lemon_macroTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is lemon_macroTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    lemon_macroARG_SDECL     A static variable declaration for the %extra_argument
**    lemon_macroARG_PDECL     A parameter declaration for the %extra_argument
**    lemon_macroARG_STORE     Code to store %extra_argument into yypParser
**    lemon_macroARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 26
#define YYACTIONTYPE unsigned char
#define lemon_macroTOKENTYPE  Token*	 								
typedef union {
  int yyinit;
  lemon_macroTOKENTYPE yy0;
  CommaArgumentList* yy5;
  UNUSED yy23;
  IncludeStatement::ElementList* yy32;
  Token const* yy33;
  int yy51;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define lemon_macroARG_SDECL  MacroParser* parser						;
#define lemon_macroARG_PDECL , MacroParser* parser						
#define lemon_macroARG_FETCH  MacroParser* parser						 = yypParser->parser						
#define lemon_macroARG_STORE yypParser->parser						 = parser						
#define YYNSTATE 38
#define YYNRULE 29
#define YYERRORSYMBOL 14
#define YYERRSYMDT yy51
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
#define YY_ACTTAB_COUNT (39)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     9,   23,   22,   21,   20,   38,   14,   16,   19,   18,
 /*    10 */     2,   17,   15,   13,   12,   11,   10,   37,   30,   29,
 /*    20 */    34,   36,   35,   26,    8,   68,    1,   31,   33,    4,
 /*    30 */     5,    3,    6,   25,   24,    7,   32,   28,   27,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,    4,    5,    0,    7,    8,    9,   10,
 /*    10 */     6,   12,   13,   20,   21,   22,   23,   14,    1,    2,
 /*    20 */    17,   18,   19,    4,    5,   15,   16,   10,    8,    5,
 /*    30 */     9,    5,    3,   11,   11,   24,   23,   23,   23,
};
#define YY_SHIFT_USE_DFLT (-2)
#define YY_SHIFT_COUNT (15)
#define YY_SHIFT_MIN   (-1)
#define YY_SHIFT_MAX   (29)
static const signed char yy_shift_ofst[] = {
 /*     0 */    -2,   -1,   17,   17,   17,   17,   23,   19,   22,   29,
 /*    10 */    26,   24,   21,   20,    4,    5,
};
#define YY_REDUCE_USE_DFLT (-8)
#define YY_REDUCE_COUNT (6)
#define YY_REDUCE_MIN   (-7)
#define YY_REDUCE_MAX   (15)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    10,    3,   -7,   15,   14,   13,   11,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    39,   67,   67,   67,   67,   67,   67,   67,   67,   54,
 /*    10 */    47,   46,   45,   67,   62,   67,   66,   65,   64,   63,
 /*    20 */    61,   60,   59,   58,   56,   57,   55,   49,   50,   53,
 /*    30 */    52,   51,   48,   44,   43,   42,   41,   40,
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
  lemon_macroARG_SDECL                /* A place to hold %extra_argument */
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
void lemon_macroTrace(FILE *TraceFILE, char const*zTracePrompt){
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
  "$",             "IDENTIFIER",    "INTEGER",       "PAREN_L",     
  "PAREN_R",       "COMMA",         "INCLUDE",       "COLON_DASH",  
  "PERIOD",        "SEMICOLON",     "STRING_LITERAL",  "ARG",         
  "OTHER",         "EOF",           "error",         "start",       
  "macro_lst",     "incl_stmt",     "base_elem_pkgd",  "other",       
  "incl_lst",      "incl_lst_sc",   "incl_lst_c",    "incl_item",   
  "arg_lst",     
};
//#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "start ::= macro_lst EOF",
 /*   1 */ "macro_lst ::=",
 /*   2 */ "macro_lst ::= macro_lst error",
 /*   3 */ "macro_lst ::= macro_lst base_elem_pkgd",
 /*   4 */ "macro_lst ::= macro_lst other",
 /*   5 */ "macro_lst ::= macro_lst incl_stmt",
 /*   6 */ "incl_stmt ::= COLON_DASH INCLUDE incl_lst PERIOD",
 /*   7 */ "incl_lst ::= incl_lst_sc",
 /*   8 */ "incl_lst ::= incl_lst_c",
 /*   9 */ "incl_lst_sc ::= incl_item",
 /*  10 */ "incl_lst_sc ::= incl_lst_sc SEMICOLON incl_item",
 /*  11 */ "incl_lst_c ::= incl_item COMMA incl_item",
 /*  12 */ "incl_lst_c ::= incl_lst_c COMMA incl_item",
 /*  13 */ "incl_item ::= STRING_LITERAL",
 /*  14 */ "incl_item ::= IDENTIFIER",
 /*  15 */ "incl_item ::= INTEGER",
 /*  16 */ "base_elem_pkgd ::= IDENTIFIER",
 /*  17 */ "base_elem_pkgd ::= IDENTIFIER PAREN_L arg_lst PAREN_R",
 /*  18 */ "arg_lst ::= ARG",
 /*  19 */ "arg_lst ::= arg_lst COMMA ARG",
 /*  20 */ "other ::= INTEGER",
 /*  21 */ "other ::= PAREN_L",
 /*  22 */ "other ::= PAREN_R",
 /*  23 */ "other ::= COMMA",
 /*  24 */ "other ::= COLON_DASH",
 /*  25 */ "other ::= SEMICOLON",
 /*  26 */ "other ::= STRING_LITERAL",
 /*  27 */ "other ::= OTHER",
 /*  28 */ "other ::= PERIOD",
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
** to lemon_macro and lemon_macroFree.
*/
void *lemon_macroAlloc(void *(*mallocProc)(size_t)){
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
  lemon_macroARG_FETCH;
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
    case 1: /* IDENTIFIER */
    case 2: /* INTEGER */
    case 3: /* PAREN_L */
    case 4: /* PAREN_R */
    case 5: /* COMMA */
    case 6: /* INCLUDE */
    case 7: /* COLON_DASH */
    case 8: /* PERIOD */
    case 9: /* SEMICOLON */
    case 10: /* STRING_LITERAL */
    case 11: /* ARG */
    case 12: /* OTHER */
    case 13: /* EOF */
{
#line 55 "bcplus/parser/detail/lemon_macro.y"
 DEALLOC((yypminor->yy0));								
#line 456 "bcplus/parser/detail/lemon_macro.c"
}
      break;
    case 15: /* start */
    case 16: /* macro_lst */
    case 17: /* incl_stmt */
    case 18: /* base_elem_pkgd */
    case 19: /* other */
{
#line 67 "bcplus/parser/detail/lemon_macro.y"
 /* Intentionally left blank */			
#line 467 "bcplus/parser/detail/lemon_macro.c"
}
      break;
    case 20: /* incl_lst */
    case 21: /* incl_lst_sc */
    case 22: /* incl_lst_c */
{
#line 95 "bcplus/parser/detail/lemon_macro.y"
 DEALLOC((yypminor->yy32));                              
#line 476 "bcplus/parser/detail/lemon_macro.c"
}
      break;
    case 23: /* incl_item */
{
#line 101 "bcplus/parser/detail/lemon_macro.y"
 DEALLOC((yypminor->yy33));                              
#line 483 "bcplus/parser/detail/lemon_macro.c"
}
      break;
    case 24: /* arg_lst */
{
#line 184 "bcplus/parser/detail/lemon_macro.y"
 DEALLOC((yypminor->yy5));														
#line 490 "bcplus/parser/detail/lemon_macro.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
  lemon_macroARG_STORE;
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
**       obtained from lemon_macroAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void lemon_macroFree(
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
int lemon_macroStackPeak(void *p){
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
   lemon_macroARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
   lemon_macroARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 15, 2 },
  { 16, 0 },
  { 16, 2 },
  { 16, 2 },
  { 16, 2 },
  { 16, 2 },
  { 17, 4 },
  { 20, 1 },
  { 20, 1 },
  { 21, 1 },
  { 21, 3 },
  { 22, 3 },
  { 22, 3 },
  { 23, 1 },
  { 23, 1 },
  { 23, 1 },
  { 18, 1 },
  { 18, 4 },
  { 24, 1 },
  { 24, 3 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
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
  lemon_macroARG_FETCH;
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
      case 0: /* start ::= macro_lst EOF */
#line 77 "bcplus/parser/detail/lemon_macro.y"
{
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
#line 824 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 6: /* incl_stmt ::= COLON_DASH INCLUDE incl_lst PERIOD */
#line 104 "bcplus/parser/detail/lemon_macro.y"
{
	        ref_ptr<IncludeStatement::ElementList> lst_ptr = yymsp[-1].minor.yy32;
            
			if (!parser->lang()->support(Language::Feature::DECL_INCLUDE)) {
                parser->_feature_error(Language::Feature::DECL_INCLUDE, &yymsp[-3].minor.yy0->beginLoc());
                YYERROR;
            } else {
	        	ref_ptr<IncludeStatement> stmt = new IncludeStatement(yymsp[-1].minor.yy32, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc() );
				parser->_handle_include(stmt);
			}

            delete yymsp[-3].minor.yy0;
            delete yymsp[0].minor.yy0;

          yy_destructor(yypParser,6,&yymsp[-2].minor);
}
#line 844 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 7: /* incl_lst ::= incl_lst_sc */
      case 8: /* incl_lst ::= incl_lst_c */ yytestcase(yyruleno==8);
#line 121 "bcplus/parser/detail/lemon_macro.y"
{
			yygotominor.yy32 = yymsp[0].minor.yy32;
		}
#line 852 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 9: /* incl_lst_sc ::= incl_item */
#line 131 "bcplus/parser/detail/lemon_macro.y"
{
            yygotominor.yy32 = new IncludeStatement::ElementList();
            yygotominor.yy32->push_back(yymsp[0].minor.yy33->str());
            delete yymsp[0].minor.yy33;
        }
#line 861 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 10: /* incl_lst_sc ::= incl_lst_sc SEMICOLON incl_item */
#line 138 "bcplus/parser/detail/lemon_macro.y"
{
            yygotominor.yy32 = yymsp[-2].minor.yy32;
            yygotominor.yy32->push_back(yymsp[0].minor.yy33->str());
            delete yymsp[0].minor.yy33;
          yy_destructor(yypParser,9,&yymsp[-1].minor);
}
#line 871 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 11: /* incl_lst_c ::= incl_item COMMA incl_item */
#line 145 "bcplus/parser/detail/lemon_macro.y"
{
            yygotominor.yy32 = new IncludeStatement::ElementList();
            yygotominor.yy32->push_back(yymsp[-2].minor.yy33->str());
            yygotominor.yy32->push_back(yymsp[0].minor.yy33->str());
            delete yymsp[-2].minor.yy33;
            delete yymsp[0].minor.yy33;
          yy_destructor(yypParser,5,&yymsp[-1].minor);
}
#line 883 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 12: /* incl_lst_c ::= incl_lst_c COMMA incl_item */
#line 154 "bcplus/parser/detail/lemon_macro.y"
{
            yygotominor.yy32 = yymsp[-2].minor.yy32;
            yygotominor.yy32->push_back(yymsp[0].minor.yy33->str());
            delete yymsp[0].minor.yy33;
          yy_destructor(yypParser,5,&yymsp[-1].minor);
}
#line 893 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 13: /* incl_item ::= STRING_LITERAL */
      case 14: /* incl_item ::= IDENTIFIER */ yytestcase(yyruleno==14);
      case 15: /* incl_item ::= INTEGER */ yytestcase(yyruleno==15);
#line 161 "bcplus/parser/detail/lemon_macro.y"
{
            yygotominor.yy33 = yymsp[0].minor.yy0;
        }
#line 902 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 16: /* base_elem_pkgd ::= IDENTIFIER */
#line 187 "bcplus/parser/detail/lemon_macro.y"
{
			ref_ptr<const Token> id_ptr = yymsp[0].minor.yy0;
			Symbol const* sym = parser->symtab()->resolve(Symbol::Type::M_TERM, *yymsp[0].minor.yy0->str());
			if (sym && sym->type() == Symbol::Type::MACRO) {
				if (parser->lang()->support(Language::Feature::MACRO)) {
					ref_ptr<Macro> elem = new Macro((MacroSymbol*)sym, NULL, yymsp[0].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
					parser->_handle_macro(elem);
				} else {
					parser->_feature_error(Language::Feature::MACRO, &yymsp[0].minor.yy0->beginLoc());
					YYERROR;
				}
			} else {
				// Change the identifier token depending on what type of symbol it is (if any)
				if (sym) {
					switch (sym->type()) {
					case Symbol::Type::CONSTANT:
						yymsp[0].minor.yy0->type(T_CONSTANT_ID);
						break;
					case Symbol::Type::OBJECT:
						yymsp[0].minor.yy0->type(T_OBJECT_ID);
						break;
					case Symbol::Type::VARIABLE:
						yymsp[0].minor.yy0->type(T_VARIABLE_ID);
						break;
					default:
						break;
					}
				}	
				parser->_token(yymsp[0].minor.yy0);
			}
		}
#line 937 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 17: /* base_elem_pkgd ::= IDENTIFIER PAREN_L arg_lst PAREN_R */
#line 220 "bcplus/parser/detail/lemon_macro.y"
{
			// this has a packed argument list to allow for macro expansion.
			ref_ptr<const Token> id_ptr = yymsp[-3].minor.yy0;
			ref_ptr<const Token> pl_ptr = yymsp[-2].minor.yy0;
			ref_ptr<CommaArgumentList> args_ptr = yymsp[-1].minor.yy5;
			ref_ptr<const Token> pr_ptr = yymsp[0].minor.yy0;

			Symbol const* sym = parser->symtab()->resolve(Symbol::Type::M_TERM, *yymsp[-3].minor.yy0->str(), yymsp[-1].minor.yy5->second->size());
			if (sym && sym->type() == Symbol::Type::MACRO) {
				
				if (parser->lang()->support(Language::Feature::MACRO)) {
					ref_ptr<Macro> elem = new Macro((MacroSymbol*)sym, yymsp[-1].minor.yy5->second, yymsp[-3].minor.yy0->beginLoc(), yymsp[0].minor.yy0->endLoc());
					parser->_handle_macro(elem);
				} else {
					parser->_feature_error(Language::Feature::MACRO, &yymsp[-3].minor.yy0->beginLoc());
					YYERROR;
				}

			} else {	
				// It's not a macro. Just go ahead and unpack it and re-add it for reading
				parser->_push_front(yymsp[0].minor.yy0);
				TokenList::reverse_iterator commas = yymsp[-1].minor.yy5->first->rbegin();
				for (Macro::ArgumentList::reverse_iterator it = yymsp[-1].minor.yy5->second->rbegin(); it != yymsp[-1].minor.yy5->second->rend(); ) {
					parser->_push_front((*it)->str()->c_str(), (*it)->beginLoc(), true);

					it++;
					if (it != yymsp[-1].minor.yy5->second->rend()) {
						parser->_push_front((*commas));
						commas++;
					}
				}

				// Change the identifier token depending on what type of symbol it is (if any)
				if (sym) {
					switch (sym->type()) {
					case Symbol::Type::CONSTANT:
						yymsp[-3].minor.yy0->type(T_CONSTANT_ID);
						break;
					case Symbol::Type::OBJECT:
						yymsp[-3].minor.yy0->type(T_OBJECT_ID);
						break;
					case Symbol::Type::VARIABLE:
						yymsp[-3].minor.yy0->type(T_VARIABLE_ID);
						break;
					default:
						break;
					}
				}	
				
				parser->_token(yymsp[-3].minor.yy0);
				parser->_token(yymsp[-2].minor.yy0);
			}
		}
#line 994 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 18: /* arg_lst ::= ARG */
#line 275 "bcplus/parser/detail/lemon_macro.y"
{
			yygotominor.yy5 = new CommaArgumentList();
			yygotominor.yy5->first = new TokenList();
			yygotominor.yy5->second = new Macro::ArgumentList();
			yygotominor.yy5->second->push_back(yymsp[0].minor.yy0);
		}
#line 1004 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 19: /* arg_lst ::= arg_lst COMMA ARG */
#line 283 "bcplus/parser/detail/lemon_macro.y"
{
			yygotominor.yy5 = yymsp[-2].minor.yy5;
			yymsp[-2].minor.yy5->first->push_back(yymsp[-1].minor.yy0);
			yymsp[-2].minor.yy5->second->push_back(yymsp[0].minor.yy0);
		}
#line 1013 "bcplus/parser/detail/lemon_macro.c"
        break;
      case 20: /* other ::= INTEGER */
      case 21: /* other ::= PAREN_L */ yytestcase(yyruleno==21);
      case 22: /* other ::= PAREN_R */ yytestcase(yyruleno==22);
      case 23: /* other ::= COMMA */ yytestcase(yyruleno==23);
      case 24: /* other ::= COLON_DASH */ yytestcase(yyruleno==24);
      case 25: /* other ::= SEMICOLON */ yytestcase(yyruleno==25);
      case 26: /* other ::= STRING_LITERAL */ yytestcase(yyruleno==26);
      case 27: /* other ::= OTHER */ yytestcase(yyruleno==27);
      case 28: /* other ::= PERIOD */ yytestcase(yyruleno==28);
#line 293 "bcplus/parser/detail/lemon_macro.y"
{ parser->_token(yymsp[0].minor.yy0); }
#line 1026 "bcplus/parser/detail/lemon_macro.c"
        break;
      default:
      /* (1) macro_lst ::= */ yytestcase(yyruleno==1);
      /* (2) macro_lst ::= macro_lst error */ yytestcase(yyruleno==2);
      /* (3) macro_lst ::= macro_lst base_elem_pkgd */ yytestcase(yyruleno==3);
      /* (4) macro_lst ::= macro_lst other */ yytestcase(yyruleno==4);
      /* (5) macro_lst ::= macro_lst incl_stmt */ yytestcase(yyruleno==5);
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
  lemon_macroARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  lemon_macroARG_STORE; /* Suppress warning about unused %extra_argument variable */
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
  lemon_macroARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 59 "bcplus/parser/detail/lemon_macro.y"
 parser->_parse_error("Syntax error.");	
#line 1095 "bcplus/parser/detail/lemon_macro.c"
  lemon_macroARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  lemon_macroARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  lemon_macroARG_STORE; /* Suppress warning about unused %extra_argument variable */
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
int lemon_macroPreInject(void* yyp, int pop, lemon_macroTOKENTYPE* lookahead) {
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
char const* lemon_macroTokenName(int tok) {
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
void lemon_macroAttemptReduce(void* yyp lemon_macroARG_PDECL) {
	yyParser* yypParser = (yyParser*)yyp;
	lemon_macroARG_STORE;
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
** "lemon_macroAlloc" which describes the current state of the parser.
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
void lemon_macro(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  lemon_macroTOKENTYPE yyminor       /* The value for the token */
  lemon_macroARG_PDECL               /* Optional %extra_argument parameter */
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
  lemon_macroARG_STORE;

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
