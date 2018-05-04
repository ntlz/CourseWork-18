#include "markov_algorithm.h"
#include "split_merge.h"

#include <fstream>
#include <iostream>

using namespace std;
//#define DEBUG

int main(int argc, char** argv)
{
    
	vector<event_sequence> sequence_of_events;
	vector<event_type_ptr> input_trace;

	string trace;
    vector<string> log;
	fstream fin("input2.txt");
    while (fin >> trace)
        log.push_back(trace);
	fin.close();

    split_merge sm(log, 3);
    sm.build();

//     markov_algorithm markov(trace, 0, 0, 4, true);
//     markov.build_graph();
//     markov.do_magic();
//     ofstream fout("output.txt");
//     fout << markov.event_graph;
//     fout.close();


#ifdef DEBUG    
    for (auto& r : markov.all_event_seq)
    {
        cout << r << "->" << r.count << ", " << r.probability << endl;
    }
#endif // DEBUG

	system("PAUSE");
}