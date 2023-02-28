#include "HeartbeatListener.hpp"
#include <spdlog/spdlog.h>

void HeartbeatListener::handle(sf::Packet packet) {
	spdlog::debug("handled heartbeat!");
}
