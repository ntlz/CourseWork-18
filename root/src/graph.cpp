#include "graph.h"

void graph::_visit(int max_length, std::stack<vertex>& path, std::vector<vertex_sequence>& result)
{
	if (path.size() >= max_length)
	{
		result.emplace_back();
		for (auto& r : path._Get_container())
			result.back().push_back(r);
		return;
	}

	for (auto& r : (*this)[path.top()])
	{
		path.push(r);

		_visit(max_length, path, result);

		path.pop();
	}
}

std::vector<vertex_sequence> graph::dfs(int max_length, vertex start)
{
	std::stack<vertex> path;
	std::vector<vertex_sequence> result;

	path.push(start);
	_visit(max_length, path, result);

	return move(result);
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
	for (const auto& v : g._graph)
	{
		for (const auto& sv : v.second)
		{
			out << v.first << "->" << sv << std::endl;
		}
	}
	out << "}";
	return out;
}
