#include "HeartbeatListener.hpp"
#include <spdlog/spdlog.h>

void HeartbeatListener::handle(sf::Packet sfPacket) {
	spdlog::debug("handled heartbeat!");
}
