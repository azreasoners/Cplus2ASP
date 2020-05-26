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


#include <cstdlib>
#include <string>
#include <list>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include "NetworkClient.h"

namespace cplus2asp {
namespace bcbridge {

// size of read buffer
size_t const NetworkClient::READ_BUFFER_DEFAULT = 4096;


// Queues a packet for asynchronous writing.
void NetworkClient::write(std::string const& msg, bool eom) {

	mInternalMutex.lock();

	bool write_in_progress = !mWriteBuf.empty();
	mWriteBuf.push_back("");
	mWriteBuf.back() = msg; 
	if (eom) mWriteBuf.back() += '\0';

	mInternalMutex.unlock();
	
	if (!write_in_progress)
	{
		// Request to write if we aren't already.
		boost::asio::async_write(mSocket,
	  		boost::asio::buffer(mWriteBuf.front().data(), mWriteBuf.front().size()),
	  		boost::bind(&NetworkClient::handler_write, this,
				boost::asio::placeholders::error));
	}

}


boost::asio::ip::tcp::resolver::iterator NetworkClient::infinite_connect(
		const boost::system::error_code& ec,
		boost::asio::ip::tcp::resolver::iterator it) {
	return mEndpointIterator;
}

// Handles a request to connect to a server.
void NetworkClient::handler_connect(const boost::system::error_code& error,
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
	if (!error)
	{
		// Queue the first reqd.
		mSocket.async_receive(
  			boost::asio::buffer(mReadBuf, mReadBufSz),
  			boost::bind(&NetworkClient::handler_read, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		mOpen = true;
	
	}
	else if (mEndpointIterator!= boost::asio::ip::tcp::resolver::iterator())
	{
		// Something bad happened.
		// Attempt to restart the socket.
		mSocket.close();
/*
		boost::asio::ip::tcp::endpoint endpoint = *mEndpointIterator;
		mSocket.async_connect(endpoint,
			boost::bind(&NetworkClient::handler_connect, this,
				boost::asio::placeholders::error, ++mEndpointIterator));
		std::cerr << "Error Connecting.\n";
*/
		async_connect(mSocket,  mEndpointIterator, 
			boost::bind(&NetworkClient::infinite_connect, this, 
				boost::asio::placeholders::error, boost::asio::placeholders::iterator),
			boost::bind(&NetworkClient::handler_connect, this,
				boost::asio::placeholders::error, boost::asio::placeholders::iterator));
			

	}
}

// Handles a read from the network.
void NetworkClient::handler_read(const boost::system::error_code& error, size_t bytes) {
	mExternalMutex.lock();

	// Notify the superior party
	mCallback(error, mReadBuf, bytes);

	mExternalMutex.unlock();

	// make sure to go request another packet.
	mSocket.async_receive(
  		boost::asio::buffer(mReadBuf, mReadBufSz),
  		boost::bind(&NetworkClient::handler_read, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}


// Sends the next packet over the network.
void NetworkClient::handler_write(const boost::system::error_code& error) {
	if (!error)
	{
			mInternalMutex.lock();
			mWriteBuf.pop_front();
			mInternalMutex.unlock();

			if (!mWriteBuf.empty()) {
				boost::asio::async_write(mSocket,
					boost::asio::buffer(mWriteBuf.front().data(), mWriteBuf.front().size()),
					boost::bind(&NetworkClient::handler_write, this,
						boost::asio::placeholders::error));
			}
	}
	else {
		do_close();
	}
}

// closes the socket
void NetworkClient::do_close() {
	mSocket.close();
	mOpen = false;
}

}}
