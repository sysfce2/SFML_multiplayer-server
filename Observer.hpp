#pragma once

#include <memory>


class IObserver {
public:
	typedef std::unique_ptr<Observer> Pointer;
};