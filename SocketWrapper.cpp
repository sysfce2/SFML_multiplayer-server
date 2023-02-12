#include "SocketWrapper.hpp"

sf::Uint32 SocketWrapper::socketCount = 0;

SocketWrapper::SocketWrapper()
	: idx(), ptr(std::make_unique<sf::TcpSocket>()) {

	idx = socketCount;
	++socketCount;
}