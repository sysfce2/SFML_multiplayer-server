#pragma once

#include <list>
#include <unordered_map>
#include "IObserver.hpp"
#include "PacketEvent.hpp"
#include "IPacketListener.hpp"

class PacketObserver : public IObserver {
private:
	using UPacketListener = std::unique_ptr<IPacketListener>;

	std::unordered_map<PacketType, std::list<UPacketListener>> handlers;
public:
	void onEvent(const PacketEvent& event) override;

	template<typename PacketListenerT>
	void registerListener(PacketType type) {
		if(handlers.find(type) == handlers.end())
			// TODO: THIS CAUSES PROBLEMS
			handlers.insert({ type, {} });

		auto& handlersByType = handlers.at(type);
		handlersByType.push_back(std::make_unique<PacketListenerT>());
	}
public:
	void handle(PacketType type, sf::Packet sfPacket) {
		auto& handlersByType = handlers.at(type);
		for(UPacketListener& l : handlersByType)
			l->handle(sfPacket);
	}
};