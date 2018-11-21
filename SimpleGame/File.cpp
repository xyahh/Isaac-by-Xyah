#include "stdafx.h"
#include "File.h"
#include <fstream>
#include <sstream>

STD vector<STD vector<STD string>> ReadCSV(STD string filepath)
{
	STD ifstream file;
	file.open(filepath);
	STD vector<STD vector<STD string>> record_file;
	STD string line;

	while (STD getline(file, line))
	{
		STD vector<STD string> record;
		STD istringstream field(line);
		STD string data;
		while (STD getline(field, data, ','))
			record.emplace_back(data);
		record_file.emplace_back(STD move(record));
	}
	file.close();
	return record_file;
}