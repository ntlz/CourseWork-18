#include "events.h"

bool operator<(const event_type_ptr & a, const event_type_ptr & b)
{
    return *a < *b;
}

std::ostream& operator<<(std::ostream& out, const event_type& t)
{
	out << t.type;
	return out;
}
std::ostream& operator<<(std::ostream& out, const event_sequence& t)
{
	for (const auto& r : t)
		out << *r;
	return out;
}

bool operator==(const event_type_ptr & a, const event_type_ptr & b)
{
    return a->type == b->type;
}
