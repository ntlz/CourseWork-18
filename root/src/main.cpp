#include "markov_algorithm.h"

#include <fstream>
#include <iostream>

using namespace std;
#define DEBUG

int main(int argc, char** argv)
{
    
	vector<event_sequence> sequence_of_events;
	vector<event_type_ptr> input_trace;

	string trace;
	fstream fin("input.txt");
	fin >> trace;
	fin.close();

    markov_algorithm markov(trace, 0, 0, 3, true);
    markov.build_graph();

    //markov.do_magic(); // this is so last week..

	markov.do_magic_alternatively(); 


    ofstream fout("output.txt");
    fout << markov.event_graph;
    fout.close();
#ifdef DEBUG    
    for (auto& r : markov.all_event_seq)
    {
        cout << r << "->" << r.count << ", " << r.probability << endl;
    }
#endif // DEBUG

	system("PAUSE");
}