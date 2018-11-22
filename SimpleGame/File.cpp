#include "stdafx.h"
#include "File.h"
#include <fstream>

STD vector<STD vector<STD string>> ReadCSV(STD string filepath, char delimiter, bool ignore_first_row)
{
	STD ifstream file;
	file.open(filepath);
	STD vector<STD vector<STD string>> record_file;
	STD string line;

	if (ignore_first_row)
		STD getline(file, line);

	while (STD getline(file, line))
	{
		STD vector<STD string> record;
		STD istringstream row(line);
		STD string data;
		while (STD getline(row, data, delimiter))
			record.emplace_back(STD move(data));
		record_file.emplace_back(STD move(record));
	}
	file.close();
	return record_file;
}