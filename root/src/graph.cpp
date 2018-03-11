#include "graph.h"
#include <stack>


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
            //cout << node.ver << " -> " << r << " : " << node.level << endl;
                ver.push(r.second);
            lvl.push(l + 1);
        }
    }

    return result;
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
	out << "Oriented graph { \n";
    for (const auto& v : g._edges)
    {
        out << v.first << "->" << v.second << std::endl;
    }
	out << "}";
	return out;
}
