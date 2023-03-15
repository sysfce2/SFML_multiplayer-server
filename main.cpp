#include <spdlog/spdlog.h>
#include "SocketStack.hpp"
#include "NetworkServer.hpp"

#include "EventBus.hpp"

// TODO: pensa se lasciare delle cose condivise tra server e client o separarle (pur ricopiandole per ora)
// magari crea uno shared items progetto su visual studio e metti solo gli enum in comune, però lasciamo le dichiarazioni e definizioni delle cose separate per i progetti
// (anche se per ora sono uguali)
// ed aggiungere anche zlib per compressione pacchetti

int main() {
	//Subject subject;

	EventBus bus;
	SocketStack connections(20);
	NetworkServer server(connections, bus, 39964);

	spdlog::set_level(spdlog::level::debug);

	while(true) {
		server.poll();
	}

	return 0;
}