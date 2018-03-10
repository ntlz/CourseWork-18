#include "vertex_sequence.h"


vertex_sequence & vertex_sequence::set_predicate(std::function<bool(const vertex_vector&)> pred)
{
	this->_pred = pred;
	return *this;
}

bool vertex_sequence::_try_split(int)
{
	return false;
}

bool vertex_sequence::is_valid(std::vector<event_sequence> es)
{
	return true;
}

vertex vertex_sequence::resolve()
{
	for (int i = 1; i < this->_group.size() - 1; i++)
		if (this->_try_split(i))
			return this->_group[i];
}

vertex_sequence::vertex_sequence()
{
}

vertex_sequence::~vertex_sequence()
{
}
