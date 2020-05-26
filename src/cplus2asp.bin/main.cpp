
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>

#include "babb/utils/memory.h"
#include "memwrappers.h"

#include "bcplus/Location.h"
#include "bcplus/languages/BCPlus.h"
#include "bcplus/languages/BC.h"
#include "bcplus/languages/CPlus.h"
#include "bcplus/languages/MVPF.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/declarations.h"
#include "bcplus/parser/BCParser.h"
#include "bcplus/symbols/SymbolTable.h"


#include "Configuration.h"
#include "Translator.h"

#define VERSION "0.1 ALPHA"

namespace fs = boost::filesystem;
namespace u = babb::utils;
using namespace cplus2asp::cplus2asp_bin;

#define DEBUG 
/// Main driver program
int main(int argc, char const** argv) {

	// load configuration information
	u::ref_ptr<Configuration> conf = new Configuration(argv[0], VERSION);
	
	switch (conf->load(argc, argv)) {
	case Configuration::Status::STAT_OK:					
		break;
	case Configuration::Status::STAT_BAD_ARG:
	case Configuration::Status::STAT_FILE_NOT_FOUND:
		std::cerr << "ERROR: An error occurred parsing command line options. Exiting." << std::endl;
		return -1;
	case Configuration::Status::STAT_HELP:
		conf->outputHelp(std::cout);
		return 0;
	case Configuration::Status::STAT_VERSION:
		conf->outputVersion(std::cout);
		return 0;

	}

	if (!conf->good()) {
		std::cerr << "ERROR: An error occurred verifying consistency of command line options. Exiting." << std::endl;
		return -1;
	}


	// setup the symbol table
	u::ref_ptr<bcplus::symbols::SymbolTable> st = new bcplus::symbols::SymbolTable(conf, new Translator::SymbolMetadataInitializer());

	// setup the parser
	conf->ostream(Verb::OP) << "Initializing parser..." << std::endl;

	u::ref_ptr<bcplus::languages::Language> lang;
	switch (conf->inputLanguage()) {
	case Configuration::Input::BCPLUS:			lang = new bcplus::languages::BCPlus();	break;
	case Configuration::Input::CPLUS:			lang = new bcplus::languages::CPlus();	break;
	case Configuration::Input::BC:				lang = new bcplus::languages::BC();		break;
	case Configuration::Input::MVPF:			lang = new bcplus::languages::MVPF();	break;
	}

	u::ref_ptr<bcplus::parser::BCParser> p = new bcplus::parser::BCParser(conf, lang, st);


	// Read from our input files and add them to the parser
	bool have_input = false;
	BOOST_FOREACH(ReferencedPath const* f, *conf) {
		p->push_back(*f);
		have_input = true;
	}

	if (!have_input) {
		conf->ostream(Verb::ERROR) << "ERROR: No input files were specified. Exiting." << std::endl;
		return -1;
	}


	// setup the translator
	conf->ostream(Verb::OP) << "Initializing translator..." << std::endl;
	u::ref_ptr<Translator> t = new Translator(conf, st); 

	// prologue
	t->prologue();
	
	conf->ostream(Verb::OP) << "Parsing..." << std::endl;
	bcplus::parser::BCParser::ParseType result;
	bool ret = true;
	do {
		result = p->parse();

		if (result.first == bcplus::parser::BCParser::Status::IO_ERR ||
			result.first == bcplus::parser::BCParser::Status::SYNTAX_ERR) ret = false;


		if (result.second) {

			bcplus::statements::Statement* stmt = result.second;
			ret = t->translate(stmt) && ret;

		}

	} while (result.first != bcplus::parser::BCParser::Status::END_INPUT);

	// epilogue
	t->epilogue();


	conf->ostream(Verb::OP) << "Done parsing..." << std::endl;

	if (!ret) return -1;
	else return 0;
	return 0;
}
