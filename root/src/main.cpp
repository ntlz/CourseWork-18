#include "events.h"
#include "vertex.h"
#include "graph.h"
#include "vertex_sequence.h"

#include <vector>
#include <set>
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

using namespace std;


typedef std::shared_ptr<event_type> event_type_ptr;

int* get_brinks(string p)
{
	int n = p.length();
	int* br = new int[n];
	br[0] = 0;
	int k = 0;
	for (int q = 1; q < n; q++)
	{
		while (k > 0 && p[k] != p[q])
			k = br[k - 1];
		if (p[k] == p[q])
			++k;
		br[q] = k;
	}
	return br;
}
int get_substrings(string t, event_sequence& subst)
{
	string p = "";
	int res = 0;
	for (int i = 0; i < subst.seq.size(); ++i)
		p += subst.seq[i]->type;
	int n = t.length();
	int m = p.length();
	int* br = get_brinks(p);
	int q = 0;
	for (int i = 0; i < n; i++)
	{
		while (q > 0 && p[q] != t[i])
			q = br[q - 1];
		if (p[q] == t[i])
			++q;
		if (q == m)
		{
			//res.push_back(i - m + 1);
			res++;
			q = br[q - 1];
		}
	}
	return res;
}

void rec(const vector<event_type_ptr> & al, vector<int>::iterator begin, vector<int>::iterator pos, vector<int>::iterator end, set<event_sequence>& es)
{
	//cout << *pos << " " << *end << endl;
	if (pos >= end)
	{
		vector<event_type_ptr> a;
		for (auto i = begin; i != end; i++)
			a.push_back(al[*i]);
		es.insert(event_sequence(move(a)));
		return;
	}
	else
	{
		for (int i = 0; i < al.size(); i++)
		{
			*pos = i;
			rec(al, begin, pos + 1, end, es);
		}
	}
}

void get_all_subseq(string& trace, int n, vector<event_sequence>& es, vector<event_type_ptr>& al)
{
	set<event_type> et;
	set<event_type_ptr> temp;
	set<event_sequence> es1;

	for (int i = 0; i < trace.size(); i++)
		et.insert(event_type(trace.substr(i, 1)));
	for (auto& r : et)
		temp.insert(event_type_ptr(new event_type(r)));

	al = vector<event_type_ptr>(temp.begin(), temp.end());

	for (int i = 1; i <= n; i++)
	{
		vector<int> temp1(i);
		rec(al, temp1.begin(), temp1.begin(), temp1.end(), es1);
	}

	//cout << "tram" << endl;
	es = vector<event_sequence>(es1.begin(), es1.end());
}

void set_count_and_prob(string& trace, vector<event_sequence>& seqs)
{
	int currentBase = 1;
	int currentSecondBase = 1;
	for (event_sequence& s : seqs)
	{
		s.count = get_substrings(trace, s);
		if (s.seq.size() == 1) //TODO: убрать костыль
			currentBase = s.count;
		if (s.seq.size() == 2)
		{
			s.probability = (double)s.count / currentBase;
			currentSecondBase = s.count;
		}
		else
		{
			if (currentSecondBase == 0)
				s.probability = 0;
			else
				s.probability = (double)s.count / currentSecondBase;
		}
		//cout << s << "->" << s.count << ", " << s.probability << endl;
	}
}

int main(int argc, char** argv)
{
	vector<event_sequence> sequence_of_events;
	vector<event_type_ptr> input_trace;

	string trace;
	fstream fin("input.txt");
	fin >> trace;
	fin.close();
	get_all_subseq(trace, 3, sequence_of_events, input_trace);
	set_count_and_prob(trace, sequence_of_events);

	graph g;
	for (event_sequence& es : sequence_of_events)
	{
		if (es.probability > 0)
		{
			for (auto it = es.seq.begin(), nit = ++es.seq.begin(); nit != es.seq.end(); it++, nit++)
			{
				g[*it].emplace(*nit);
			}
		}
	}
	ofstream fout("output.txt");
	fout << g << endl;
	fout.close();
	vector<vertex_sequence> vs;

	for (auto& el : g) //по графу, пара: вершина - массив инцидентных ей
	{
		vs = g.dfs(3, el.first); //для текущей вершины находим все пути длины 3
		for (auto& path : vs) // итерируемся по путям, проверяем их валидность
		{
			for (auto& r : path)
				cout << r;
			cout << " ";
		}
		cout << endl;
	}
	system("PAUSE");
}