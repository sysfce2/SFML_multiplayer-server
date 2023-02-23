#pragma once

#include "PacketType.hpp"

struct PacketWrapper {
	virtual ~PacketWrapper() {}

	virtual PacketType getType() { return PacketType::NONE; }
};

struct HeartbeatPacket : PacketWrapper {
	short int beatID = rand();

	PacketType getType() override { return PacketType::HEARTBEAT; }
};