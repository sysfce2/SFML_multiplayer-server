#pragma once

#include <memory>
#include <SFML/Network.hpp>

class SocketWrapper {
public:
	using Idx = sf::Uint32;
	using Pointer = std::unique_ptr<sf::TcpSocket>;
private:
	static Idx socketCount;

	SocketWrapper::Idx idx;
	SocketWrapper::Pointer ptr;
public:
	SocketWrapper();

	auto& getPtr() { return ptr; }
	SocketWrapper::Idx getIdx() { return idx; }
};