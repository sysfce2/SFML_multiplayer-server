#include "NetworkServer.hpp"
#include <spdlog/spdlog.h>
#include "Events.hpp"


NetworkServer::NetworkServer(SocketStack& socketStack, EventBus& bus, const int port)
	: port(port), socketStack(socketStack), bus(bus) {
	// setup listener
	if(tcpListener.listen(port) != sf::Socket::Done) {
		spdlog::error("Cannot listen on port {}: listener is not ready.", port);
		return;
	}

	selector.add(tcpListener);
	
	spdlog::info("Correctly setup listener on port {}.", port);
}

void NetworkServer::poll() {
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
				SocketWrapper::Pointer wrapper = std::make_unique<SocketWrapper>();
				// get socket id
				SocketWrapper::ID socketId = wrapper->getId();
				// get tcpSocket by dereferencing unique_ptr
				sf::TcpSocket& tcpSocket = wrapper->getSocket();
				// try to accept incoming connection
				if(tcpListener.accept(tcpSocket) != sf::Socket::Done) {
					spdlog::error("Something went wrong while trying to accept a new connection!");
					return;
				}
				// add socket to selector
				selector.add(tcpSocket);
				// store our new socket
				socketStack.add(std::move(wrapper));
				// notify event
				bus.emit<EConnection>(socketId);
				// broadcast
				spdlog::info("A new connection has been stored correctly.");
			}
		}

		// if a socket has sent a packet, receive it
		for(auto& wrapper : socketStack.getWrappers()) {
			// get id
			SocketWrapper::ID socketId = wrapper->getId();
			// get tcpSocket by dereferencing unique_ptr
			sf::TcpSocket& sfSocket = wrapper->getSocket();

			if(selector.isReady(sfSocket)) {
				// create packet to store
				sf::Packet p;
				// handle packet event
				switch(sfSocket.receive(p)) {
					case sf::Socket::Done:
						bus.emit<EPacketReceived>(p, socketId);
						spdlog::info("A new packet has been received.");
						break;
					case sf::Socket::Disconnected:
						// remove from selector
						selector.remove(sfSocket);
						// disconnect socket
						sfSocket.disconnect();
						// remove from socketStack
						socketStack.remove(socketId);
						// broadcast
						bus.emit<EDisconnection>();
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