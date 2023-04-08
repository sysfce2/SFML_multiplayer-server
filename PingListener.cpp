#include "PingListener.hpp"
#include <spdlog/spdlog.h>

void PingListener::handle(C2SConnection e) {
	pings.emplace(e.connectionId, 0);
}

void PingListener::handle(C2SDisconnection e) {
	pings.erase(e.connectionId);
}

void PingListener::handle(C2SPingPacket e) {
	pings[e.connectionId] = 0;
}

void PingListener::handle(S2STick e) {
	auto elapsed = e.dt.asMilliseconds();

	for(auto& [id, time] : pings) {
		time += elapsed;

		if(time >= MAX_NO_REPLY_TIME_MS)
			server.disconnectClient(id);
	}
}