#pragma once

#include "IEvent.hpp"
#include "Socket.hpp"

class ConnectionEvent : public IEvent {
private:
	Socket::Pointer& socket;
public:
	ConnectionEvent(Socket::Pointer& socket)
		: socket(socket) {}

	Socket::Pointer& getSocket() { return socket; }

	EventType getType() const override { return CONNECTION; }
};