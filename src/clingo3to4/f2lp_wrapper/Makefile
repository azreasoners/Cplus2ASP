#Referenced from http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

CC = g++

CFLAGS = -g 

INCLUDES = -I/use/include/boost

LFALGS = -L/usr/lib/x86_64-linux-gnu/

LIBS = -lboost_regex -lboost_program_options

SRCS = main.cpp clingo3to4.cpp domain.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = clingo3to4

.PHONY: depend clean

.DEFAULT_GOAL := all

all:	$(MAIN)
		@echo clingo3to4 compiled successfully

$(MAIN):$(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFALGS) $(LIBS)

.c.o:	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:	$(RM) *.o *~ $(MAIN)

depend: $(SRCS) makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it