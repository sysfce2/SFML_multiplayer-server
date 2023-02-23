#include <spdlog/spdlog.h>
#include "SocketStack.hpp"
#include "ConnectionListener.hpp"
#include "PacketType.hpp"
#include "PacketObserver.hpp"
#include "ConnectionObserver.hpp"

#include "HeartbeatListener.hpp"


// TODO: pensa se lasciare delle cose condivise tra server e client o separarle (pur ricopiandole per ora)
// magari crea uno shared items progetto su visual studio e metti solo gli enum in comune, però lasciamo le dichiarazioni e definizioni delle cose separate per i progetti
// (anche se per ora sono uguali)
// ed aggiungere anche zlib per compressione pacchetti
int main() {
	Subject subject;
	SocketStack connections(20);
	ConnectionListener listener(connections, subject, 39964);

	spdlog::set_level(spdlog::level::debug);

	auto& packetObserver = subject.attach<PacketObserver>();
	packetObserver.registerListener<HeartbeatListener>(PacketType::HEARTBEAT);

	packetObserver.handle(PacketType::HEARTBEAT, sf::Packet{});

	subject.attach<ConnectionObserver>();

	while(true) {
		listener.poll();
	}

	return 0;
}