#include "ConnectionListener.hpp"
#include <chrono>
#include <iostream>

ConnectionListener::ConnectionListener(SocketStack& socketStack, const int port) : port(port), socketStack(socketStack) {
	// setup listener
	if(listener.listen(port) != sf::Socket::Done) {
		std::cout << "Cannot listen on port " << port << ": listener is not ready." << std::endl;
		return;
	}

	// add listener to selector
	selector.add(listener);
	std::cout << "Correctly setup listener on port " << port << ". Waiting..." << std::endl;
}

void ConnectionListener::start() {
	std::cout << "Spawning new listener thread on port: " << port << "." << std::endl;

	// this thread is used to listen for connections
	// without blocking the rest of the program
	std::thread t(&ConnectionListener::listen, this);

	// start thread
	t.detach();
}

void ConnectionListener::listen() {
	while(shouldListen) {
		// poll events:
		// - packet received,		handled using socket.receive
		// - connection by client,	handled using listener.accept
		if(selector.wait()) {
			// reset flags
			canSendPackets = false;
			hasReceivedNewPacket = false;
			hasAcceptedNewConnection = false;

			// get ownership of sockets vector
			std::lock_guard<std::mutex> lock(m);
			// get sockets from socketStack
			std::vector<Socket::Pointer>& sockets = socketStack.getSockets();

			// if a socket has sent a packet, receive it
			for(Socket::Pointer& socket : sockets)
				if(selector.isReady(*socket)) {
					// create packet to store
					sf::Packet p;
					// receive from socket
					socket->receive(p);
					// store packet in incomingPackets
					incomingPackets.emplace(socket, p);
					// change flag required by condition variable in other thread
					hasReceivedNewPacket = true;
				}

			// if the listener is ready for a connection, accept it
			if(selector.isReady(listener)) {
				// create socket
				Socket::Pointer socket = std::make_unique<sf::TcpSocket>();
				// accept the incoming connection and assign it to the newly created socket
				listener.accept(*socket);
				// if we have reached our maximum capacity, immediately disconnect our new socket
				if(sockets.size() >= socketStack.maxSize()) {
					std::cout << "Maximum number of sockets has been reached. Aborting new connection." << std::endl;
					socket->disconnect();
				} else {
					// store our new socket
					sockets.push_back(std::move(socket));
					// change flag required by condition variable in other thread
					hasAcceptedNewConnection = true;
					std::cout << "A new connection has been stored correctly" << std::endl;
				}
			}


			// change flag required by condition variable in other thread
			canSendPackets = true;
		}

		// notify all waiting thread that they can ready from the vectors
		cv.notify_all();
	}

	if(!shouldEnd) {
		// if we're here, it means that this SocketStack was paused
		// wait some time to reduce cpu usage
		std::this_thread::sleep_for(std::chrono::seconds(3));
		// recursively call this function otherwise the thread will die as "isWorking" is set to false
		listen();
	}
}

bool ConnectionListener::wait(Event event) {
	// wait for a specific event
	std::unique_lock<std::mutex> lock(m);

	switch(event) {
		case Event::CAN_SEND_PACKET:
			cv.wait(lock, [this] { return canSendPackets; });
			break;

		case Event::NEW_PACKET:
			cv.wait(lock, [this] { return hasReceivedNewPacket; });
			break;

		case Event::NEW_CONNECTION:
			cv.wait(lock, [this] { return hasAcceptedNewConnection; });
			break;

		default:
			// something went wrong
			lock.unlock();
			std::cout << "ConnectionListener tried waiting for unknown event." << std::endl;
			return false;
	}

	return true;
}