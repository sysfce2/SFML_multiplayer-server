#pragma once
#include <SFML/Network/Packet.hpp>

enum class PacketType {
	C2S_PINGPACKET
};

sf::Packet& operator>>(sf::Packet& packet, PacketType& packetType);
sf::Packet& operator<<(sf::Packet& packet, PacketType& packetType);