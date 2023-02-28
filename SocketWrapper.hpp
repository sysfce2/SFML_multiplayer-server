#pragma once

#include <memory>
#include <SFML/Network.hpp>

class SocketWrapper {
public:
	using ID = sf::Uint32;
	using Pointer = std::unique_ptr<sf::TcpSocket>;
private:
	static ID socketCount;

	SocketWrapper::ID id;
	SocketWrapper::Pointer ptr;
public:
	SocketWrapper();

	void send(sf::Packet& packet);

	auto& getPtr() { return ptr; }
	SocketWrapper::ID getIdx() { return id; }
};