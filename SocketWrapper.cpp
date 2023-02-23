#include "SocketWrapper.hpp"
#include <spdlog/spdlog.h>

sf::Uint32 SocketWrapper::socketCount = 0;

SocketWrapper::SocketWrapper()
	: id(socketCount), ptr(std::make_unique<sf::TcpSocket>()) {

	++socketCount;
}

void SocketWrapper::send(PacketWrapper& packet) {
	// this packet will be sent over the network
	sf::Packet sfPacket;
	PacketType type = packet.getType();

	// add our own member variables to the sf::Packet
	// ALWAYS PUT OUR PACKET TYPE BEFORE ANY OTHER VARIABLE!
	switch(type) {
		default:
			spdlog::error("Tried sending packet with UNKNOWN type!");
			return;
		case PacketType::NONE:
			spdlog::error("Tried sending a packet with type: NONE!");
			return;
		case PacketType::HEARTBEAT:
			auto& p = dynamic_cast<HeartbeatPacket&>(packet);
			sfPacket << type << p.beatID;
			break;
	}

	spdlog::debug("Sent packet with type: {}", static_cast<int>(type));
	// send sf::Packet
	ptr->send(sfPacket);
}