#pragma once
#include <list>
#include <utility>
#include "Events.hpp"
#include "IEventListener.hpp"

class EventBus {
private:
	std::list<IEventListener::Pointer> listeners;
public:
	template<typename Event, typename... Args>
	void emit(Args&&... args) {
		for(auto& listener : listeners)
			listener->handle(Event(std::forward<Args>(args)...));
	};

	template<typename ListenerT>
	void registerListener() {
		auto listener = std::make_unique<ListenerT>();
		listeners.push_back(std::move(listener));
	}
};