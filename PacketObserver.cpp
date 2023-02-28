#include "PacketObserver.hpp"
#include "PacketType.hpp"

#include "HeartbeatListener.hpp"

PacketObserver::PacketObserver() {
	this->registerListener<HeartbeatListener>(PacketType::HEARTBEAT);

	sf::Packet p;
	PacketType t = PacketType::HEARTBEAT;
	p << t;
	SocketWrapper s;
	PacketEvent e(p, s);
	onEvent(e);
}

void PacketObserver::onEvent(const PacketEvent& event) {
	auto& packet = event.getPacket();

	PacketType type;
	packet >> type;

	auto& listenersByType = listeners[type];
	for(auto& listener : listenersByType) {
		listener->handle(packet);
	}
}
