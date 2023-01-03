#pragma once
#include <mutex>
#include <thread>
#include <atomic>
#include <SFML/Network.hpp>
#include "SocketStack.hpp"

class ConnectionListener {
public:
	ConnectionListener(SocketStack& socketStack, const int port);
private:
	const int port;
	SocketStack& socketStack;

	// thread-related flags
	std::atomic_bool shouldEnd;
	std::atomic_bool shouldListen;

	// read: https://stackoverflow.com/questions/7555321/why-to-pass-mutex-as-a-parameter-to-a-function-being-called-by-a-thread
	std::mutex m;
	std::condition_variable cv;

	bool canSendPackets;
	bool hasReceivedNewPacket;
	bool hasAcceptedNewConnection;

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::map<SocketStack::SocketPointer, sf::Packet> incomingPackets;
public:
	void start();
private:
	void listen();
};