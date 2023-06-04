#include <spdlog/spdlog.h>
#include "EventBus.hpp"
#include "PingListener.hpp"
#include "NetworkServer.hpp"
#include "ConnectionStack.hpp"

// TODO: fixa read access violation quando un client si disconnette dal server (?)
// SE PIU PLAYER ESCONO INSIEME CRAASHA!

int main() {
	ConnectionStack connectionStack(20);
	NetworkServer server(connectionStack, 39964);

	spdlog::set_level(spdlog::level::debug);

	EventBus::registerListener<PingListener>(server);

	sf::Clock clock;
	while(true) {
		sf::Time dt = clock.restart();

		server.poll();
		EventBus::emit<S2STick>(dt);
	}

	return 0;
}