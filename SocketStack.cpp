#include "SocketStack.hpp"
#include <chrono>

SocketStack::SocketStack(const int maxSockets) : maxSockets(maxSockets) {
	// reserve socket space
	sockets.reserve(maxSockets);
};

void SocketStack::sendToSocket(int socketIdx, sf::Packet packet) {
	// wait for cv
	std::unique_lock<std::mutex> lock(...);
}

void SocketStack::startProcessingPackets() {
	// wait until a new packet has been received
	std::unique_lock<std::mutex> lock(packetsMutex);
	cvPacketEvent.wait(lock, [this] { return shouldProcessNewPacket; });
}

void SocketStack::endProcessingPackets() {

}