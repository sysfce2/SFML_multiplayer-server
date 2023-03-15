#pragma once

#include <memory>
#include <SFML/Network.hpp>

class SocketWrapper {
public:
	using ID = sf::Uint32;
	using Pointer = std::unique_ptr<SocketWrapper>;
private:
	static ID socketCount;

	sf::TcpSocket socket;
	SocketWrapper::ID id;
public:
	SocketWrapper();

	auto& getSocket() { return socket; }
	SocketWrapper::ID getId() { return id; }
};