#pragma once
#include <map>
#include <mutex>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <condition_variable>
#include "Socket.hpp"

class ConnectionListener;

class SocketStack {
private:
	const int maxSockets;
	std::vector<Socket::Pointer> sockets;
public:
	SocketStack(const int maxSockets);

	int maxSize() { return maxSockets; }
	int getSocketIdx(Socket::Pointer& socket);
	Socket::Pointer& getSocket(int idx) { return sockets[idx]; }

	std::vector<Socket::Pointer>& getSockets() { return sockets; }
};