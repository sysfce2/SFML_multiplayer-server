#include "SocketStack.hpp"
#include <spdlog/spdlog.h>

SocketStack::SocketStack(const size_t maxSockets) : maxSockets(maxSockets) {
	wrappers.reserve(maxSockets);
};

void SocketStack::add(SocketWrapper::Pointer wrapper) {
	spdlog::debug("Socket with ID: {} has been added to the socket stack.", wrapper->getId());
	wrappers.push_back(std::move(wrapper));
}

void SocketStack::remove(SocketWrapper::ID id) {
	spdlog::debug("Socket with ID: {} has been removed from the socket stack.", id);
	wrappers.erase(std::remove_if(wrappers.begin(), wrappers.end(), [id](auto& s) { return id == s->getId(); }), wrappers.end());
}

// TODO: maybe use std::optional<SocketWrapper&> as return type
SocketWrapper& SocketStack::getWrapper(SocketWrapper::ID id) {
	for(auto& wrapper : wrappers)
		if(wrapper->getId() == id)
			return *wrapper;

	spdlog::error("Socket with ID: {} was not found", id);
}