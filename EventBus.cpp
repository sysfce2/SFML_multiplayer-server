#include "EventBus.hpp"

void EventBus::registerListener(IEventListener::Pointer listener) {
	listeners.push_back(std::move(listener));
}