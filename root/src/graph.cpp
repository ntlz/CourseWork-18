#include "graph.h"
#include <stack>
#include <queue>
#include <iostream>

using std::cout;
using std::endl;

std::ostream& operator<<(std::ostream& out, const std::pair<vertex, vertex>& t)
{
	out << t.first << " -> " << t.second;
	return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::stack<T>& temp)
{
	std::stack<T> s(temp), t;
	for (; !s.empty(); s.pop())
		t.push(s.top());
	for (; !t.empty(); t.pop())
		out << t.top() << " ";
	return out;
}

std::vector<vertex_sequence> graph::dfs_stack(int max_length, vertex start)
{
    std::stack<int> lvl;
    std::stack<vertex> ver;
    std::stack<vertex> path;
    std::vector<vertex_sequence> result;

    ver.push(start);
    lvl.push(1);
    for (; !ver.empty(); )
    {
        vertex v = ver.top();
        ver.pop();
        int l = lvl.top();
        lvl.pop();

        for (; path.size() >= l; path.pop());

        path.push(v);

        if (l == max_length)
        {
            result.emplace_back();
            for (auto& r : path._Get_container())
                result.back().push_back(r);
            continue;
        }


        for (auto& r : (*this)[v])
        {
            if (r.first == v)
            {
                ver.push(r.second);
                lvl.push(l + 1);
            }
        }
    }
    return result;
}

vertex_sequence graph::dfs_stack_with_a_twist(const int max_length, const vertex start, const std::function<bool(vertex_sequence&)> callback)
{
	cout << endl << "searching in THIS: " << *this << endl << endl;

	std::stack<int> lvl;
	std::stack<vertex> ver;
	std::stack<vertex> path;

	ver.push(start);
	lvl.push(1);
	for (; !ver.empty(); )
	{
		vertex v = ver.top();
		ver.pop();
		int l = lvl.top();
		lvl.pop();

		//cout << "### came to " << v << " via " << edg << endl;

		for (; path.size() >= l; path.pop());

		path.push(v);

		if (l == max_length)
		{
			vertex_sequence temp;

			for (auto& r : path._Get_container())
				temp.push_back(r);

			if (callback(temp))
			{
				cout << "restarting dfs" << endl;

				return temp;
			}
			continue;
		}

		//cout << "### " << "for vertex " << v << endl;
		for (auto& r : (*this)[v])
		{
			if (r.first == v)
			{
				//cout << "### " << "pushing " << r << endl;
				ver.push(r.second);
				lvl.push(l + 1);
			}
		}
	}
	return vertex_sequence();
}

void graph::insert_edge(edge e)
{
    _edges.insert(e);
}

void graph::remove_edge(edge e)
{
    _edges.erase(e);
}

graph::graph()
{
}


graph::~graph()
{
}

std::ostream & operator<<(std::ostream & out, const graph & g)
{
	out << "digraph { \n";
    for (const auto& v : g._edges)
    {
        out << v.first << "->" << v.second << std::endl;
    }
	out << "}";
	return out;
}
