#include "split_merge.h"
#include <iostream>
#include <fstream>

template <class T>
void dprint(T&& t, std::string file = "output.txt")
{
    std::ofstream out(file);
    out << t << std::endl;
    out.close();
}
template <class T>
void deb_print(T&& t, std::string file = "debug_output.txt")
{
	std::ofstream out(file, std::ios::app);
	out << t << std::endl;
	out.close();
}
template <class T>
void out(T&& t, std::string file = "output.dot")
{
	std::ofstream out(file);
	out << t << std::endl;
	out.close();
}
void split_merge::build()
{
    for (std::string t : _log)
        process_trace(t);
    build_init_ts();
	if (_order > 2)
	{
		process_ts();
		refine();
	}
	dprint(_ts);
	out(_ts);
}

void split_merge::refine()
{
	std::set<edge> to_remove;
	for (const edge& t : _ts)
	{
		if (!t.get_is_visited())
			to_remove.insert(t);
	}
	for (auto& p : to_remove)
		_ts.remove_edge(p);
}
void split_merge::process_trace(std::string& tr)
{
    get_pairs(tr);
    get_chains(tr);
}
void split_merge::get_pairs(std::string& tr)
{
    for (int i = 0; i < tr.length() - 1; ++i)
    {
        std::pair<event_type_ptr, event_type_ptr> f = std::pair<event_type_ptr, event_type_ptr>(event_type_ptr(new event_type(tr.substr(i,1))), event_type_ptr(new event_type(tr.substr(i+1,1))));
        if (_pairs.find(f) == _pairs.end())
            _pairs[f] = 1;
        else
            ++_pairs[f];
    }
}
void split_merge::get_chains(std::string& tr)
{
	if (tr.length() >= _order)
	{
		for (int i = 0; i <= tr.length() - _order; ++i)
		{
			int k = 0;
			std::vector<event_type_ptr> temp;
			while (k < _order)
				temp.push_back(event_type_ptr(new event_type(tr.substr(k++ + i, 1))));
			event_sequence es(temp);
			//std::cout << es << std::endl;
			_chains.push_back(es);
			if (_chains_to_num.find(es) == _chains_to_num.end())
				_chains_to_num[es] = 1;
			else
				++_chains_to_num[es];
		}
	}
}
void split_merge::build_init_ts()
{
    for (auto& q : _pairs)
    {
        _ts.add_edge(vertex(q.first.first), vertex(q.first.second));
        //std::cout << *q.first.first << "->" << *q.first.second << std::endl;
    }
	deb_print(_ts);
}
void split_merge::process_ts()
{
    for (int i = 0; i < _log.size(); i++)
    {
        std::string tr = _log[i];
        replay_trace(tr);
		deb_print(_ts);
    }
}
void split_merge::replay_trace(std::string& tr)
{
	vertex s = _ts.get_init_state();
	vertex a = vertex(event_type_ptr(new event_type(tr.substr(0, 1))));
	edge r(s, a);
    r.visit();
	_ts.insert_edge(r);
	vertex current_vertex = r.second();
	int i;
	for (i = 0; i <= tr.length() - _order; i++)
	{
		event_sequence current_seq;
		for (int j = 0; j < _order; ++j)
		{
			if (i + j < tr.length() && tr.substr(i + j, 1) != "")
				current_seq.push_back(event_type_ptr(new event_type(tr.substr(i + j, 1))));
		}
		if (current_seq.size() >= _order)
		{
			std::vector<vertex_sequence> paths = _ts.dfs_stack(_order, current_vertex);
			std::cout << "Current vertex: " << current_vertex << std::endl;
			std::cout << "Current sequence: " << current_seq << std::endl;
			std::cout << "All sequences: " << std::endl;
			//for (auto t : paths)
			//	std::cout << t << std::endl;
			//remove_invalid(paths);
			//paths = _ts.dfs_stack(_order, current_vertex); //обновить пути после удаления!
			auto pi = std::find(paths.begin(), paths.end(), current_seq);
			if (pi == paths.end())
			{
				vertex_sequence v = recover_seq(current_seq, current_vertex);
				current_vertex = v[1];
				_ts.visit_seq(v);
				deb_print(_ts);
			}
			else
				current_vertex = _ts.visit_seq(*pi);
			remove_invalid(paths);
		}
		else
			break;
	}
	check_tail(current_vertex, tr, i);
}

void split_merge::check_tail(vertex& cv, std::string tr, int i)
{
	for (int k = 0; k < _order - 1; k++)
	{
		std::vector<vertex_sequence> t = _ts.dfs_stack(_order, cv);
		remove_invalid(t);

		edge_vector e = _ts[cv];
		for (edge& q : e)
		{
			if (q.second().get_type() == event_type_ptr(new event_type(tr.substr(i + k, 1))))
			{
				cv = q.second();
				break;
			}
		}
		if (k == _order - 2)
			cv.set_accepting();
	}
}
void split_merge::remove_invalid(std::vector<vertex_sequence> paths)
{
	std::vector<vertex_sequence> invalid(paths.size());
	std::copy_if(paths.begin(), paths.end(), invalid.begin(), [&a = this->_chains_to_num](const vertex_sequence& r) -> bool {
		auto it = find_if(a.begin(), a.end(), [&r](const std::pair<event_sequence, short>& e) -> bool {
			return e.first == r;
		});
		return (it == a.end());
	});
	std::set<edge> to_remove;
	std::cout << "Invalid: " << std::endl;
	for (auto& p : invalid)
	{
		std::cout << p << std::endl;
		for (auto it = p.begin(); it != p.end() && it + 1 != p.end(); it++)
		{
			edge n = _ts.find_edge(*it, *(it + 1));
			if (!n.get_is_visited())
			{
				to_remove.insert(n);
				break;
			}
		}
	}
	for (auto& p : to_remove)
	{
		_trunk.push_back(p);
		_ts.remove_edge(p);
	}
}
vertex_sequence split_merge::recover_seq(const event_sequence& cur_seq, const vertex& cur_vertex)
{
    vertex lc = cur_vertex;
	vertex prev;
    vertex_sequence recovered;
    recovered.push_back(lc);
    for (auto it = cur_seq.begin(); it != cur_seq.end() && it + 1 != cur_seq.end(); it++)
    {
        bool found = false;
        edge_vector es = _ts[lc]; //все инцидентные ребра
        for (edge& f : es)
        {
            if (f.first() == lc && f.second() == *(it + 1)) // ищем нужную вершину, которая следующая в последовательности
            {
                recovered.push_back(f.second());
				prev = lc;
                lc = f.second();
                found = true;
                f.visit();
            }
        }
        if (!found)
        {
            bool se = false;
            bool se2 = false;

            bool check_invalid = false;
            bool check_desired = false;

			edge inserting; 
			edge_vector::iterator r = std::find_if(_trunk.begin(), _trunk.end(), [&it](const edge& existing) -> bool {
                return existing.first() == (*it) && existing.second() == *(it + 1) && existing.get_num_visits() != -1;
            });
			if (r != _trunk.end())
				inserting = *r;
			else
			{
				inserting = edge(lc, vertex(*(it + 1)), -1);
				_trunk.push_back(inserting);
			}
            std::set<vertex> ex = _ts.get_vertices(inserting.second().get_type());
            for (vertex r : ex)
            {
                inserting = edge(lc, r);
                _ts.insert_edge(inserting);
                std::tie(check_invalid, check_desired) = check_added(inserting, cur_seq);
                if (se = !check_invalid && check_desired)
                {
                    recovered.push_back(inserting.second());
					prev = lc;
                    lc = inserting.second();
					inserting.visit();
                    break;
                }
				_ts.remove_edge(inserting);
            }
            if (!se)
            {
                edge ins2(lc, vertex(inserting.second(), inserting.second().get_id()));
                bool t;
				_ts.insert_edge(ins2);
                std::tie(se2, t) = check_added(ins2, cur_seq);
				if (!se2)
				{
					prev = lc;
					lc = ins2.second();
					ins2.visit();
				}
                else
                {
					_ts.remove_edge(ins2);
					recovered.pop_back();
                    edge ins3(vertex(inserting.first(), inserting.first().get_id()), vertex(inserting.second(), inserting.second().get_id()));
					recovered.push_back(ins3.first());
                    edge_vector es = _ts[prev];
                    for (auto& e : es)
                    {
                        if (e.second().get_type() == inserting.first().get_type())
                        {
                            _ts.copy_out_edges(e.second(), ins3.first());
							if (e.first() != _ts.get_init_state())
								_ts.remove_edge(e);
                        }
                    }
					edge qw(prev, ins3.first());
					qw.visit();
                    _ts.insert_edge(qw);
					ins3.visit();
                    _ts.insert_edge(ins3);
					prev = lc;
                    lc = ins3.second();
					deb_print(_ts);
                }
                recovered.push_back(lc);
            }
        }
    }
    return recovered;
}

std::tuple<bool, bool> split_merge::check_added(edge& e, const event_sequence& cur)
{
    using namespace std;
    vector<vertex_sequence> forw, revs, temp, temp2;
    bool flag_inv = false, flag_des = false;
	vertex init = _ts.get_init_state();
    temp = _ts.dfs_stack(_order, e.second(), true);
	temp2.resize(temp.size());
	std::copy_if(temp.begin(), temp.end(), temp2.begin(), [init](vertex_sequence& vs) -> bool { 
		return (find(vs.begin(), vs.end(), init) == vs.end());
	});
	for (auto& t : temp2)
	{
		if (t.get_size() == 0)
			continue;
		if (*(t.begin() + 1) == e.first())
		{
			reverse(t.begin(), t.end());
			revs.push_back(t);
		}
	}
    temp = _ts.dfs_stack(_order, e.first(), false);
	temp2.clear();
	temp2.resize(temp.size());
	std::copy_if(temp.begin(), temp.end(), temp2.begin(), [init](vertex_sequence& vs) -> bool {
		return (find(vs.begin(), vs.end(), init) == vs.end());
	});
	for (auto& t : temp2)
	{
		if (t.get_size() == 0)
			continue;
		if (*(t.begin() + 1) == e.second())
			forw.push_back(t);
	}

	for (int i = 0; i < _order - 2; i++)
	{
		for (auto & r : revs)
		{
			event_sequence seqs;
			for (auto& k : r)
				seqs.push_back(k.get_type());
			if (_chains_to_num.find(seqs) == _chains_to_num.end())
			{
				flag_inv = true; // мы нашли невалидную последовательность
				break;
			}
			if (seqs == cur)
				flag_des = true; //нашли нужную	
		}
		for (auto & f : forw)
		{
			event_sequence seqs;
			for (auto& k : f)
				seqs.push_back(k.get_type());
			if (_chains_to_num.find(seqs) == _chains_to_num.end())
			{
				flag_inv = true; // мы нашли невалидную последовательность
				break;
			}
			if (seqs == cur)
				flag_des = true; //нашли нужную	
		}
	}
    return std::make_tuple(flag_inv, flag_des);
}