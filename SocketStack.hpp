#pragma once
#include <map>
#include <mutex>
#include <memory>
#include <thread>
#include <iostream>
#include <condition_variable>
#include <SFML/Network.hpp>

class SocketStack {
public:
	typedef std::unique_ptr<sf::TcpSocket> SocketPointer;

	SocketStack(const int maxSockets);
private:
	const int maxSockets;
	std::vector<SocketPointer> sockets;
public:
	void sendToSocket(int socketIdx, sf::Packet packet);
	
	int maxSize() { return maxSockets; }
	std::vector<SocketPointer>& getSockets() { return sockets; }
};