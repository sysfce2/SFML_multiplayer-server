#include "NetworkServer.hpp"
#include <spdlog/spdlog.h>
#include "Events.hpp"
#include "PacketBuilder.hpp"


NetworkServer::NetworkServer(ConnectionStack& connectionStack, const int port)
	: port(port), connectionStack(connectionStack), packetProcessor() {
	// setup listener
	if(tcpListener.listen(port) != sf::Socket::Done) {
		spdlog::error("Cannot listen on port {}: is a listener already attached?", port);
		return;
	}

	selector.add(tcpListener);
	
	spdlog::info("Correctly setup listener on port {}.", port);
}

void NetworkServer::poll() {
	if(selector.wait(sf::seconds(1.0f))) {
		processNewConnections();
		processIncomingPackets();
	}
}

void NetworkServer::processIncomingPackets() {
	auto& connections = connectionStack.getConnections();

	for(auto it = connections.begin(); it != connections.end();) {
		auto connection = it->get();

		ClientConnection::ID connectionId = connection->getId();
		sf::TcpSocket& socket = connection->getSocket();

		if(!selector.isReady(socket))
			return;

		sf::Packet p;
		bool hasClientDisconnected = false;

		switch(socket.receive(p)) {
			case sf::Socket::Done:
				// first, emit general "packet received" event
				EventBus::emit<C2SPacketPreprocess>(connectionId, p);
				// then, process the packet and emit correct, specific event
				packetProcessor.process(p, connectionId);
				spdlog::debug("A new packet has been received.");
				break;

			case sf::Socket::Disconnected:
				hasClientDisconnected = true;
				disconnectClient(connectionId, false);
				spdlog::info("A client has disconnected.");
				break;

			default:
				spdlog::error("A packet has been received, but it has an unhandled SFML state!");
				break;
		}

		it = hasClientDisconnected ? connections.erase(it) : ++it;
	}
}

void NetworkServer::processNewConnections() {
	if(!selector.isReady(tcpListener))
		return;

	if(connectionStack.size() >= connectionStack.maxSize()) {
		spdlog::warn("Maximum number of sockets has been reached. Closing new connection.");
		return;
	}

	ClientConnection::Pointer connection		= std::make_unique<ClientConnection>();
	ClientConnection::ID connectionId			= connection->getId();
	sf::TcpSocket& socket						= connection->getSocket();

	if(tcpListener.accept(socket) != sf::Socket::Done) {
		spdlog::error("Something went wrong while trying to accept a new connection!");
		return;
	}

	// handle connection
	selector.add(socket);
	connectionStack.add(std::move(connection));
	EventBus::emit<C2SConnection>(connectionId);

	spdlog::info("A new connection has been stored correctly.");

	// broadcast new connection
	broadcastNewConnection(connectionId);
	spdlog::debug("Broadcasting new connection to connected clients.");
}

void NetworkServer::broadcastNewConnection(ClientConnection::ID id) {
	sf::Packet p = PacketBuilder::build(PacketType::S2C_NEW_CLIENT);

	// TODO implement packet

	broadcastExcept(id, p);
}

// TODO: make a networkpacket sf::packet wrapper with packettype as m variable
void NetworkServer::send(ClientConnection::ID id, sf::Packet& p) {
	auto& connection = connectionStack.getConnection(id);
	auto& socket = connection.getSocket();

	spdlog::debug("Sending packet to ID [{}]", id);
	socket.send(p);
}

void NetworkServer::broadcast(sf::Packet& p) {
	for(ClientConnection::Pointer& c : connectionStack.getConnections())
		send(c->getId(), p);
}

void NetworkServer::broadcastExcept(ClientConnection::ID excludeId, sf::Packet& p) {
	for(ClientConnection::Pointer& c : connectionStack.getConnections()) {
		ClientConnection::ID currentId = c->getId();

		if(currentId == excludeId)
			continue;

		send(currentId, p);
	}
}

void NetworkServer::disconnectClient(ClientConnection::ID id) {
	disconnectClient(id, true);
}

void NetworkServer::disconnectClient(ClientConnection::ID id, bool removeFromConnections) {
	auto& connection = connectionStack.getConnection(id);
	auto& socket = connection.getSocket();

	EventBus::emit<C2SDisconnection>(id);
	selector.remove(socket);

	if(removeFromConnections)
		connectionStack.remove(id);
}