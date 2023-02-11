#include <vector>
#include <SFML/Network.hpp>
#include "SocketStack.hpp"
#include "ConnectionListener.hpp"

#include "PacketObserver.hpp"

Subject subject;
SocketStack connections(20);
ConnectionListener listener(connections, subject, 39964);

int main() {
	subject.attach<PacketObserver>();

	while(true) {
		listener.poll();
	}

	return 0;
}