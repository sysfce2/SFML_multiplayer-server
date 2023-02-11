#pragma once

#include <memory>
#include "PacketEvent.hpp"
#include "ConnectionEvent.hpp"

class IObserver {
public:
	typedef std::unique_ptr<IObserver> Pointer;
public:
	virtual ~IObserver() {}

	virtual void onEvent(const PacketEvent& event)		{};
	virtual void onEvent(const ConnectionEvent& event)	{};
};