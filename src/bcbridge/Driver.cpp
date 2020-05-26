/* 
 * Copyright (c) 2010-2013 <Joseph Babb, Michael Cassollary, Joohyung Lee>
 *
 * For information on how to contact the authors, please visit
 *	http://reasoning.eas.asu.edu/cplus2asp
 *
 * This file is part of the cplus2asp system.
 *
 * cplus2asp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cplus2asp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iterator>


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "boost/process.hpp"

#ifdef BOOST_TIMER
#include <boost/timer/timer.hpp>
#endif

#include "babb/utils/memory.h"
#include "memwrappers.h"
#include "bcplus/parser/BCParser.h"
#include "bcplus/symbols/SymbolTable.h"
#include "bcplus/statements/Statement.h"
#include "bcplus/statements/laws.h"
#include "bcplus/languages/BCPlus.h"
#include "bcplus/languages/OBCPlus.h"

#include "as2transition/TransitionFormatter.h"
#include "as2transition/TransitionPath.h"
#include "as2transition/Timestep.h"
#include "as2transition/Predicate.h"

#include "cplus2asp.bin/Translator.h"
#include "cplus2asp.bin/Configuration.h"


#include "Driver.h"
#include "NetworkClient.h"

namespace cplus2asp {
namespace bcbridge {

// The default port to attempt to connect to oClingo with.
std::string const Driver::DEF_OCLINGO_PORT("25277");

// The default host to attempt to connect to oClingo at.
std::string const Driver::DEF_OCLINGO_HOST("localhost");


Driver::Driver(std::string const& host, std::string const& port)
			: mHost(host), 
			mPort(port), 
			mLastStep(0), 
			mClosing(false)
#ifdef BOOST_TIMER
			, mShowTime(false)
#endif
	{ 

	mFormatter.config()->stripPrefix(true);

	symtab("");

}


namespace fs = boost::filesystem;
namespace el = bcplus::elements;
namespace st = bcplus::statements;
namespace sy = bcplus::symbols;
namespace pa = bcplus::parser;
namespace la = bcplus::languages;
namespace bp = boost::process;

bool Driver::symtab(std::string const& symtab) {
	bool ret = true;

	u::ref_ptr<cplus2asp_bin::Configuration> conf = new cplus2asp_bin::Configuration("bcbridge", "1.0");
	if (symtab.size()) {
		conf->symtabInput(new ReferencedPath(symtab));

	}

	// setup the translation unit...
	conf->outputStream(mTransOut);
	conf->inputLanguage(cplus2asp_bin::Configuration::Input::BCPLUS);
	
    u::ref_ptr<sy::SymbolTable> st = new sy::SymbolTable(conf);
	if (!st->good()) {
		ret = false;
		conf->symtabInput(NULL);
		st = new sy::SymbolTable(conf);
	}
	mParser = new pa::BCParser(conf, new la::OBCPlus(), st);
	mTranslator = new cplus2asp_bin::Translator(conf, st);


	return ret;
}

// copies from source to dest and then closes dest
void copy_and_close(std::istream* source, bp::postream* dest) {

			std::copy(std::istreambuf_iterator<char>(*source), std::istreambuf_iterator<char>(),
							std::ostreambuf_iterator<char>(*dest));
			dest->close();
}

// Main driver function.
int Driver::run() {

  try
  {
/*
	std::cout 	<< "Entering user-interactive mode. " 																<< std::endl
				<< "NOTICE: The reactive bridge is in alpha and is not yet complete." 								<< std::endl
				<< "It's currently required that the user provide input directly in the input language of oClingo." << std::endl
				<< "Valid oClingo input consists of four components:" 												<< std::endl
				<< "    - A step declaration '#step t.' where t is a positive integer. "							<< std::endl
				<< "      This specifies the current step."															<< std::endl
				<< "    - A number of facts specifying the abnormality constants that have occurred."				<< std::endl
				<< "      This should be of one of the forms:"														<< std::endl
				<< "              'ab_h(<static-abnormality>, t).' OR"												<< std::endl
				<< "              'ab_occ(<dynamic-abnormality>,t-1).'"												<< std::endl
				<< "      where t is the step provided in the step declaration."									<< std::endl
				<< "    - A line containing only '#endstep.' specifying the end of input. "							<< std::endl
				<< "When you're finished, you should input 'exit' on a line by itself."								<< std::endl;
*/


	std::cout << "% Attempting to connect to oClingo..." << std::endl;

    mClient = new NetworkClient(
		mHost, 
		mPort, 
		boost::bind(&Driver::processOclingoPacket, this, _1, _2, _3));

	while (!mClient->open()) {
		boost::this_thread::sleep( boost::posix_time::milliseconds(200));

	}

	std::cout 	<< "% Connected succesfully." << std::endl
				<< "% Please provide an observation stream to send to oclingo." << std::endl
				<< "% An observation stream component is of the form:" << std::endl
				<< "% \t#step." << std::endl
				<< "% \t<rules>"<< std::endl
				<< "% \t#endstep."<<std::endl
				<< "% where <rules> is a set of observational laws " << std::endl
				<< "% \tobserved F at t." << std::endl
				<< "% and temporal constraint laws " << std::endl
				<< "% \tconstraint F at t." << std::endl
				<< "% containing no variables." << std::endl
				<< "% At any time you can input the line \"#done.\" to finish and exit." << std::endl << std::endl;

    std::string line;
	std::string trimmed;

//  Providied by initial oClingo prompt.
//	std::cout << "?- ";

    while (!mClosing && mClient->open() && std::getline(std::cin, line))
    {

		trimmed = line;
		boost::trim(trimmed);
		if (trimmed == "#step.") {
			mTransIn.str("");
			bool endstep = false, empty = true;

			// got a new step... collect the data until they say to stop...
			while (!mClosing && !endstep && mClient->open() && std::getline(std::cin, line)) {
				trimmed = line;
				boost::trim(trimmed);
				if (trimmed == "#endstep.") {
					endstep = true;
				} else if (trimmed == "#done.") {
					endstep = true;
					mClosing = true;
				} else {
					empty = false;
					// add the line to our input to parse
					mTransIn << line << std::endl;
				}
			}

			if (mClosing) continue;


			// parse the input
			if (!empty) {
				std::string transin = mTransIn.str();
				mTransOut.str("");
				mParser->reset();
				mParser->push_back(transin.c_str());


				pa::BCParser::ParseType result;
				bool ret = true;
				int step = 0;

				do {
					result = mParser->parse();

					if (result.first == pa::BCParser::Status::IO_ERR ||
						result.first == pa::BCParser::Status::SYNTAX_ERR) ret = false;

						if (result.second) {
							u::ref_ptr<st::Statement> stmt = result.second;

							switch (stmt->type()) {
							case st::Statement::Type::LAW_OBSERVED:
								{
									u::ref_ptr<const st::ObservedLaw> l = (st::ObservedLaw const*)stmt.get();
									int step2 = l->at()->val();
									if (l->head()->cmask() & sy::ConstantSymbol::Type::M_ACTIONS) step2++;
									step = (step2 > step ? step2 : step);
								}
								break;
							case st::Statement::Type::LAW_TEMPORAL_CONSTRAINT:
								{
									u::ref_ptr<const st::TemporalConstraintLaw> l = (st::TemporalConstraintLaw const*)stmt.get();
									int step2 = l->at()->val();
									if (l->head()->cmask() & sy::ConstantSymbol::Type::M_ACTIONS) step2++;
									step = (step2 > step ? step2 : step);
								}
								break;
							default: 
								break;
							}

							ret = mTranslator->translate(stmt) && ret;
						}

				} while (result.first != pa::BCParser::Status::END_INPUT);


#ifdef DEBUG
				std::cout << "Translator output: \n" << mTransOut.str() << std::endl;
#endif

		

				// process it through F2LP
				if (ret) {

					bp::context ctx;
					ctx.environment = bp::self::get_environment();
					ctx.stdin_behavior  = bp::capture_stream();
					ctx.stdout_behavior = bp::capture_stream();
					ctx.stderr_behavior = bp::inherit_stream();
				
					std::list<std::string> arglist; 
					arglist.push_back("-c"); 
					arglist.push_back("-i"); 
					bp::child c = bp::launch(bp::find_executable_in_path("f2lp"), arglist, ctx);
					
					// send our translation data as input
/*
E
					copy(std::istreambuf_iterator<char>(mTransOut), std::istreambuf_iterator<char>(),
						std::ostreambuf_iterator<char>(c.get_stdin()));
*/

					boost::thread workerthread(boost::bind(&copy_and_close, &mTransOut, &c.get_stdin()));

	
					mF2lpOut.str("");
					mF2lpOut.clear();
	
					// receive data back as input
					copy(std::istreambuf_iterator<char>(c.get_stdout()), std::istreambuf_iterator<char>(),
						std::ostreambuf_iterator<char>(mF2lpOut));
	

					// wait for F2LP to finish
					workerthread.join();

					bp::status s = c.wait();
					if (s.exited() && s.exit_status()) ret = false;

				}

#ifdef DEBUG
				std::cout << "F2LP output: \n" << mF2lpOut.str() << std::endl;
#endif

				// sanitize F2LP output for use with oClingo
				std::stringstream tmp;

				std::string line;
				while (mF2lpOut.good()) {
					std::getline(mF2lpOut, line);
					boost::trim(line);
					if (line[0] == '%' || line[0] == '#') continue;
					if (line.find(":- false.") == 0) continue;
					if (line.find("true.") == 0) continue;
//					if (line.find("false") == 0) line = line.substr(5);
					tmp << line << "\n";
				}

	
				if (!ret) {
					std::cerr << "% ERROR: An error occurred parsing the online module. The module will be ignored." << std::endl;
				} else {
					send(tmp.str(), step, mClosing);
				}
		} else {
			send("", 0, mClosing);
		}





		} else if (trimmed == "#done.") {
			mClosing = true;
		} else {
			std::cerr << "% ERROR: Ignoring invalid input. Expected one of \"#step.\" or \"#done.\"." << std::endl;
		}

    }

	close(0);
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
	return 1;
  }
	return 0;
}

// Processes a packet received by oclingo.
void Driver::processOclingoPacket(boost::system::error_code const& error, char const* msg, size_t length) { 
	std::string strmsg(msg, length), tmp;
	size_t pos;

	if (!error) {

		if (((pos = strmsg.find("Step:")) != std::string::npos) 
				|| ((pos = strmsg.find("Input:")) != std::string::npos)) {
			if (( pos = strmsg.find('\n', pos)) != std::string::npos && pos < strmsg.size())
				strmsg = strmsg.substr(pos+1);
			else
				strmsg = "";
			mOclingoOut << std::endl;
			displayTransition(mOclingoOut, std::cout);
			mOclingoOut.str("");
		}
		

		if ((pos = strmsg.find("End of Step.")) == std::string::npos)
			mOclingoOut << strmsg;
		else {
			tmp = strmsg.substr(0,pos);
			mOclingoOut << tmp << std::endl;
			displayTransition(mOclingoOut, std::cout);

			// Start collecting the next answer set.
			mOclingoOut.str("");
			pos += strlen("End of Step.");
			if (pos <= strmsg.size())
				mOclingoOut << strmsg.substr(pos);
		}
	

	} else if (!mClosing) {
		std::cerr << "% The connection to oClingo has been closed remotely.\n";
		close(1);
	}

	if (mClosing) {
		std::cout << "% Finished." << std::endl;
		close(0);
	}
//	int stat = system(tmpCmd.c_str());

} 

// Closes the active connections and exits.
void Driver::close(int code) {
	mClosing = true;	
	mClient->close();
	exit(code);
}
// Sends a message to oClingo.
void Driver::send(std::string const& msg, size_t step, bool stop) {
	std::stringstream tmp;


	if (msg.size() || step) {
		tmp << "#step " << step << ".\n";
	}

	tmp << msg << std::endl;

	if (stop) {
		tmp << "#stop.\n";
	} else {
		tmp << "#endstep.\n";
	}

#ifdef DEBUG
	std::cout << "Sending: '" << tmp.str() << "'." << std::endl;
#endif

	mClient->write(tmp.str(), true);
#ifdef BOOST_TIMER
	mTimer = boost::timer::cpu_timer();
	mTimer.start();
#endif


}

// Displays a transition to the user.
void Driver::displayTransition(std::istream& in, std::ostream& out) {


#ifdef BOOST_TIMER
	boost::timer::cpu_times time = mTimer.elapsed();
#endif

//	if (mPlan) delete mPlan;
	mPlan = mFormatter.format(in);

	if (mPlan->length()) {
		mPlan->output(out);

#ifdef BOOST_TIMER
		if (mShowTime) {
			out << boost::timer::format(time, 4, "Elapsed time: %w") << "\n";
		}
#endif

		out << "% Please input the next module: " << std::endl;
		out.flush();
	}


}

}}
