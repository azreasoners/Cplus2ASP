include $(top_srcdir)/src/Makefile.shared

bin_PROGRAMS = clingo3to4

clingo3to4_CPPFLAGS = $(BOOST_CPPFLAGS)

clingo3to4_LDADD = $(BOOST_REGEX_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)

BUILT_SOURCES = f2lp_wrapper/main.cpp

clingo3to4_SOURCES = \
f2lp_wrapper/main.cpp \
f2lp_wrapper/clingo3to4.cpp \
f2lp_wrapper/domain.cpp 

