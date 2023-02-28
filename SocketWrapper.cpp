#include "SocketWrapper.hpp"
#include <spdlog/spdlog.h>

SocketWrapper::ID SocketWrapper::socketCount = 0;

SocketWrapper::SocketWrapper()
	: id(socketCount), ptr(std::make_unique<sf::TcpSocket>()) {

	++socketCount;
}

// TODO: maybe remove this and just call getPtr then send the packet directly from sfml socket
void SocketWrapper::send(sf::Packet& packet) {
	ptr->send(packet);
}