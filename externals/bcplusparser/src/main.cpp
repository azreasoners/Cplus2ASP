
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Configuration.h"
#include "bcplus/Location.h"
#include "bcplus/languages/BCPlus.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/declarations.h"
#include "bcplus/symbols/SymbolTable.h"
#include "bcplus/symbols/Symbol.h"
#include "bcplus/symbols/MacroSymbol.h"
#include "bcplus/parser/BCParser.h"

#define VERSION "0.1 ALPHA"

namespace fs = boost::filesystem;
namespace u = babb::utils;
using namespace bcplus;



/// Main driver program
int main(int argc, char const** argv) {

	// load configuration information
	u::ref_ptr<Configuration> conf = new Configuration(argv[0], VERSION);
	
	switch (conf->load(argc, argv)) {
	case Configuration::Status::STAT_OK:					
		break;
	case Configuration::Status::STAT_BAD_ARG:
		std::cerr << "ERROR: An error occurred parsing command line options. Exiting." << std::endl;
		return 1;
	case Configuration::Status::STAT_HELP:
		conf->outputHelp(std::cout);
		return 0;
	case Configuration::Status::STAT_VERSION:
		conf->outputVersion(std::cout);
		return 0;
	}

	if (!conf->good()) {
		std::cerr << "ERROR: An error occurred verifying consistency of command line options. Exiting." << std::endl;
		return 1;
	}



	// setup the parser
	conf->ostream(Verb::OP) << "Initializing parser..." << std::endl;
	u::ref_ptr<parser::BCParser> p = new parser::BCParser(conf, new languages::BCPlus());
	conf->ostream(Verb::OP) << "Adding test input..." << std::endl;
	p->push_back(fs::path("tmp.cp"), false);
	
	conf->ostream(Verb::OP) << "Parsing..." << std::endl;
	parser::BCParser::ParseType result;
	do {
		result = p->parse();

		if (result.second) {

			statements::Statement* stmt = result.second;
			// TODO	

		}

	} while (result.first != parser::BCParser::Status::END_INPUT);

	return 0;
}
