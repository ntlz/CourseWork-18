#include "events.h"

std::ostream& operator<<(std::ostream& out, const event_type& t)
{
	out << t.type;
	return out;
}
std::ostream& operator<<(std::ostream& out, const event_sequence& t)
{
	for (const auto& r : t.seq)
		out << *r;
	return out;
}