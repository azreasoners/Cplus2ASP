
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


#pragma once

#include <iostream>
#include <string>
#include <sstream>

#ifdef BOOST_TIMER
#include <boost/timer/timer.hpp>
#endif

#include "as2transition/TransitionFormatter.h"
#include "as2transition/TransitionPath.h"

#include "babb/utils/memory.h"
#include "memwrappers.h"
#include "bcplus/parser/BCParser.h"
#include "cplus2asp.bin/Configuration.h"
#include "cplus2asp.bin/Translator.h"

#include "NetworkClient.h"
#include "boost/bind.hpp"
#include "boost/asio/error.hpp"

namespace cplus2asp {
namespace bcbridge {

// Main Driver class for the bridge
class Driver {
public:
	/// The default port to attempt to connect to oClingo with.
	static std::string const DEF_OCLINGO_PORT; 

	/// The default host to attempt to connect to oClingo at.
	static std::string const DEF_OCLINGO_HOST;

private:
	/**************************************************************************/
	/* Members */
	/**************************************************************************/
	std::string mHost;					///< Host name to connect to.
	std::string mPort;					///< Port name to connect to.

	as2transition::TransitionFormatter mFormatter;		///< as2transition API object

	std::stringstream mTransIn;			///< Input for the translator
	std::stringstream mTransOut;		///< Output from the translator
	std::stringstream mF2lpOut;			///< Output from F2LP
	std::stringstream mOclingoOut;		///< Buffer for oClingo output.

#ifdef BOOST_TIMER
	boost::timer::cpu_timer mTimer;		///< The timer used to track the time it takes oClingo to respond.
#endif
	

	size_t mLastStep;					///< The last step that we received input for.

	bool mClosing;						///< Whether it's been requested that we close the connection.
#ifdef BOOST_TIMER
	bool mShowTime;						///< Whether we are tracking and showing the tim between each oClingo response.
#endif

	babb::utils::ref_ptr<as2transition::TransitionPath> mPlan;
	babb::utils::ref_ptr<NetworkClient> mClient;
	babb::utils::ref_ptr<bcplus::parser::BCParser> mParser;
	babb::utils::ref_ptr<cplus2asp_bin::Translator> mTranslator;

public:
	/**************************************************************************/
	/* Constructors / Destructors */
	/**************************************************************************/
	/**
	 * @brief Basic constructor. 
	 * Prepares everything but does not initialize the connections.
	 * @param host The host to establish the connection to.
	 * @param port The port to establish the connection to.
	 */
	Driver(std::string const& host = DEF_OCLINGO_HOST, std::string const& port = DEF_OCLINGO_PORT);

	/// Destructor. Closes connections, frees memory.
	inline ~Driver() {
		if (mClient) {
			if (mClient->open()) mClient->close();
		}
	}
	
	/**************************************************************************/
	/* Public Methods */
	/**************************************************************************/
	/// Runs the driver program
	int run();


	/// Gets the as2transition object used to format the oClingo output.
	inline as2transition::TransitionFormatter& formatter() { return mFormatter; }
	inline as2transition::TransitionFormatter const& formatter() const { return mFormatter; }


	/// Gets the port being used to connect to oClingo.
	inline std::string const& port() const { return mPort; }

	/// Sets the port being used to connect to oClingo
	/// @param port THe new port.
	/// @return true if the port was valid, false otherwise.
	/// TODO: Validation.
	inline bool port(std::string const& port) { mPort = port; return true; }


	/// Gets the host that we're connecting to oClingo on.
	inline std::string const& host() const { return mHost; }

	/// Sets the host that we're connecting to oClingo on.
	/// @param host the new host.
	/// @return true if the host was valid, false otherwise.
	/// TODO: Validation.
	inline bool host(std::string const& host) { mHost = host; return true; }


	/// Sets the file that we're reading the symbol table from
	/// @param symtab The new symbol table file
	/// @return True if the file was read successfully, false otherwise.
	bool symtab(std::string const& symtab);


#ifdef BOOST_TIMER
	/// Sets whether we should display the time it took oClingo to respond.
	inline void displayTime(bool time) { mShowTime = time; }
#endif

private:
	/**************************************************************************/
	/* Private Methods */
	/**************************************************************************/
		
	/**
	  * Processes a packet received by oclingo.
	  * @param error A description of any errors which have occurred. If any.
	  * @param msg The packet that was received.
	  * @param the length of the packet.
	  */
	void processOclingoPacket(boost::system::error_code const& error, char const* msg, size_t length); 

	/// Closes the active connections and exits.
	/// @param code THe code to exit with.
	void close(int code);

	/// Sends a message to oClingo.
	void send(std::string const& msg, size_t step, bool stop = false);

	/// Displays a transition to the user.
	void displayTransition(std::istream& in, std::ostream& out);

};

}}
