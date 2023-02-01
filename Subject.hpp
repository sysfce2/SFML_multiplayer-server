#pragma once

#include <list>
#include "IObserver.hpp"

class ISubject {
private:
	std::list<IObserver::Pointer> observers;
public:
	void postEvent();
};