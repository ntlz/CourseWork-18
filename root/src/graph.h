#include <utility>
#include <vector>
#include <set>
#include <functional>
#include <algorithm>
#include <map>
#include <stack>

#include "vertex.h"
#include "vertex_sequence.h"

#pragma once

std::ostream& operator<<(std::ostream& out, const std::pair<vertex, vertex>& t);

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
			if (v == t.first || v == t.second)
				res.insert(t);
        return move(res);
	}
    edge_set operator[](event_type_ptr v) const
    {
        edge_set res;
        for (const auto& t : _edges)
            if (v == t.first.type || v == t.second.type)
                res.insert(t);
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

	/* 
	requires a callback handler for newly found path;
	handler should return a boolean value, indicating whether the latest path
	has been resolved, thus modifying the graph;
	if true: dsf will return boolean value, signifying the need to reiterate it;
	*/
	vertex_sequence dfs_stack_with_a_twist(const int max_length, const vertex start, const std::function<bool(vertex_sequence&)>);

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

