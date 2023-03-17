#include "ClientConnection.hpp"
#include <spdlog/spdlog.h>

// keep track of our last ID
ClientConnection::ID ClientConnection::connectionCount = 0;

ClientConnection::ClientConnection()
	: id(connectionCount), socket() {

	++connectionCount;
}