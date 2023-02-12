#include "SocketStack.hpp"
#include <chrono>
#include "ConnectionListener.hpp"

SocketStack::SocketStack(const int maxSockets) : maxSockets(maxSockets) {
	// reserve socket space
	sockets.reserve(maxSockets);
};

SocketWrapper& SocketStack::getSocketWrapper(SocketWrapper::Idx idx) {
	for(auto& socket : sockets)
		if(socket.getIdx() == idx)
			return socket;
}