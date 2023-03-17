#include "NetworkServer.hpp"
#include <spdlog/spdlog.h>
#include "Events.hpp"


NetworkServer::NetworkServer(ConnectionStack& connectionStack, EventBus& bus, const int port)
	: port(port), connectionStack(connectionStack), bus(bus), packetProcessor() {
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
			if(connectionStack.size() >= connectionStack.maxSize()) {
				// broadcast
				spdlog::warn("Maximum number of sockets has been reached. Aborting new connection.");
			} else {
				// create socket
				ClientConnection::Pointer connection = std::make_unique<ClientConnection>();
				// get socket id
				ClientConnection::ID connectionId = connection->getId();
				// get tcpSocket by dereferencing unique_ptr
				sf::TcpSocket& socket = connection->getSocket();
				// try to accept incoming connection
				if(tcpListener.accept(socket) != sf::Socket::Done) {
					spdlog::error("Something went wrong while trying to accept a new connection!");
					return;
				}
				// add socket to selector
				selector.add(socket);
				// store our new socket
				connectionStack.add(std::move(connection));
				// notify event
				bus.emit<C2SConnection>(connectionId);
				// broadcast
				spdlog::info("A new connection has been stored correctly.");
			}
		}

		// if a socket has sent a packet, receive it
		for(auto& connection : connectionStack.getConnections()) {
			// get id
			ClientConnection::ID connectionId = connection->getId();
			// get tcpSocket by dereferencing unique_ptr
			sf::TcpSocket& socket = connection->getSocket();

			if(selector.isReady(socket)) {
				// create packet to store
				sf::Packet p;
				// handle packet event
				switch(socket.receive(p)) {
					case sf::Socket::Done:
						// first, emit general "packet received" event
						bus.emit<C2SPacketPreprocess>(connectionId, p);
						// then, process the packet and emit correct, specific event
						packetProcessor.process(p, connectionId, bus);
						spdlog::info("A new packet has been received.");
						break;
					case sf::Socket::Disconnected:
						// broadcast
						bus.emit<C2SDisconnection>(connectionId);
						// remove from selector
						selector.remove(socket);
						// disconnect socket
						socket.disconnect();
						// remove from socketStack
						connectionStack.remove(connectionId);
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

void NetworkServer::send(ClientConnection::ID id, PacketType type, sf::Packet& p) {
	auto& c = connectionStack.getConnection(id);
	auto& s = c.getSocket();

	p << type;

	s.send(p);
}