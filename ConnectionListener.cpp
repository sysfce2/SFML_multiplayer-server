#include "ConnectionListener.hpp"
#include <iostream>
#include "PacketEvent.hpp"
#include "ConnectionEvent.hpp"

ConnectionListener::ConnectionListener(SocketStack& socketStack, Subject& subject, const int port)
	: port(port), socketStack(socketStack), subject(subject) {
	// setup listener
	if(tcpListener.listen(port) != sf::Socket::Done) {
		std::cout << "Cannot listen on port " << port << ": listener is not ready." << std::endl;
		return;
	}
	
	std::cout << "Correctly setup listener on port " << port << "." << std::endl;
}

void ConnectionListener::poll() {
	// listen for these events:
	// - incoming connection
	// - incoming packet from connection
	if(selector.wait(sf::seconds(0.017f))) {
		// get sockets
		std::vector<Socket::Pointer>& sockets = socketStack.getSockets();

		// if the listener is ready for a connection, accept it
		if(selector.isReady(tcpListener)) {
			// reject new connection if we've reached our maximum number of sockets in the socketStack
			if(sockets.size() >= socketStack.maxSize()) {
				// broadcast
				std::cout << "Maximum number of sockets has been reached. Aborting new connection." << std::endl;
			} else {
				// create socket
				Socket::Pointer socket = std::make_unique<sf::TcpSocket>();
				// accept the incoming connection and assign it to the newly created socket
				tcpListener.accept(*socket);
				// notify event
				subject.notify<ConnectionEvent>(socket);
				// store our new socket
				sockets.push_back(std::move(socket));
				// broadcast
				std::cout << "A new connection has been stored correctly." << std::endl;
			}
		}

		// if a socket has sent a packet, receive it
		for(Socket::Pointer& socket : sockets)
			if(selector.isReady(*socket)) {
				// create packet to store
				sf::Packet p;
				// receive from socket
				socket->receive(p);
				// notify event
				subject.notify<PacketEvent>(p, socket);
				// broadcast
				std::cout << "A new packet has been received." << std::endl;
			}
	}
}