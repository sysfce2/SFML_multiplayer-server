#include "PingListener.hpp"
#include <spdlog/spdlog.h>

void PingListener::handle(C2SConnection e) {
	pings.emplace(e.connectionId, 0);
}

void PingListener::handle(C2SPingPacket e) {
	pings[e.connectionId] = 0;
}

void PingListener::handle(C2SDisconnection e) {
	auto& id = e.connectionId;

	std::erase_if(pings, [id](auto& ping) {
		auto const& [currentId, currentTime] = ping;
		return id == currentId; }
	);
}

void PingListener::handle(S2STick e) {
	auto elapsed = e.dt.asMilliseconds();

	for(auto it = pings.begin(); it != pings.end();) {
		auto& time			= it->second;
		const auto id		= it->first;

		time += elapsed;

		if(time >= MAX_NO_REPLY_TIME_MS) {
			spdlog::info("Lost connection with client {}.", id);

			it = pings.erase(it);
			server.disconnectClient(id);
		} else {
			++it;
		}
	}
}