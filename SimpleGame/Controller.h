#pragma once

class Controller
{
	friend Cyan;

public:

	void MapControl(int Value, size_t CommandIndex);

private:
	void HandleControls(size_t ObjectIndex,
		const STD vector<int>& Pressed,
		const STD vector<int>& Released
	);

private:
	STD multimap<int, size_t>	m_Controls;
};


