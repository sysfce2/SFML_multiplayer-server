#include "PacketProcessor.hpp"


// https://stackoverflow.com/questions/60086331/throw-exception-on-missing-function-overload-with-stdvariant-instead-of-compil
void PacketProcessor::process(sf::Packet& packet, ClientConnection::ID id, EventBus& bus) {
	PacketType type;
	packet >> type;

	switch(type)
	{
		case PacketType::C2S_PINGPACKET:
			bus.emit<C2SPingPacket>(id, packet);
			break;

		default:
			spdlog::warn("Tried handling unregistered packet type {}.", static_cast<int>(type));
			break;
	}
}