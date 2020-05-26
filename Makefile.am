
AUTOMAKE_OPTIONS = foreign

if HAVE_AS2TRANSITION
EXTRA_DIRS = externals/as2transition
else
EXTRA_DIRS =
endif

if HAVE_BCPLUSPARSER
EXTRA_DIRS += externals/bcplusparser
endif


# A list of subdirectories we need to work on.
SUBDIRS =  $(EXTRA_DIRS) src/cplus2asp src/cplus2asp.bin src/bcbridge src/clingo3to4

