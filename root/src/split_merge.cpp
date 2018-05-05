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
void split_merge::build()
{
    for (std::string t : _log)
        process_trace(t);

    build_init_ts();
    process_ts();
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
        if (i == 0)
            initialize_heads(f.first);
        if (_pairs.find(f) == _pairs.end())
            _pairs[f] = 1;
        else
            ++_pairs[f];
    }
}
void split_merge::get_chains(std::string& tr)
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
void split_merge::initialize_heads(event_type_ptr a)
{
    vertex v = _ts.get_init_state();
    vertex f(a);
    _ts.add_edge(v, a);
}
void split_merge::build_init_ts()
{
    for (auto& q : _pairs)
    {
        _ts.add_edge(vertex(q.first.first), vertex(q.first.second));
        //std::cout << *q.first.first << "->" << *q.first.second << std::endl;
    }
    dprint(_ts);
}
void split_merge::process_ts()
{
    for (int i = 0; i < _log.size(); i++)
    {
        std::string tr = _log[i];
        replay_trace(tr);
    }
}

void split_merge::replay_trace(std::string& tr)
{
    vertex s = _ts.get_init_state();
    vertex a = vertex(event_type_ptr(new event_type(tr.substr(0, 1))));
    edge init = _ts.find_edge(s, a);

    vertex current_vertex = init.second();
	int i;
    for (i = 0; i <= tr.length() - _order; i++)
    {
        event_sequence current_seq;
        for (int j = 0; j < _order; ++j)
            current_seq.push_back(event_type_ptr(new event_type(tr.substr(i + j, 1))));
        std::vector<vertex_sequence> paths = _ts.dfs_stack(_order, current_vertex);
		std::cout << "Current vertex: " << current_vertex << std::endl;
		std::cout << "Current sequence: " << current_seq << std::endl;
		std::cout << "All sequences: " << std::endl;
		for (auto t : paths)
			std::cout << t << std::endl;
		//remove_invalid(paths);

		//paths = _ts.dfs_stack(_order, current_vertex); //обновить пути после удаления!
        auto pi = std::find(paths.begin(), paths.end(), current_seq);
        if (pi == paths.end())
        {
            vertex_sequence v = recover_seq(current_seq, current_vertex);
            current_vertex = v[1];
        }
        else
            current_vertex = _ts.visit_seq(*pi);
		remove_invalid(paths);
		deb_print(_ts);
    }
	for (int k = 0; k < _order - 1; k++)
	{
		std::vector<vertex_sequence> t = _ts.dfs_stack(_order, current_vertex);
		remove_invalid(t);

		edge_vector e = _ts[current_vertex];
		for (edge& q : e)
		{
			if (q.second().type == event_type_ptr(new event_type(tr.substr(i + k, 1))))
				current_vertex = q.second();
		}
	}
	dprint(_ts);
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
			if (!n._is_visited)
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
    vertex_sequence recovered;
    recovered.push_back(lc);
    for (auto it = cur_seq.begin(); it != cur_seq.end() && it + 1 != cur_seq.end(); it++)
    {
        bool found = false;
        edge_vector es = _ts[lc]; //все инцидентные ребра
        for (edge& f : es)
        {
            if (f.first() == *it && f.second() == *(it + 1)) // ищем нужную вершину, которая следующая в последовательности
            {
                recovered.push_back(f.second());
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
                return existing.first() == (*it) && existing.second() == *(it + 1) && existing.num_visits != -1;
            });
			if (r != _trunk.end())
				inserting = *r;
			else
			{
				inserting = edge(vertex(*it), vertex(*(it + 1)), -1);
				_trunk.push_back(inserting);
			}
            std::set<vertex> ex = _ts.get_vertices(inserting.second().type);
            for (vertex r : ex)
            {
                inserting = edge(lc, r);
                _ts.insert_edge(inserting);
                std::tie(check_invalid, check_desired) = check_added(inserting, cur_seq);
                if (se = !check_invalid && check_desired)
                {
                    recovered.push_back(inserting.second());
                    lc = inserting.second();
                    break;
                }
				_ts.remove_edge(inserting);
            }
            if (!se)
            {
                edge ins2(lc, vertex(inserting.second(), inserting.second().id));
                bool t;
				_ts.insert_edge(ins2);
                std::tie(se2, t) = check_added(ins2, cur_seq);
				if (!se2)
					lc = ins2.second();
                else
                {
					_ts.remove_edge(ins2);
                    edge ins3(vertex(inserting.first(), inserting.first().id), vertex(inserting.second(), inserting.second().id));
                    edge_vector es = _ts[lc];
                    for (auto& e : es)
                    {
                        if (e.second().type == inserting.first().type)
                        {
                            _ts.copy_out_edges(e.second(), ins3.first());
                            _ts.remove_edge(e);
                        }
                    }
                    _ts.add_edge(lc, ins3.first());
                    _ts.insert_edge(ins3);
                    lc = ins3.first();
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
    vector<vertex_sequence> forw, revs, temp;
    bool flag_inv = false, flag_des = false;

    temp = _ts.dfs_stack(_order, e.second(), true);
    for (auto& t : temp)
        if (*(t.begin() + 1) == e.first())
        {
            reverse(t.begin(), t.end());
            revs.push_back(t);
        }
    temp = _ts.dfs_stack(_order, e.first(), false);
    for (auto& t : temp)
        if (*(t.begin() + 1) == e.second())
            forw.push_back(t);

	for (int i = 0; i < _order - 2; i++)
	{
		/*for (auto & r : revs)
		{
			for (auto& f : forw)
			{
				event_sequence seqs;
				vector<vertex> aaa;
				aaa.resize(_order);

				//copy(b.begin() + 2, b.end() - b.size() + 2 + i, it);
				auto it = copy(r.begin() + i, r.end(), aaa.begin());
				copy(f.begin() + 2, f.end() - f.get_size() + 2 + i, it);
				for (auto& u : aaa)
					seqs.push_back(u.type);
				std:cout << seqs << std::endl;
				if (_chains_to_num.find(seqs) == _chains_to_num.end())
				{
					flag_inv = true; // мы нашли невалидную последовательность
					break;
				}
				if (seqs == cur)
					flag_des = true; //нашли нужную
			}
			if (flag_inv)
				break;
		}
		if (flag_inv)
			break;
	}*/
		for (auto & r : revs)
		{
			event_sequence seqs;
			for (auto& k : r)
				seqs.push_back(k.type);
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
				seqs.push_back(k.type);
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