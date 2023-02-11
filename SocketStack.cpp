#include "SocketStack.hpp"
#include <chrono>
#include "ConnectionListener.hpp"

SocketStack::SocketStack(const int maxSockets) : maxSockets(maxSockets) {
	// reserve socket space
	sockets.reserve(maxSockets);
};

int SocketStack::getSocketIdx(Socket::Pointer& socket) {
	for(int i = 0; i < sockets.size(); i++) {
		if(sockets[i] == socket) return i;
	}

	return -1;
}