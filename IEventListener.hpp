#pragma once

#include <memory>
#include "Events.hpp"

class IEventListener {
public:
	using Pointer = std::unique_ptr<IEventListener>;
public:
	virtual void handle(EPacketSent e);
	virtual void handle(EConnection e);
	virtual void handle(EDisconnection e);
	virtual void handle(EPacketReceived e);
};