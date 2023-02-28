#pragma once

#include <unordered_map>
#include "IObserver.hpp"
#include "PacketType.hpp"
#include "PacketEvent.hpp"
#include "IPacketListener.hpp"

class PacketObserver : public IObserver {
private:
	std::unordered_map<PacketType, std::list<IPacketListener::Pointer>> listeners;
public:
	PacketObserver();

	void onEvent(const PacketEvent& event) override;
private:
	template<typename ListenerT>
	void registerListener(PacketType type) {
		auto& listenersByType = listeners[type];

		listenersByType.push_back(std::make_unique<ListenerT>());
	}
};