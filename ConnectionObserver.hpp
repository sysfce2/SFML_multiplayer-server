#pragma once

#include "IEvent.hpp"
#include "IObserver.hpp"

class ConnectionObserver : public IObserver {
	void onEvent(const ConnectionEvent& event) override;
};