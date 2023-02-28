#pragma once

#include <memory>

class IPacketListener {
public:
	using Pointer = std::unique_ptr<IPacketListener>;
public:
	virtual ~IPacketListener() {}

	// we don't take a reference to the sf::Packet otherwise once
	// we've passed it to a function for the first time
	// its member variables will have been consumed!
	virtual void handle(sf::Packet packet) = 0;
};