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

	void push_back(vertex v)
	{
		_group.push_back(v);
	}
    int get_size()
    {
        return _group.size();
    }
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
	//bool _try_split(vertex vs, int pos);
    //vertex iterate_modifications();
	std::function<bool(const vertex_vector&)> _pred = [](const vertex_vector&) -> bool { return false; };

	vertex_sequence & set_predicate(std::function<bool(const vertex_vector&)>);

	bool is_valid(std::vector<event_sequence>& es, int begin, int end, double probability_thrhld, double count_thrhld);


	vertex_sequence(); 
	//vertex_sequence(typename vertex_vector::reverse_iterator begin, typename vertex_vector::reverse_iterator end) : _group(begin, end) { }
	~vertex_sequence();
};
bool  operator< (const vertex_sequence& a, const event_sequence& b);
bool  operator< ( const event_sequence& a, const vertex_sequence& b);
std::ostream& operator<<(std::ostream& out, const vertex_sequence& t);

