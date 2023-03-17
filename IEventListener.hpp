#pragma once

#include <memory>
#include "Events.hpp"

class IEventListener {
public:
	using Pointer = std::unique_ptr<IEventListener>;
public:
	virtual void handle(C2SPingPacket e);
	virtual void handle(C2SConnection e);
	virtual void handle(C2SDisconnection e);
	virtual void handle(C2SPacketPreprocess e);
	virtual void handle(S2CPacketPreprocess e);
};