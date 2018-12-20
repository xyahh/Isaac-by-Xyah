#pragma once
class Event
{
public:
	Event(STD function<void()> Action) :
		 Action(Action) {}
	~Event() {}

	void Execute()
	{
		Action();	
	}

private:
	STD function<void()> Action;
};

