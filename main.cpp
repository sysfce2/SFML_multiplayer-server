#include <spdlog/spdlog.h>
#include "EventBus.hpp"
#include "NetworkServer.hpp"
#include "ConnectionStack.hpp"
#include "PacketProcessor.hpp"

// TODO: pensa se lasciare delle cose condivise tra server e client o separarle (pur ricopiandole per ora)
// magari crea uno shared items progetto su visual studio e metti solo gli enum in comune, però lasciamo le dichiarazioni e definizioni delle cose separate per i progetti
// (anche se per ora sono uguali)
// ed aggiungere anche zlib per compressione pacchetti

int main() {
	EventBus bus;
	ConnectionStack connectionStack(20);
	NetworkServer server(connectionStack, bus, 39964);

	spdlog::set_level(spdlog::level::debug);

	while(true) {
		server.poll();
	}

	return 0;
}