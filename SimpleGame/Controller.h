#pragma once

class Controller
{
	friend Cyan;

public:

	void MapControl(int Value, const STD string& CommandName);

private:

	void HandleControls(const IDType& ObjectIndex,
		const STD vector<int>& Pressed,
		const STD vector<int>& Released
	);

private:
	STD multimap<int, STD string>	m_Controls;
};


