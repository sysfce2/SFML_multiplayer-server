#include "ConnectionListener.hpp"
#include <chrono>
#include <iostream>

ConnectionListener::ConnectionListener(SocketStack& socketStack, const int port) : port(port), socketStack(socketStack) {
	// setup listener
	if(listener.listen(port) != sf::Socket::Done) {
		std::cout << "Cannot listen on port " << port << ": listener is not ready." << std::endl;
		return;
	}

	// add listener to selector
	selector.add(listener);
	std::cout << "Correctly setup listener on port " << port << ". Waiting to start..." << std::endl;
}

void ConnectionListener::start() {
	std::cout << "Spawning new listener thread on port: " << port << "." << std::endl;

	// this thread is used to listen for connections
	// without blocking the rest of the program
	std::thread t(&ConnectionListener::listen, this);

	// start thread
	t.detach();
}

void ConnectionListener::listen() {
	while(shouldListen) {
		// listen for these events:
		// - incoming connection
		// - incoming packet from connection
		if(selector.wait()) {
			// get ownership of mutex
			std::lock_guard<std::mutex> lock(m);

			// reset events
			// these must be reset after we've taken ownership of the mutex
			hasReceivedNewPacket = false;
			hasAcceptedNewConnection = false;
			// ---

			// get sockets
			std::vector<Socket::Pointer>& sockets = socketStack.getSockets();

			// if a socket has sent a packet, receive it
			for(Socket::Pointer& socket : sockets)
				if(selector.isReady(*socket)) {
					// create packet to store
					sf::Packet p;
					// receive from socket
					socket->receive(p);
					// get socket idx from socket stack
					Socket::Idx idx = socketStack.getSocketIdx(socket);
					// store packet in incomingPackets
					incomingPackets.emplace(idx, p);
					// change flag required by condition variable in other thread
					hasReceivedNewPacket = true;
				}

			// if the listener is ready for a connection, accept it
			if(selector.isReady(listener)) {
				// create socket
				Socket::Pointer socket = std::make_unique<sf::TcpSocket>();
				// accept the incoming connection and assign it to the newly created socket
				listener.accept(*socket);
				// if we have reached our maximum capacity, immediately disconnect our new socket
				if(sockets.size() >= socketStack.maxSize()) {
					std::cout << "Maximum number of sockets has been reached. Aborting new connection." << std::endl;
					socket->disconnect();
				} else {
					// store our new socket
					sockets.push_back(std::move(socket));
					// change flag required by condition variable in other thread
					hasAcceptedNewConnection = true;
					std::cout << "A new connection has been stored correctly" << std::endl;
				}
			}


			// set capabilities to allow wake-up of condition variables from the other thread
			canCvsWakeUp = true;
		}

		// notify condition variables from other threads
		// KEEP THIS IN AN OUTER-SCOPE RELATIVE TO LOCK_GUARD!
		// we have to notify only after the lock has been released
		// see condition_variable#notify_one on cppreference
		cv.notify_one();
	}

	if(!shouldEnd) {
		// the thread has been paused
		// wait some time to reduce cpu usage
		std::this_thread::sleep_for(std::chrono::seconds(3));
		// recursively call this function otherwise the thread will die
		listen();
	}
}

/*
	blocks current thread until a specified event is met
*/
// MAYBE USE EVENT BUS
bool ConnectionListener::wait(NetworkEvent event) {
	// wait for a specific event
	std::unique_lock<std::mutex> lock(m);

	switch(event) {
		case NetworkEvent::CAN_SEND_PACKET:
			cv.wait(lock, [this] { return canCvsWakeUp; });
			return true;

		case NetworkEvent::NEW_PACKET:
			cv.wait(lock, [this] { return canCvsWakeUp; });
			return hasReceivedNewPacket;

		case NetworkEvent::NEW_CONNECTION:
			cv.wait(lock, [this] { return canCvsWakeUp; });
			return hasAcceptedNewConnection;

		default:
			std::cout << "ConnectionListener tried waiting for unknown event." << std::endl;
			return false;
	}

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lock.unlock();
	cv.notify_one();

	// TODO VERY IMPORTANT
	// this won't currently work
	// il motivo per cui non funzionerebbe è perchè usando un if con questa funzione di wait
	// la funzione ritorna true solo dopo che ha già ceduto il mutex all'altro thread, quindi
	// non ha alcun senso bloccare il thread visto che aspettiamo solo di ritornare l'evento
	// che ci serve senza aspettare la funzione che viene specificata dal programmatore.
	// una possibile soluzione potrebbe essere di create una funziona che sblocca il lock e notifica
	// la condition variable. Questo ha come lato negativo che il programmatore dovrà per forza
	// utilizzare questa funzione alla fine della sua, altrimenti il lock non viene mai rilasciato
	// e il listen non continuerà, bloccando fondamentalmente il server.
	return false;
}