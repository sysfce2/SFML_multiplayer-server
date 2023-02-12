#pragma once

#include "IEvent.hpp"
#include "SocketWrapper.hpp"

class PacketEvent : public IEvent {
private:
	sf::Packet packet;
	SocketWrapper& socket;
public:
	PacketEvent(sf::Packet packet, SocketWrapper& socket)
		: packet(packet), socket(socket)  {}

	sf::Packet& getPacket()	{ return packet; }
	SocketWrapper& getSocket() { return socket; }

	EventType getType() const override { return PACKET; }
};