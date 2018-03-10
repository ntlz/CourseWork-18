#include <map>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
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

	vertex_set& operator[](vertex v)
	{
		return _graph[v];
	}
	const vertex_set& operator[](const vertex v) const
	{
		return _graph.at(v);
	}
protected:	
	std::map<vertex, vertex_set> _graph;

public:

	void modify();
	void _visit(int max_length, std::stack<vertex>& path, std::vector<vertex_sequence>& result);
	std::vector<vertex_sequence> dfs(int max_length, vertex start);

	graph();
	~graph();
	typename std::map<vertex, vertex_set>::iterator begin()
	{
		return _graph.begin();
	}
	typename std::map<vertex, vertex_set>::iterator end()
	{
		return _graph.end();
	}
	typename std::map<vertex, vertex_set>::const_iterator begin() const
	{
		return _graph.begin();
	}
	typename std::map<vertex, vertex_set>::const_iterator end() const
	{
		return _graph.end();
	}
	friend std::ostream& operator<<(std::ostream& out, const graph& g);
};


