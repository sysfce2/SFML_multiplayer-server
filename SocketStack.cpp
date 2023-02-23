#include "SocketStack.hpp"
#include <spdlog/spdlog.h>
#include "ConnectionListener.hpp"

SocketStack::SocketStack(const size_t maxSockets) : maxSockets(maxSockets) {
	// reserve socket space
	sockets.reserve(maxSockets);
};

void SocketStack::add(SocketWrapper&& socket) {
	spdlog::debug("Socket with ID: {} has been added to the socket stack.", socket.getIdx());
	sockets.push_back(std::move(socket));
}

void SocketStack::remove(SocketWrapper::ID id) {
	spdlog::debug("Socket with ID: {} has been removed from the socket stack.", id);
	sockets.erase(std::remove_if(sockets.begin(), sockets.end(), [id](SocketWrapper& s) { return id == s.getIdx(); }), sockets.end());
}

// TODO: maybe use std::optional<SocketWrapper&> as return type
SocketWrapper& SocketStack::getSocketWrapper(SocketWrapper::ID id) {
	for(auto& socket : sockets)
		if(socket.getIdx() == id)
			return socket;

	spdlog::error("Socket with ID: {} was not found", id);
}