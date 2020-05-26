include $(top_srcdir)/src/Makefile.rls



lib_LIBRARIES = ../libbcplusparser.a
bin_PROGRAMS = ../bcplusparser

___bcplusparser_CPPFLAGS = $(BOOST_CPPFLAGS)
___bcplusparser_LDFLAGS = $(BOOST_FILESYSTEM_LDFLAGS) $(BOOST_SYSTEM_LDFLAGS)
___bcplusparser_LDADD = ../libbcplusparser.a $(BOOST_FILESYSTEM_LIBS) $(BOOST_SYSTEM_LIBS)
___libbcplusparser_a_CPPFLAGS = $(BOOST_CPPFLAGS)
___libbcplusparser_a_LDFLAGS = $(BOOST_FILESYSTEM_LDFLAGS) $(BOOST_SYSTEM_LDFLAGS)
#___libbcplusparser_a_LIBADD = $(BOOST_FILESYSTEM_LIBS) $(BOOST_SYSTEM_LIBS)
#___bcplusparser_LDADD = ../libbcplusparser.a

BUILT_SOURCES = \
	bcplus/Configuration.cpp \
	bcplus/symbols/Symbol.cpp \
	bcplus/symbols/ConstantSymbol.cpp \
	bcplus/parser/detail/Scanner.1.cpp \
	bcplus/parser/detail/Scanner.2.cpp \
	bcplus/parser/detail/lemon_parser.cpp \
	bcplus/parser/detail/lemon_macro.cpp 

#cplus2asp_SOURCES = \
#	Configuration.cpp \
#	main.cpp \
#	Parser.cpp \
#	Referenced.cpp \
#	parser/SymbolTable.cpp \
#	parser/elements/AtomicFormula.cpp \
#	parser/elements/Element.cpp \
#	parser/elements/QuantifierFormula.cpp \
#	parser/symbols/ConstantSymbol.cpp \
#	parser/symbols/ObjectSymbol.cpp \
#	parser/symbols/SortSymbol.cpp \
#	parser/symbols/Symbol.cpp \
#	parser/symbols/VariableSymbol.cpp \
#	parser/symbols/detail/BaseSymbol.cpp \
#	parser/detail/atom_parser.c \
#	parser/detail/ScannerBufferSource.cpp \
#	parser/detail/ScannerFileSource.cpp \
#	parser/detail/Scanner.cpp \
#	parser/detail/ScannerSource.cpp 


___bcplusparser_SOURCES = \
	main.cpp

___libbcplusparser_a_SOURCES =\
	bcplus/Configuration.cpp \
	bcplus/Location.cpp \
	bcplus/elements/AtomicFormula.cpp \
	bcplus/elements/Element.cpp \
	bcplus/elements/QuantifierFormula.cpp \
	bcplus/elements/Macro.cpp \
	bcplus/elements/CardinalityFormula.cpp \
	bcplus/languages/Language.cpp \
	bcplus/languages/BCPlus.cpp \
	bcplus/languages/OBCPlus.cpp \
	bcplus/languages/CPlus.cpp \
	bcplus/languages/BC.cpp \
	bcplus/languages/MVPF.cpp \
	bcplus/symbols/ConstantSymbol.r2c \
	bcplus/symbols/AttributeSymbol.cpp \
	bcplus/symbols/ObjectSymbol.cpp \
	bcplus/symbols/SortSymbol.cpp \
	bcplus/symbols/Symbol.cpp \
	bcplus/symbols/VariableSymbol.cpp \
	bcplus/symbols/MacroSymbol.cpp \
	bcplus/symbols/QuerySymbol.cpp \
	bcplus/symbols/detail/BaseSymbol.cpp  \
	bcplus/symbols/SymbolTable.cpp \
	bcplus/symbols/NumberRangeSymbol.cpp \
	bcplus/statements/Statement.cpp \
	bcplus/statements/QueryStatement.cpp \
	bcplus/statements/detail/ObjectBinding.cpp \
	bcplus/parser/Token.cpp \
	bcplus/parser/BCParser.cpp \
	bcplus/parser/Number.cpp \
	bcplus/parser/detail/lemon_parser.cpp \
	bcplus/parser/detail/MacroParser.cpp \
	bcplus/parser/detail/lemon_macro.cpp \
	bcplus/parser/detail/Scanner.1.cpp \
	bcplus/parser/detail/Scanner.2.cpp \
	bcplus/parser/detail/Scanner.3.cpp \
	bcplus/parser/detail/ScannerSource.cpp \
	bcplus/parser/detail/ScannerTokenSource.cpp \
	bcplus/parser/detail/ScannerRawSource.cpp \
	bcplus/parser/detail/ScannerBufferSource.cpp \
	bcplus/parser/detail/ScannerFileSource.cpp \
	bcplus/parser/detail/NumberRange.cpp \
	bcplus/parser/detail/NumberRangeEval.cpp

