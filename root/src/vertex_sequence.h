#include <stack>
#include <functional>
#include <algorithm>

#include "vertex.h"


#pragma once
class vertex_sequence
{
public:
	typedef std::vector<vertex> vertex_vector;
	typedef std::set<std::pair<vertex, vertex>> edge_set;
	vertex_sequence() { }
	/*Methods*/
	void push_back(vertex v)
	{
		_group.push_back(v);
	}
	void pop_back()
	{
		_group.pop_back();
	}
	int get_size()
	{
		return _group.size();
	}

	/*Operators and iterators*/
	vertex operator[] (int i)
	{
		return _group[i];
	}
	typename std::vector<vertex>::iterator begin()
	{
		return _group.begin();
	}
	typename std::vector<vertex>::iterator end()
	{
		return _group.end();
	}
	typename std::vector<vertex>::const_iterator begin() const
	{
		return _group.begin();
	}
	typename std::vector<vertex>::const_iterator end() const
	{
		return _group.end();
	}

protected:
	vertex_vector _group;

public:
	friend bool  operator< (const vertex_sequence& a, const event_sequence& b);
	friend bool  operator< (const event_sequence& a, const vertex_sequence& b);
	friend std::ostream& operator<<(std::ostream& out, const vertex_sequence& t);
	friend bool operator==(const event_sequence& a, const vertex_sequence& b);
	friend bool operator==(const vertex_sequence& a, const event_sequence& b);
};
