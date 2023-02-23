#include "ConnectionListener.hpp"
#include <spdlog/spdlog.h>
#include "PacketEvent.hpp"
#include "ConnectionEvent.hpp"


ConnectionListener::ConnectionListener(SocketStack& socketStack, Subject& subject, const int port)
	: port(port), socketStack(socketStack), subject(subject) {
	// setup listener
	if(tcpListener.listen(port) != sf::Socket::Done) {
		spdlog::error("Cannot listen on port {}: listener is not ready.", port);
		return;
	}

	selector.add(tcpListener);
	
	spdlog::info("Correctly setup listener on port {}.", port);
}

void ConnectionListener::poll() {
	// listen for these events:
	// - incoming connection
	// - incoming packet from connection
	if(selector.wait(sf::seconds(0.017f))) {
		// if the listener is ready for a connection, accept it
		if(selector.isReady(tcpListener)) {
			// reject new connection if we've reached our maximum number of sockets in the socketStack
			if(socketStack.size() >= socketStack.maxSize()) {
				// broadcast
				spdlog::warn("Maximum number of sockets has been reached. Aborting new connection.");
			} else {
				// create socket
				SocketWrapper socket;
				// get tcpSocket by dereferencing unique_ptr
				sf::TcpSocket& tcpSocket = *socket.getPtr();
				// try to accept incoming connection
				if(tcpListener.accept(tcpSocket) != sf::Socket::Done) {
					spdlog::error("Something went wrong while trying to accept a new connection!");
					return;
				}
				// add socket to selector
				selector.add(tcpSocket);
				// notify event
				subject.notify<ConnectionEvent>(socket);
				// store our new socket
				socketStack.add(std::move(socket));
				// broadcast
				spdlog::info("A new connection has been stored correctly.");
			}
		}

		// if a socket has sent a packet, receive it
		for(SocketWrapper& socket : socketStack.getSockets()) {
			// get tcpSocket by dereferencing unique_ptr
			sf::TcpSocket& sfSocket = *socket.getPtr();

			if(selector.isReady(sfSocket)) {
				// create packet to store
				sf::Packet p;
				// handle packet event
				switch(sfSocket.receive(p)) {
					case sf::Socket::Done:
						subject.notify<PacketEvent>(p, socket);
						spdlog::info("A new packet has been received.");
						break;
					case sf::Socket::Disconnected:
						// remove from selector
						selector.remove(sfSocket);
						// disconnect socket
						sfSocket.disconnect();
						// remove from socketStack
						socketStack.remove(socket.getIdx());
						spdlog::info("A client has disconnected.");
						break;
					default:
						spdlog::error("A packet has been received, but it has an unhandled state!");
						break;
				}
			}
		}
	}
}