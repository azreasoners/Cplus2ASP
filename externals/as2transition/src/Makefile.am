include $(top_srcdir)/src/Makefile.shared


lib_LIBRARIES = ../libas2transition.a
bin_PROGRAMS = ../as2transition


BUILT_SOURCES = \
	as2transition/TransitionFormatter.cpp \
	as2transition/parser/Scanner.cpp \
	as2transition/parser/lemon_parser.cpp

___libas2transition_a_SOURCES = \
	as2transition/TransitionFormatter.cpp \
	as2transition/Config.cpp \
	as2transition/PredElement.cpp \
	as2transition/Predicate.cpp \
	as2transition/Timestep.cpp \
	as2transition/TransitionPath.cpp \
	as2transition/parser/Scanner.cpp \
	as2transition/parser/lemon_parser.cpp \
	as2transition/parser/PredicateParser.cpp 


___as2transition_SOURCES = \
	main.cpp 

___as2transition_LDADD = ../libas2transition.a

