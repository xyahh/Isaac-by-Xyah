#pragma once
class EventDispatcher
{
public:

	template<typename Func>
	void AddEvent(u_int Event, Func&& Fx)
	{
		m_Events.emplace(Event, Fx);
	}
	void Dispatch(u_int EventType)
	{
		auto Event = m_Events.find(EventType);
		if (Event != m_Events.end())
			Event->second();
	}

private:
	STD map<u_int, STD function<void()>> m_Events;
};

