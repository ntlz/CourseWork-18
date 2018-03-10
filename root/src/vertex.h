#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>

#include "events.h"

#pragma once


class vertex
{
public:
	typedef std::shared_ptr<vertex> vertex_ptr;
	typedef std::shared_ptr<event_type> event_type_ptr;

protected:
	
public:
	size_t id;
	event_type_ptr type;
	vertex() { };
	vertex(event_type_ptr et) { type = et; id = std::hash<std::string>()(type->type); }
	~vertex();

	bool operator==(const vertex& v) const
	{
		return (this->id == v.id);
	}
	bool operator!=(const vertex& v) const 
	{
		return !(*this == v);
	}
	bool operator<(const vertex& v) const
	{
		return this->id < v.id;
	}
};

std::ostream& operator<<(std::ostream& out, const vertex& t);

