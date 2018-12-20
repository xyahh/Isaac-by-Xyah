#pragma once
class IDType
{
	friend Cyan;

private:
	STD string  Name;
	size_t		Number;

	IDType(STD string Name, size_t Number) :
		Name(Name),
		Number(Number)
	{}

public:
	const STD string& GetName() const { return Name; }
	const size_t& GetNumber() const { return Number; }

	friend bool operator<(const IDType& A, const IDType& B);
};

inline bool operator<(const IDType& A, const IDType& B)
{
	if (!A.Name.empty() || !B.Name.empty())
		return A.Name < B.Name;
	else
		return A.Number < B.Number;
}