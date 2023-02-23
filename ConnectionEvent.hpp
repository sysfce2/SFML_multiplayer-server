#pragma once

#include "IEvent.hpp"
#include "SocketWrapper.hpp"

class ConnectionEvent : public IEvent {
private:
	SocketWrapper& socket;
public:
	ConnectionEvent(SocketWrapper& socket)
		: socket(socket) {}

	SocketWrapper& getSocket() const { return socket; }
};