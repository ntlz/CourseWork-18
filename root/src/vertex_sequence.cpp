#include "vertex_sequence.h"



vertex_sequence & vertex_sequence::set_predicate(std::function<bool(const vertex_vector&)> pred)
{
	this->_pred = pred;
	return *this;
}

bool vertex_sequence::is_valid(std::vector<event_sequence>& es, int begin, int end, double probability_thrhld, double count_thrhld)
{
    auto it = lower_bound(es.begin() + begin, es.begin() + begin + end, *this);
    return (it->count > count_thrhld && it->probability > probability_thrhld);
}
bool operator<(const vertex_sequence & a, const event_sequence & b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.seq.begin(), b.seq.end());
}
bool operator<(const event_sequence & a, const vertex_sequence & b)
{
    return std::lexicographical_compare(a.seq.begin(), a.seq.end(), b.begin(), b.end());
}
std::ostream & operator<<(std::ostream & out, const vertex_sequence & t)
{
    for (const auto& r : t)
        out << r;
    return out;
}
vertex_sequence::vertex_sequence()
{
}

vertex_sequence::~vertex_sequence()
{
}
