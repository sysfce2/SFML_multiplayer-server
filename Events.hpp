#pragma once
#include <memory>
#include "ClientConnection.hpp"

struct S2STick {
	sf::Time dt;
};

struct C2SConnection {
	ClientConnection::ID connectionId;
};

struct C2SDisconnection {
	ClientConnection::ID connectionId;
};

struct S2CPacketPreprocess {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};

struct C2SPacketPreprocess {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};

struct C2SPingPacket {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};