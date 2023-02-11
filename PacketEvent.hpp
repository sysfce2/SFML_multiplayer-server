#pragma once

#include "IEvent.hpp"
#include "Socket.hpp"

class PacketEvent : public IEvent {
private:
	sf::Packet m_Packet;
	Socket::Pointer& m_Socket;
public:
	PacketEvent(sf::Packet packet, Socket::Pointer& socket)
		: m_Packet(packet), m_Socket(socket)  {}

	sf::Packet& getPacket()			{ return m_Packet; }
	Socket::Pointer& getSocket()	{ return m_Socket; }

	EventType getType() const override { return PACKET; }
};