#pragma once
#include <vector>
#include <memory>
#include "SocketWrapper.hpp"

class SocketStack {
private:
	const size_t maxSockets;
	std::vector<SocketWrapper::Pointer> wrappers;
public:
	SocketStack(const size_t maxSockets);

	size_t size() { return wrappers.size(); }
	size_t maxSize() { return maxSockets; }

	void remove(SocketWrapper::ID id);
	void add(SocketWrapper::Pointer wrapper);

	auto& getWrappers() { return wrappers; }
	SocketWrapper& getWrapper(SocketWrapper::ID id);
};