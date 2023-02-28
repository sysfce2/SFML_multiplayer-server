#pragma once

#include "IObserver.hpp"
#include "ConnectionEvent.hpp"

class ConnectionObserver : public IObserver {
	void onEvent(const ConnectionEvent& event) override;
};