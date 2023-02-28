#pragma once
#include <vector>
#include "SocketWrapper.hpp"

class SocketStack {
private:
	const size_t maxSockets;
	std::vector<SocketWrapper> sockets;
public:
	SocketStack(const size_t maxSockets);

	size_t size() { return sockets.size(); }
	size_t maxSize() { return maxSockets; }

	void add(SocketWrapper&& socket);
	void remove(SocketWrapper::ID id);

	auto& getSockets() { return sockets; }

	SocketWrapper& getSocketWrapper(SocketWrapper::ID id);
};