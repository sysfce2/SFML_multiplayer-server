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
	if(selector.wait(sf::seconds(0.016f))) {
		processNewConnections();
		processIncomingPackets();
	}
}

void NetworkServer::processIncomingPackets() {
	for(auto& connection : connectionStack.getConnections()) {
		ClientConnection::ID connectionId	= connection->getId();
		sf::TcpSocket&	socket				= connection->getSocket();

		if(!selector.isReady(socket))
			return;

		sf::Packet p;
		switch(socket.receive(p)) {
			case sf::Socket::Done:
				// first, emit general "packet received" event
				bus.emit<C2SPacketPreprocess>(connectionId, p);
				// then, process the packet and emit correct, specific event
				packetProcessor.process(p, connectionId, bus);
				spdlog::info("A new packet has been received.");
				break;

			case sf::Socket::Disconnected:
				disconnectClient(connectionId);
				spdlog::info("A client has disconnected.");
				break;

			default:
				spdlog::error("A packet has been received, but it has an unhandled state!");
				break;
		}
	}
}

void NetworkServer::processNewConnections() {
	if(!selector.isReady(tcpListener))
		return;

	if(connectionStack.size() >= connectionStack.maxSize()) {
		spdlog::warn("Maximum number of sockets has been reached. Aborting new connection.");
		return;
	}

	ClientConnection::Pointer connection		= std::make_unique<ClientConnection>();
	ClientConnection::ID connectionId			= connection->getId();
	sf::TcpSocket& socket						= connection->getSocket();

	if(tcpListener.accept(socket) != sf::Socket::Done) {
		spdlog::error("Something went wrong while trying to accept a new connection!");
		return;
	}

	// handle conncetion
	selector.add(socket);
	connectionStack.add(std::move(connection));
	bus.emit<C2SConnection>(connectionId);

	spdlog::info("A new connection has been stored correctly.");
}

void NetworkServer::sendTo(ClientConnection::ID id, PacketType type, sf::Packet& p) {
	auto& connection = connectionStack.getConnection(id);
	auto& socket = connection.getSocket();

	p << type;

	socket.send(p);
}

void NetworkServer::disconnectClient(ClientConnection::ID id) {
	auto& connection = connectionStack.getConnection(id);
	auto& socket = connection.getSocket();

	// !! don't change order :)
	bus.emit<C2SDisconnection>(id);
	selector.remove(socket);
	socket.disconnect();
	connectionStack.remove(id);
}