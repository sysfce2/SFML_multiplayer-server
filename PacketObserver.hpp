#pragma once

#include "IObserver.hpp"
#include "PacketEvent.hpp"

class PacketObserver : public IObserver {
	void onEvent(const PacketEvent& event) override;
};