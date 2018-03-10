#include "vertex.h"
#include <iostream>

vertex::~vertex()
{

}

std::ostream & operator<<(std::ostream & out, const vertex & t)
{
	//out << "\"{ type: " << *(t.type) << ", id: " << t.id << " }\"";
	out << *(t.type);
	return out;
}
