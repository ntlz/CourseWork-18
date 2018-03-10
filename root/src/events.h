#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>

#pragma once

class event_type
{
public:
	std::string type;

	event_type()
	{
	}

	event_type(std::string str)
	{
		this->type = str;
	}

	bool operator==(const event_type& t) const
	{
		return this->type == t.type;
	}
	bool operator!=(const event_type& t) const 
	{
		return !(*this == t);
	}
	bool operator<(const event_type& t) const
	{
		return this->type < t.type;
	}
};

typedef std::shared_ptr<event_type> event_type_ptr;
class event_sequence
{
public:
	std::vector<event_type_ptr> seq;
	double probability;
	int count;

	event_sequence() : count(0), probability(0)
	{
	}
	event_sequence(std::vector<event_type_ptr> v) : event_sequence()
	{
		this->seq = v;
	}

	bool operator<(const event_sequence& t) const
	{
		return std::lexicographical_compare(this->seq.begin(), this->seq.end(), t.seq.begin(), t.seq.end()); 
	}
	bool operator==(const event_sequence& t) const
	{
		return std::lexicographical_compare(this->seq.begin(), this->seq.end(), t.seq.begin(), t.seq.end());
	}
};

std::ostream& operator<<(std::ostream& out, const event_type& t);
std::ostream& operator<<(std::ostream& out, const event_sequence& t);