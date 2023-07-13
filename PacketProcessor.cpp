#include "PacketProcessor.hpp"
#include <spdlog/spdlog.h>

void PacketProcessor::process(sf::Packet& packet, ClientConnection::ID id) {
	PacketType type;
	packet >> type;

	switch(type)
	{
		case PacketType::C2S_PING:
			EventBus::emit<C2SPingPacket>(id, packet);
			break;

		default:
			spdlog::warn("Tried handling unregistered packet type {}.", static_cast<int>(type));
			break;
	}
}