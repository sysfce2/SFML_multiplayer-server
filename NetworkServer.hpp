#pragma once
#include "EventBus.hpp"
#include "SocketStack.hpp"

class NetworkServer {
private:
	const int port;
	EventBus& bus;
	SocketStack& socketStack;

	sf::TcpListener tcpListener;
	sf::SocketSelector selector;
public:
	NetworkServer(SocketStack& socketStack, EventBus& bus, const int port);

	void poll();
};