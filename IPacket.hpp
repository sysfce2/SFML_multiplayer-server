#pragma once

#include <SFML/Network/Packet.hpp>

class IPacket {
	virtual sf::Packet& operator<<(sf::Packet& packet) = 0;
	virtual sf::Packet& operator>>(sf::Packet& packet) = 0;
};