#pragma once
#include <vector>
#include "SocketWrapper.hpp"

class ConnectionListener;

class SocketStack {
private:
	const int maxSockets;
	std::vector<SocketWrapper> sockets;
public:
	SocketStack(const int maxSockets);

	int maxSize() { return maxSockets; }
	SocketWrapper& getSocketWrapper(SocketWrapper::Idx idx);

	auto& getSockets() { return sockets; }
};