#include "clingo3to4.h"
/*
Manual command for compiling
g++ -g -I /usr/include/boost program.cpp clingo3to4.cpp domain.cpp -o clingo3to4 -L /usr/lib/x86_64-linux-gnu/ -lboost_regex -lboost_program_options
*/

int main(int argc, char const *argv[])
{
	clingo3to4 converter;
	converter.convert(argc,argv);
	return 0;
}