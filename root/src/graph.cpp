#include "graph.h"
#include <stack>


std::vector<vertex_sequence> graph::dfs_stack(int max_length, vertex start, bool reverse)
{
    std::stack<int> lvl;
    std::stack<vertex> ver;
    std::stack<vertex> path;
    std::vector<vertex_sequence> result;
	vertex init = get_init_state();
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
			if (r.first() == r.second())
				continue;
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
bool graph::check_exists(vertex& a)
{
	return find_if(_edges.begin(), _edges.end(), [a](const edge b) -> bool {
		return b.first() == a || b.second() == a;
	}) != _edges.end();
}
void graph::insert_edge(edge e)
{
    _edges.insert(e);
}

void graph::remove_edge(edge e)
{
    //if (!e.get_is_visited())
		_edges.erase(e);
}
std::set<vertex> graph::get_vertices(event_type_ptr etp)
{
	std::set<vertex> k;
	for (const edge& e : _edges)
	{
		if (e.first().get_type() == etp)
			k.insert(e.first());
		if (e.second().get_type() == etp)
			k.insert(e.second());
	}
	return k;
}

const edge graph::find_edge(const vertex& a, const vertex& b)
{
    auto w = find_if(_edges.begin(), _edges.end(), [&a, &b](const edge& e) -> bool
    {
        return e.first() == a && e.second() == b;
    });
	if (w == _edges.end())
	{
		vertex p = a;
		edge t(p, vertex(p, p.get_id()));
		t.visit();
		insert_edge(t);
		return t;
	}
	else
		return *w;
}
std::tuple<edge, bool> graph::find_edge_ptr(const vertex& a, const event_type_ptr& b)
{
	std::set<vertex> p = get_vertices(b);
	for (auto v : p)
	{
		auto w = find_if(_edges.begin(), _edges.end(), [&a, &v](const edge& e) -> bool
		{
			return e.first() == a && e.second() == v;
		});
		if (w != _edges.end())
			return std::make_pair(*w, true);			
	}
	return std::make_pair(*_edges.begin(), false);
}

void graph::copy_out_edges(vertex & a, vertex & b)
{
    edge_vector q = (*this)[a];
	size_t co = 0;
    for (auto n : q)
    {
		if (n.first() == a && n.get_is_visited())
		{
			edge t(b, n.second());
			t.visit();
			insert_edge(t);
		}
		if (n.second() == a)
			++co;
    }
	if (co == 1)
	{
		for (auto n : q)
			if (n.first() == a)
				remove_edge(n);
	}
}
vertex graph::visit_seq(vertex_sequence & s)
{
    edge_vector temp(_edges.begin(), _edges.end());

    for (auto it = s.begin(); it != s.end() && it + 1 != s.end(); it++)
    {
		edge r(*it, *(it + 1));
		auto w = find(temp.begin(), temp.end(), r);
        /*auto w = find_if(temp.begin(), temp.end(), [&it](const edge& e) -> bool
        {
			std::cout << *it << std::endl;
            return e.first() == *it && e.second() == *(it + 1);
        });*/
		if (w != temp.end())
			w->visit();
		else
			std::cout << "Я дурак";
    }
    _edges = edge_set(temp.begin(), temp.end());
    return s[1];
}

std::ostream & operator<<(std::ostream & out, const graph & g)
{
	out << "Digraph {\n";
	out << "node [shape = doublecircle];";
	for (const auto& v : g._edges)
	{
		if (*(v.first().is_acc()))
			out << v.first() << " ";
		if (*(v.second().is_acc()))
			out << v.second() << " ";
	}
	out << ";\nnode [shape = circle];\n";
    for (const auto& v : g._edges)
    {
        out << v.first() << "->" << v.second() << "[label = \"" << *(v.second().get_type()) << "\"]" << std::endl;
    }
	out << "}";
	return out;
}

std::ostream & operator<<(std::ostream & out, const edge & e)
{
    out << e.first() << ":" << e.second();
    return out;
}
