#pragma once

#include <memory>
#include <SFML/Network.hpp>

namespace Socket {
	typedef int Idx;
	typedef std::unique_ptr<sf::TcpSocket> Pointer;
}