include $(top_srcdir)/src/Makefile.shared

# Turn off the reactive bridge as it's not implemented.
#AM_CFLAGS += -DNO_REACTIVE_BRIDGE
#AM_CXXFLAGS += -DNO_REACTIVE_BRIDGE


bin_PROGRAMS = cplus2asp

cplus2asp4_CPPFLAGS = $(BOOST_CPPFLAGS)
cplus2asp4_LDFLAGS = $(BOOST_THREAD_LDFLAGS) $(BOOST_FILESYSTEM_LDFLAGS) $(BOOST_IOSTREAMS_LIBS) $(BOOST_SYSTEM_LDFLAGS)
cplus2asp4_LDADD = -lbcplusparser $(BOOST_THREAD_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_IOSTREAMS_LIBS) $(BOOST_SYSTEM_LIBS)


BUILT_SOURCES = cplus2asp.cpp

cplus2asp4_SOURCES = \
	Config.cpp \
	utils.cpp \
	cplus2asp.r2c \
	Interpreter.r2c


