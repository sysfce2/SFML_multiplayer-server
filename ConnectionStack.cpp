#include "ConnectionStack.hpp"
#include <spdlog/spdlog.h>

ConnectionStack::ConnectionStack(const size_t maxConnections) : maxConnections(maxConnections) {
	connections.reserve(maxConnections);
}

void ConnectionStack::add(ClientConnection::Pointer connection) {
	spdlog::debug("Socket with ID: {} has been added to the socket stack.", connection->getId());
	connections.push_back(std::move(connection));
}

void ConnectionStack::remove(ClientConnection::ID id) {
	spdlog::debug("Socket with ID: {} has been removed from the socket stack.", id);
	connections.erase(std::remove_if(connections.begin(), connections.end(), [id](auto& s) { return id == s->getId(); }), connections.end());
}

// TODO: maybe use std::optional<SocketWrapper&> as return type
ClientConnection& ConnectionStack::getConnection(ClientConnection::ID id) {
	for(auto& connection : connections)
		if(connection->getId() == id)
			return *connection;

	spdlog::error("Socket with ID: {} was not found", id);
}