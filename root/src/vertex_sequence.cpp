#include "vertex_sequence.h"


bool operator<(const vertex_sequence & a, const event_sequence & b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}
bool operator<(const event_sequence & a, const vertex_sequence & b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}
std::ostream & operator<<(std::ostream & out, const vertex_sequence & t)
{
    for (const auto& r : t)
        out << r;
    return out;
}
bool operator==(const event_sequence & a, const vertex_sequence & b)
{
    return equal(a.begin(), a.end(), b.begin(), b.end(), [](const event_type_ptr& a, const vertex& b) -> bool 
    {
		return a->get_type() == b.get_type()->get_type();
    });
}
bool operator==(const vertex_sequence & a, const event_sequence & b)
{
    return b == a;
}
