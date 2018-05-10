#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>
#include <iostream>
#include <cmath>
#include "events.h"

#pragma once



class vertex
{
public:
	typedef std::shared_ptr<vertex> vertex_ptr;
	typedef std::shared_ptr<event_type> event_type_ptr;
	typedef std::shared_ptr<bool> shared_bool_ptr;

public:
	vertex() : type(new event_type("")) { id = -1; };
	vertex(event_type_ptr et) { type = et; id = 0; }
	vertex(vertex& v, short id) { type = v.type, this->id = id + std::abs(rand()) % 200; } // используется только для новых вершин!
	~vertex();
	void set_accepting() { *is_accepting = true; }
	event_type_ptr get_type() const
	{
		return this->type;
	}
	short get_id() { return id; }
	shared_bool_ptr is_acc() { return is_accepting; }
	shared_bool_ptr is_acc() const { return is_accepting; }
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
		return std::pair<event_type_ptr, size_t>(this->type, this->id) < std::pair<event_type_ptr, size_t>(v.type, v.id);
	}
	friend bool operator==(const vertex& v, const event_type_ptr& a);
	friend bool operator< (const vertex& v, const event_type_ptr& a);
	friend bool operator< (const event_type_ptr& a, const vertex& v);
	friend std::ostream& operator<<(std::ostream& out, const vertex& t);
	/*Fields*/
protected:
	//bool is_old = true;
	short id;
	event_type_ptr type;
	shared_bool_ptr is_accepting = std::make_shared<bool>(false);
};

