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
	event_type() {	}

	event_type(std::string str)	{ this->type = str;	}
    //event_type(char e) { this->_e = e; }

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
	std::string get_type()
	{
		return this->type;
	}
	const std::string get_type() const
	{
		return this->type;
	}
protected:
	std::string type;
	//char _e;
};

typedef std::shared_ptr<event_type> event_type_ptr;

class event_sequence
{
	typedef std::vector<event_type_ptr> vec_evt_ptr;

public:
	event_sequence()
	{
	}
	event_sequence(std::vector<event_type_ptr> v) : event_sequence()
	{
		this->seq = v;
	}
    
	/*Iterators and operators*/
    typename vec_evt_ptr::iterator begin()
    {
        return seq.begin();
    }
    typename vec_evt_ptr::iterator end()
    {
        return seq.end();
    }
    typename vec_evt_ptr::const_iterator begin() const
    {
        return seq.begin();
    }
    typename vec_evt_ptr::const_iterator end() const
    {
        return seq.end();
    }
	bool operator<(const event_sequence& t) const
	{
		return std::lexicographical_compare(this->seq.begin(), this->seq.end(), t.seq.begin(), t.seq.end()); 
	}
	bool operator==(const event_sequence& t) const
	{
		return std::equal(this->seq.begin(), this->seq.end(), t.seq.begin(), t.seq.end());
	}

	/*Methods*/
	void push_back(event_type_ptr a)
	{
		this->seq.push_back(a);
	}
	void resize(int p)
	{
		this->seq.resize(p);
	}
	int size()
	{
		return seq.size();
	}
protected:
	/*Fields*/
	vec_evt_ptr seq;
};
bool operator<(const event_type_ptr& a, const event_type_ptr& b);
std::ostream& operator<<(std::ostream& out, const event_type& t);
std::ostream& operator<<(std::ostream& out, const event_sequence& t);
bool operator==(const event_type_ptr& a, const  event_type_ptr& b);