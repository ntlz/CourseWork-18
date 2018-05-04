#include "graph.h"
#include <stack>


std::vector<vertex_sequence> graph::dfs_stack(int max_length, vertex start, bool reverse)
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
            if (!reverse)
            {
                if (r.first() == v)
                {
                    ver.push(r.second());
                    lvl.push(l + 1);
                }
            }
            else
            {
                if (r.second() == v)
                {
                    ver.push(r.first());
                    lvl.push(l + 1);
                }
            }
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

const edge& graph::find_edge(const vertex& a, const vertex& b)
{
    return *find_if(_edges.begin(), _edges.end(), [&a, &b](const edge& e) -> bool
    {
        return e.first() == a && e.second() == b;
    });
}

void graph::copy_out_edges(vertex & a, vertex & b)
{
    edge_vector t = (*this)[a];
    for (auto n : t)
    {
        if (n.first() == a)
            this->add_edge(b, n.second());
    }
}
vertex graph::visit_seq(vertex_sequence & s)
{
    edge_vector temp(_edges.begin(), _edges.end());

    for (auto it = s.begin(); it != s.end() && it + 1 != s.begin(); it++)
    {
        (*find_if(temp.begin(), temp.end(), [&it](const edge& e) -> bool
        {
            return e.first() == *it && e.second() == *(it + 1);
        })).visit();
    }
    _edges = edge_set(temp.begin(), temp.end());
    return s[1];
}
graph::graph()
{
}


graph::~graph()
{
}

std::ostream & operator<<(std::ostream & out, const graph & g)
{
	out << "Digraph { \n";
    for (const auto& v : g._edges)
    {
        out << v.first() << "->" << v.second() << std::endl;
    }
	out << "}";
	return out;
}

std::ostream & operator<<(std::ostream & out, const edge & e)
{
    out << e.first() << ":" << e.second();
    return out;
}
