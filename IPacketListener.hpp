#pragma once

#include "Packets.hpp"

class IPacketListener {
public:
	virtual ~IPacketListener() {}

	// we don't take a reference to the sf::Packet otherwise once
	// we've passed it to a function for the first time
	// its member variables will have been consumed!
	virtual void handle(sf::Packet sfPacket) = 0;
};