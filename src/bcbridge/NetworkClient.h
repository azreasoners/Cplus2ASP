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

#include <cstdlib>
#include <string>
#include <list>
#include <mutex>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include "babb/utils/memory.h"

namespace cplus2asp {
namespace bcbridge {

/**
 * @brief A client wrapper in order to facilitate interaction with oClingo.
 * Based on the boost ASIO example provided here: 
 *          http://www.boost.org/doc/libs/1_39_0/doc/html/boost_asio/example/chat/chat_client.cpp
 */
class NetworkClient : public babb::utils::Referenced
{
public:

	/************************************************************************/
	/* Constants / Types */
	/************************************************************************/
	/// Default buffer size
	static size_t const READ_BUFFER_DEFAULT;


private:
	/************************************************************************/
	/* Members */
	/************************************************************************/
	// io facilitators
  	boost::asio::io_service mIOService;						///< The service being used to communicate over the network.
  	boost::asio::ip::tcp::socket mSocket;					///< The socket being used to communicate over the network.
	boost::thread* mIOThread;								///< The thread used ot drive the IO operations
    boost::asio::ip::tcp::resolver mResolver;				///< TODO
    boost::asio::ip::tcp::resolver::query mQuery;			///< TODO
    boost::asio::ip::tcp::resolver::iterator mEndpointIterator;		///< TODO

	bool mOpen;												///< Tracks whether the client connection is open.

	// io buffers
  	std::list<std::string> mWriteBuf;						///< Buffer used to store messages that are waiting to be sent over the network.
	size_t mReadBufSz;										///< Maximum read buffer size.
	char* mReadBuf;											///< Buffer used to store the most recent message received.
	std::mutex mInternalMutex;								///< A mutex that is used to synchronize buffers.

	// io callback
	boost::function<void(boost::system::error_code const&, 
		char const* msg, size_t length)> mCallback;			///< The method we're going to call to notify the superior code of a read.
	std::mutex mExternalMutex;								///< A mutex that is used to synchronize with the superior.

public:

	/************************************************************************/
	/* Constructors / Destructors */
	/************************************************************************/

	/**
	 * Initializes the client, binding it to the socket and prepping it to read/write.
	 * @param hostname The hostname to connect to.
	 * @param port The port to connect to.
	 * @param callback The function called whenever a message is received. 
	 * @param readBufferSize The size of the read buffer.
	 * callback should have the signature (boost::system::error_code error, char const* msg, size_t length) s.t.:
	 * 		error  - describes any I/O errors that have occurred.
	 *		msg    - The buffer containing the message contents.
	 *		length - The number of bytes received.
	 */
	inline NetworkClient(
		std::string const& hostname, 
		std::string const& port, 
		boost::function<void(boost::system::error_code const& error, char const* msg, size_t length)> callback, 
		size_t readBufferSize = READ_BUFFER_DEFAULT)
	   	: mIOService(), 
		  mSocket(mIOService),
		  mResolver(mIOService),
		  mQuery(hostname, port),
		  mOpen(false)
	{
		mEndpointIterator = mResolver.resolve(mQuery);
	  	mCallback = callback;

			// initialize buffers
		mReadBufSz = readBufferSize;
		mReadBuf = new char[readBufferSize];

//		boost::asio::ip::tcp::endpoint endpoint = *mEndpointIterator;

		// Bind to port.
		//mSocket.async_connect(endpoint,
		//	boost::bind(&NetworkClient::handler_connect, this,
	//		boost::asio::placeholders::error, ++mEndpointIterator));
		
		async_connect(mSocket,  mEndpointIterator, 
			boost::bind(&NetworkClient::infinite_connect, this, 
				boost::asio::placeholders::error, boost::asio::placeholders::iterator),
			boost::bind(&NetworkClient::handler_connect, this,
				boost::asio::placeholders::error, boost::asio::placeholders::iterator));


		  mIOThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &mIOService));

	}
	
	/// Destructor. Closes the connection.
	inline ~NetworkClient() { close(); }

	/************************************************************************/
	/* Public Methods */
	/************************************************************************/
	



	/// Asynchronously writes a message to the socket.
	/// @param msg The message to write.
	/// @param Whether to append the EOM byte to the message.
  	void write(std::string const& msg, bool eom = false);

	/// Asynchronously closes the socket connection.
  	inline void close() {
		std::cout << "Closing.\n";
    	//mIOService.post(boost::bind(&NetworkClient::do_close, this));
		do_close();
//    	mIOThread->join();
		delete mIOThread;
		std::cout << "Closed.\n";
  	}

	/// Determines whether the client connection is open.
	inline bool open() { return mOpen; }

	/// Provides thread synchronization services by locking a mutex that must 
	/// be acquired by this object prior to calling the read callback.
	inline void lock() {
		mExternalMutex.lock();
	}

	/// Provides thread synchronization services by unlocking a mutex that must 
	/// be acquired by this object prior to calling the read callback.
	inline void unlock() {
		mExternalMutex.unlock();
	}

private:

	/************************************************************************/
	/* IO Handler Methods */
	/************************************************************************/
	boost::asio::ip::tcp::resolver::iterator infinite_connect(
		boost::system::error_code const& error,
		boost::asio::ip::tcp::resolver::iterator it);

	/**
	 * @brief Handles an asynchronous request to connect to a server.
	 */
	void handler_connect(const boost::system::error_code& error,
      		boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

  
	/**
	 * @brief Handles packets being received over the network asynchronously.
	 */
	void handler_read(const boost::system::error_code& error, size_t length);

	/**
	 * @brief Handles writing out messages over the socket as it becomes available.
	 */
  	void handler_write(const boost::system::error_code& error);

	/**
	 * @brief Handles closing the socket and freeing associated resources asynchronously.
	 */
  	void do_close();

};



}}
