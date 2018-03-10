#include <stack>
#include <functional>

#include "vertex.h"


#pragma once
class vertex_sequence : event_sequence
{
public:
	typedef std::vector<vertex> vertex_vector;
	void push_back(vertex v)
	{
		_group.push_back(v);
	}
	typename std::vector<vertex>::iterator begin()
	{
		return _group.begin();
	}
	typename std::vector<vertex>::iterator end()
	{
		return _group.end();
	}
protected:

	vertex_vector _group;

	bool _try_split(int);

	std::function<bool(const vertex_vector&)> _pred = [](const vertex_vector&) -> bool { return false; };

public:
	vertex_sequence & set_predicate(std::function<bool(const vertex_vector&)>);

	bool is_valid(std::vector<event_sequence> es);

	vertex resolve();

	vertex_sequence(); 
	//vertex_sequence(typename vertex_vector::reverse_iterator begin, typename vertex_vector::reverse_iterator end) : _group(begin, end) { }
	~vertex_sequence();
};

