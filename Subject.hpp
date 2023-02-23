#pragma once

#include <list>
#include <utility>
#include "IEvent.hpp"
#include "IObserver.hpp"

class Subject {
private:
	std::list<IObserver::Pointer> observers;
public:
	/*
		This function instantiates a new event of type EventT and
		passes the arguments from the notify function to the constructor
		of said event using variadic parameters
	*/
	template<typename EventT, typename... Args>
	void notify(Args&&... args) {
		for(IObserver::Pointer& o : observers)
			o->onEvent(EventT(std::forward<Args>(args)...));
	}

	/*
		This function attaches a new instance of EventT to the subject
	*/
	template<typename ObserverT>
	auto& attach() {
		observers.push_back(std::make_unique<ObserverT>());

		auto& lastObserver = observers.back();
		ObserverT& o = dynamic_cast<ObserverT&>(*(lastObserver.get()));

		return o;
	}


	void detachAll() { observers.clear(); }
};
