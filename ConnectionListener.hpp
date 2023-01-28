#pragma once
#include <mutex>
#include <thread>
#include <atomic>
#include "SocketStack.hpp"

class ConnectionListener {
public:
	enum Event { NEW_PACKET, CAN_SEND_PACKET, NEW_CONNECTION };

	ConnectionListener(SocketStack& socketStack, const int port);
private:
	const int port;
	SocketStack& socketStack;

	// thread-related flags
	std::atomic_bool shouldEnd;
	std::atomic_bool shouldListen;

	std::mutex m;
	std::condition_variable cv;

	bool canSendPackets;
	bool hasReceivedNewPacket;
	bool hasAcceptedNewConnection;

	sf::TcpListener listener;
	sf::SocketSelector selector;

	// int: socket's idx
	std::map<Socket::Idx, sf::Packet> incomingPackets;
public:
	void start();

	bool wait(Event event);
private:
	void listen();
};