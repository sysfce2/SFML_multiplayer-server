#pragma once
#include <unordered_map>
#include "IEventListener.hpp"

class PingListener : public IEventListener {
private:
	using TimeInterval = sf::Int32;

	std::unordered_map<ClientConnection::ID, TimeInterval> pings;
public:
	void handle(S2STick e) override;
	void handle(C2SPingPacket e) override;
	void handle(C2SConnection e) override;
	void handle(C2SDisconnection e) override;
};