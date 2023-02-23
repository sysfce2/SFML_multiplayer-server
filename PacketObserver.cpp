#include "PacketObserver.hpp"
#include <spdlog/spdlog.h>
#include "PacketType.hpp"

void PacketObserver::onEvent(const PacketEvent& event) {
	auto& sfPacket = event.getPacket();

	PacketType type;
	sfPacket >> type;
 
	this->handle(type, sfPacket);
}
