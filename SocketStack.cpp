#include "SocketStack.hpp"
#include <chrono>
#include "ConnectionListener.hpp"

SocketStack::SocketStack(const int maxSockets) : maxSockets(maxSockets) {
	// reserve socket space
	sockets.reserve(maxSockets);
};

void SocketStack::sendTo(Socket::Idx socketIdx, sf::Packet packet, ConnectionListener& listener) {
	if(listener.wait(ConnectionListener::CAN_SEND_PACKET)) {
		Socket::Pointer& socket = getSocket(socketIdx);
		socket->send(packet);
	}
}

int SocketStack::getSocketIdx(Socket::Pointer& socket) {
	for(int i = 0; i < sockets.size(); i++) {
		if(sockets[i] == socket) return i;
	}

	return -1;
}