#pragma once
#include <unordered_map>
#include "NetworkServer.hpp"
#include "IEventListener.hpp"

class PingListener : public IEventListener {
private:
	using DeltaTime = sf::Int32;
	const float MAX_NO_REPLY_TIME_MS = 5000;
private:
	NetworkServer& server;
	std::unordered_map<ClientConnection::ID, DeltaTime> pings;
public:
	PingListener(NetworkServer& server) : server(server) {};
public:
	void handle(S2STick e) override;
	void handle(C2SPingPacket e) override;
	void handle(C2SConnection e) override;
	void handle(C2SDisconnection e) override;
};