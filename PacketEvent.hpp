#pragma once

#include "IEvent.hpp"
#include "Socket.hpp"

class PacketEvent : public IEvent {
private:
	sf::Packet packet;
	Socket::Pointer& socket;
public:
	PacketEvent(sf::Packet packet, Socket::Pointer& socket)
		: packet(packet), socket(socket)  {}

	sf::Packet& getPacket()			{ return packet; }
	Socket::Pointer& getSocket()	{ return socket; }

	EventType getType() const override { return PACKET; }
};