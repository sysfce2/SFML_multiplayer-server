#include <vector>
#include <SFML/Network.hpp>
#include "SocketStack.hpp"
#include "ConnectionListener.hpp"

SocketStack connections(20);
ConnectionListener listener(connections, 39964);

int main() {

	listener.start();

	while(true) {

		if(listener.wait(::)) {

		}

	}

	return 0;
}