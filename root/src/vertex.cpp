#include "vertex.h"
#include <iostream>

vertex::~vertex()
{

}

bool operator==(const vertex & v, const event_type_ptr & a)
{
    return v.type == a;
}

bool operator<(const vertex & v, const event_type_ptr & a)
{
    return *(v.type) < *a;
}

bool operator<(const event_type_ptr & a, const vertex & v)
{
    return *a < *(v.type);
}

std::ostream & operator<<(std::ostream & out, const vertex & t)
{
	//out << "\"{ type: " << *(t.type) << ", id: " << t.id << " }\"";
	if (t.type == event_type_ptr(new event_type("")))
        out << "\"" << "s0" << "(" << t.id << ")\"";
    else
        out << "\""<< *(t.type) << "(" << t.id << ")\"";
	return out;
}
