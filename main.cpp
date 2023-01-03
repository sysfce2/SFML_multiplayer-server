#include <vector>
#include <SFML/Network.hpp>
#include "SocketStack.hpp"
#include "ConnectionListener.hpp"

SocketStack connections(20);
ConnectionListener listener(connections, 39964);

int main() {

	while(true) {

	}

	return 0;
}