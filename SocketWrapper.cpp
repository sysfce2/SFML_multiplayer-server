#include "SocketWrapper.hpp"
#include <spdlog/spdlog.h>

// keep track of our last ID
SocketWrapper::ID SocketWrapper::socketCount = 0;

SocketWrapper::SocketWrapper()
	: id(socketCount), socket() {

	++socketCount;
}