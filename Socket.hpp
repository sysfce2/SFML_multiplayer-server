#pragma once

#include <memory>
#include <SFML/Network.hpp>

// this struct is a wrapper to sf::TcpSocket.
// the point of this is to also store the socket's id
// alongside the actual socket from sfml
class Socket {
public:
	typedef int Idx;
	typedef std::unique_ptr<sf::TcpSocket> Pointer;
private:
	Socket::Idx idx;
	Socket::Pointer& sfTcpSocket;
public:
	Socket::Idx getIdx();
};