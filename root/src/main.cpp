#include "split_merge.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;
//#define DEBUG

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
	file = "input2.txt";
	ifstream fin(file);
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