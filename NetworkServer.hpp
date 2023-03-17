#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include "EventBus.hpp"
#include "PacketType.hpp"
#include "ConnectionStack.hpp"
#include "PacketProcessor.hpp"

class NetworkServer {
private:
	const int port;

	EventBus& bus;
	PacketProcessor packetProcessor;
	ConnectionStack& connectionStack;

	sf::TcpListener tcpListener;
	sf::SocketSelector selector;
public:
	NetworkServer(ConnectionStack& connections, EventBus& bus, const int port);

	void poll();

	// TODO: maybe move this somewhere else
	void send(ClientConnection::ID id, PacketType type, sf::Packet& p);
};