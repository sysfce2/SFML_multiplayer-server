#pragma once

#include "IPacketListener.hpp"

class HeartbeatListener : public IPacketListener {
public:
	void handle(sf::Packet sfPacket) override;
};