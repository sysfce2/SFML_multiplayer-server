#pragma once
#include <memory>
#include "ClientConnection.hpp"

struct C2SConnection {
	ClientConnection::ID connectionId;
};

struct C2SDisconnection {
	ClientConnection::ID connectionId;
};

struct S2CPacketPreprocess {
	sf::Packet& packet;
	ClientConnection::ID connectionId;
};

struct C2SPacketPreprocess {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};

struct C2SPingPacket {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};