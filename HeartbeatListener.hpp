#pragma once

#include <SFML/Network/Packet.hpp>
#include "IPacketListener.hpp"

class HeartbeatListener : public IPacketListener {
public:
	void handle(sf::Packet packet) override;
};