#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <stack>

#include "vertex.h"
#include "vertex_sequence.h"

#pragma once

class graph
{
public:
	typedef vertex::vertex_ptr vertex_ptr;
	typedef std::set<vertex> vertex_set;
    typedef std::pair<vertex, vertex> edge;
    typedef std::set<edge> edge_set;
    

	edge_set operator[](vertex v) const
	{
        edge_set res;
        for (const auto& t : _edges)
        {
            if (v == t.first || v == t.second)
                res.insert(t);
        }
        return move(res);
	}
    edge_set operator[](event_type_ptr v) const
    {
        edge_set res;
        for (const auto& t : _edges)
        {
            if (v == t.first.type || v == t.second.type)
                res.insert(t);
        }
        return move(res);
    }

    void add_edge(vertex a, vertex b)
    {
        _edges.insert(std::make_pair(a, b));
    }
protected:	
	//std::map<vertex, vertex_set> incid_table;
    edge_set _edges;

public:

    std::vector<vertex_sequence> dfs_stack(int max_length, vertex start);
    void insert_edge(edge e);
    void remove_edge(edge e);

	graph();
	~graph();
	typename edge_set::iterator begin()
	{
		return _edges.begin();
	}
	typename edge_set::iterator end()
	{
		return _edges.end();
	}
	typename edge_set::const_iterator begin() const
	{
		return _edges.begin();
	}
	typename edge_set::const_iterator end() const
	{
		return _edges.end();
	}
	friend std::ostream& operator<<(std::ostream& out, const graph& g);
};


