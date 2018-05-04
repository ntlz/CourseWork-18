#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <stack>

#include "vertex.h"
#include "vertex_sequence.h"

#pragma once

class edge;

std::ostream& operator<<(std::ostream& out, const edge& e);

class edge
{
public:
    edge() { }
    edge(vertex& a, vertex& b) 
    {
        _trans = std::pair<vertex, vertex>(a, b);
    }
	edge(vertex& a, vertex& b, short c)
	{
		_trans = std::pair<vertex, vertex>(a, b);
		num_visits = c;
	}
    vertex first()
    {
        return _trans.first;
    }
    vertex first() const
    {
        return _trans.first;
    }
    vertex second()
    {
        return _trans.second;
    }
    vertex second() const
    {
        return _trans.second;
    }
    bool operator<(const edge& e) const
    {
        return this->_trans < e._trans;
        //std::cout << *this << " < " << e << " == " << (_trans.first < e._trans.first && _trans.second < e._trans.second) << std::endl;
        //return  _trans.first < e._trans.first && _trans.second < e._trans.second;
    }
    bool operator==(const edge& e) const
    {
        return _trans.first == e._trans.first && _trans.second == e._trans.second;
    }
    void visit() { this->_is_visited = true; }
    std::pair<vertex, vertex> _trans;
    bool _is_visited = false;
    short num_visits = 0;
};

class graph
{
public:
	typedef vertex::vertex_ptr vertex_ptr;
	typedef std::set<vertex> vertex_set;
    typedef std::set<edge> edge_set;
    typedef std::vector<edge> edge_vector;

	edge_vector operator[](vertex v) const
	{
        edge_set res;
        for (const auto& t : _edges)
        {
            if (v == t.first() || v == t.second())
                res.insert(t);
        }
        return edge_vector(res.begin(), res.end());
	}
    edge_vector operator[](event_type_ptr v) const
    {
        edge_set res;
        for (const auto& t : _edges)
        {
            if (v == t.first().type || v == t.second().type)
                res.insert(t);
        }
        return edge_vector(res.begin(), res.end());
    }

    void add_edge(vertex a, vertex b)
    {
        edge k(a, b);
        _edges.insert(k);
    }
protected:	
	//std::map<vertex, vertex_set> incid_table;
    edge_set _edges;

public:

    std::vector<vertex_sequence> dfs_stack(int max_length, vertex start, bool reverse = false);
    void insert_edge(edge e);
    void remove_edge(edge e);
    void copy_out_edges(vertex& a, vertex& b);
    vertex visit_seq(vertex_sequence& s);
    const edge& find_edge(const vertex & a, const vertex & b);
    //edge& find_edge(const vertex & a, const vertex & b);
    vertex get_init_state() { return vertex(); }

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
    std::set<vertex> get_vertices(event_type_ptr etp)
    {
        std::set<vertex> k;
        for (const edge& e : _edges)
        {
            if (e.first().type == etp)
                k.insert(e.first());
            if (e.second().type == etp)
                k.insert(e.second());
        }
        return k;
    }
	friend std::ostream& operator<<(std::ostream& out, const graph& g);
};

