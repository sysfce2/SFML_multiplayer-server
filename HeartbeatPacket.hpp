#pragma once

#include "IPacket.hpp"

struct HeartbeatPacket : public IPacket {
	sf::Packet& operator<<(sf::Packet& packet) override;
	sf::Packet& operator>>(sf::Packet& packet) override;
};