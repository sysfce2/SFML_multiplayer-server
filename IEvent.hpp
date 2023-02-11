#pragma once

enum EventType { NONE, PACKET, CONNECTION };

/// <summary>
/// Base interface class for all events.
/// WARNING: Do not store references to member variables! They are not guaranteed to be kept alive after
/// the event has gone out of scope.
/// </summary>
class IEvent {
public:
	virtual ~IEvent() {}

	virtual EventType getType() const { return NONE; }
};