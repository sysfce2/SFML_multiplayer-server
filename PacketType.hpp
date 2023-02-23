#pragma once

#include <SFML/Network/Packet.hpp>

enum class PacketType { NONE, HEARTBEAT };

sf::Packet& operator>>(sf::Packet& packet, PacketType& packetType);
sf::Packet& operator<<(sf::Packet& packet, PacketType& packetType);