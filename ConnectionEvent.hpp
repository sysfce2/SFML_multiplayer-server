#pragma once

#include "IEvent.hpp"
#include "Socket.hpp"

class ConnectionEvent : public IEvent {
private:
	Socket::Pointer& m_Socket;
public:
	ConnectionEvent(Socket::Pointer& socket)
		: m_Socket(socket) {}

	Socket::Pointer& getSocket() { return m_Socket; }

	EventType getType() const override { return CONNECTION; }
};