#pragma once
#include "Subject.hpp"
#include "SocketStack.hpp"

class ConnectionListener {
private:
	const int port;
	Subject& subject;
	SocketStack& socketStack;

	sf::TcpListener tcpListener;
	sf::SocketSelector selector;
public:
	ConnectionListener(SocketStack& socketStack, Subject& subject, const int port);

	void poll();
};