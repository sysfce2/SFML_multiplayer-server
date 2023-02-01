#pragma once
#include <mutex>
#include <thread>
#include <atomic>
#include "SocketStack.hpp"

enum NetworkEvent { NEW_PACKET, NEW_CONNECTION, CAN_SEND_PACKET };

class ConnectionListener {
public:
	ConnectionListener(SocketStack& socketStack, const int port);
private:
	const int port;
	SocketStack& socketStack;

	// thread-related flags
	std::atomic_bool shouldEnd		= false;
	std::atomic_bool shouldListen	= true;

	std::mutex m;
	std::condition_variable cv;

	// this flag is used to signal that a listen cycle has ended, and that the mutex has been released
	// this means that the other thread can take ownership, and that the condition variables can be awaken
	bool canCvsWakeUp				= false;

	// events
	bool hasReceivedNewPacket		= false;
	bool hasAcceptedNewConnection	= false;

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::map<Socket::Idx, sf::Packet> incomingPackets;
public:
	void start();

	bool wait(NetworkEvent event);
private:
	void listen();
};