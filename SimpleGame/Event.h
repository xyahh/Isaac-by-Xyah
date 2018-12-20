#pragma once
class Event
{
public:
	Event() {}

	Event(STD function<void()> Action) :
		 Action(Action) {}
	~Event() {}

	void Set(const STD function<void()>& Fx)
	{
		Action = Fx;
	}

	void Execute()
	{
		Action();	
	}

private:
	STD function<void()> Action;
};

