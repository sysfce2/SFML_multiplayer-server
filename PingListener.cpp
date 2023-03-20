#include "PingListener.hpp"
#include <spdlog/spdlog.h>

void PingListener::handle(C2SConnection e) {
	pings.emplace(e.connectionId, 0);
	spdlog::debug("called connection");
}

void PingListener::handle(C2SDisconnection e) {
	pings.erase(e.connectionId);
	spdlog::debug("called disconnection");

}

void PingListener::handle(C2SPingPacket e) {
	pings[e.connectionId] = 0;
	spdlog::debug("called ping");

}

void PingListener::handle(S2STick e) {
	auto elapsed = e.dt.asMilliseconds();

	for(auto& [id, time] : pings)
		time += elapsed;

	spdlog::debug("called tick, added {}", elapsed);
}