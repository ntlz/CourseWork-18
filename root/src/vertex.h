#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>

#include <cmath>
#include "events.h"

#pragma once



class vertex
{
public:
	typedef std::shared_ptr<vertex> vertex_ptr;
	typedef std::shared_ptr<event_type> event_type_ptr;

protected:
	
public:
    bool is_old = true;
	size_t id;
	event_type_ptr type;
	std::shared_ptr<bool> is_accepting = std::make_shared<bool>(false);
	vertex() : type(new event_type("")) { id = -1; };
	vertex(event_type_ptr et) { type = et; id = 0; }
    vertex(vertex& v, int id) { type = v.type, this->id = id + std::abs(rand()) % 200;  is_old = false; } // используется только для новых вершин!
	~vertex();
	void set_accepting() { *is_accepting = true; }
	bool operator==(const vertex& v) const
	{
		return (this->id == v.id && this->type == v.type);
	}
	bool operator!=(const vertex& v) const 
	{
		return !(*this == v);
	}
	bool operator<(const vertex& v) const
	{
		return std::pair<event_type_ptr, size_t>(this->type,  this->id) < std::pair<event_type_ptr, size_t>(v.type, v.id);
	}
};
bool operator==(const vertex& v, const event_type_ptr& a);
bool operator< (const vertex& v, const event_type_ptr& a);
bool operator< (const event_type_ptr& a, const vertex& v);
std::ostream& operator<<(std::ostream& out, const vertex& t);

