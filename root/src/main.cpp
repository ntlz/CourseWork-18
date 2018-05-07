#include "split_merge.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;
//#define DEBUG

vector<string> parse_csv(ifstream& in)
{
	std::vector<std::string> result;
	std::string line;

	std::stringstream lineStream(line);
	std::string cell;

	while (std::getline(lineStream, cell, ','))
	{
		result.push_back(cell);
	}
	// This checks for a trailing comma with no data after it.
	if (!lineStream && cell.empty())
	{
		// If there was a trailing comma then add an empty element.
		result.push_back("");
	}
	return result;
}

int main(int argc, char** argv)
{
	int o;
	string file;
	string trace;
	vector<string> log;
	/*if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << " order path_to_file" << endl;
		return 1;
	}
	else
	{
		o = atoi(argv[1]);
		file = argv[2];
		ifstream fin(file);
		if (!fin)
		{
			cerr << "File doesn't exist!" << endl;
			return 1;
		}
		while (fin >> trace)
			log.push_back(trace);
		fin.close();

		split_merge sm(log, o);
		sm.build();
	}*/
	o = 3;
	file = "input3.txt";
	//file = "input.csv";
	ifstream fin(file);
	//log = parse_csv(fin);
	while (fin >> trace)
		log.push_back(trace);
	fin.close();

	split_merge sm(log, o);
	sm.build();
#ifdef DEBUG    
    for (auto& r : markov.all_event_seq)
    {
        cout << r << "->" << r.count << ", " << r.probability << endl;
    }
#endif // DEBUG

	system("PAUSE");
}