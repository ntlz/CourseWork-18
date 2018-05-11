#pragma once
#include "events.h"
#include "graph.h"
#include "vertex_sequence.h"
#include <tuple>

class split_merge
{
    typedef std::set<edge> edge_set;
    typedef std::vector<edge> edge_vector;
    typedef std::pair<event_type_ptr, event_type_ptr> ev_pair;

public:
    split_merge() { }
    split_merge(std::vector<std::vector<std::string>> log, int k);
	void to_json(std::string of);
	void build(std::string of);
	void add_loops();
protected:
    void process_trace(std::vector<event_type_ptr>& trace);
    void get_pairs(std::vector<event_type_ptr>& tr);
    void get_chains(std::vector<event_type_ptr>& tr);
    //void initialize_heads(event_type_ptr a);
    void process_ts();
	std::vector<event_type_ptr> shrink_trace(std::vector<event_type_ptr> trace);
    void replay_trace(std::vector<event_type_ptr> & tr);
    std::tuple<bool, bool> check_added(edge & e, const event_sequence& cur);
	void iterate_seq(event_sequence& current_seq, vertex& current_vertex);
	void check_tail(vertex & cv, std::vector<event_type_ptr> ts, int i);
    void remove_invalid(std::vector<vertex_sequence> paths, vertex vc);
    vertex_sequence recover_seq(const event_sequence & cur_seq, const vertex& cur_vertex);
    void build_init_ts();
    void refine();

    int _order;
    graph _ts;
    std::vector <std::vector< event_type_ptr>> _log;
    std::vector<event_sequence> _chains;
    std::map<event_sequence, short> _chains_to_num;
    std::map<ev_pair, short> _pairs;
    std::vector<edge> _trunk;
};