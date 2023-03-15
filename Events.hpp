#pragma once
#include "SocketWrapper.hpp"

struct EDisconnection {};

struct EConnection {
	SocketWrapper::ID socketId;
};

struct EPacketSent {
	SocketWrapper::ID socketId;
};

struct EPacketReceived {
	sf::Packet packet;
	SocketWrapper::ID socketId;
};
