#pragma once
#include <spdlog/spdlog.h>
#include <SFML/Network/Packet.hpp>
#include "Events.hpp"
#include "EventBus.hpp"
#include "PacketType.hpp"
#include "ClientConnection.hpp"

// TODO: Make this namespace
// also check if other classes can be namespace'd
class PacketProcessor {
public:
	void process(sf::Packet& packet, ClientConnection::ID id);
};