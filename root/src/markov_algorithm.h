#include "events.h"
#include "graph.h"
#include "vertex_sequence.h"


class markov_algorithm
{
    typedef std::pair<vertex, vertex> edge;
    typedef std::set<edge> edge_set;
public:
    double probability_thd;
    int count_thd;
    int table_order;
    bool do_split;

    graph event_graph;
    graph bi_event_graph;
    std::string trace;
    std::vector<event_type_ptr> input_types; 
    std::vector<event_sequence> all_event_seq;
public:
    
    void build_graph();
    void do_magic();
	void do_magic_alternatively();
    int* get_brinks(std::string p);
    int get_substrings(std::string t, event_sequence& subst);
    void rec(const std::vector<event_type_ptr> & al, std::vector<int>::iterator begin, std::vector<int>::iterator pos, std::vector<int>::iterator end, std::set<event_sequence>& es);
    void get_all_subseq(std::string& trace, int n, std::vector<event_sequence>& es, std::vector<event_type_ptr>& al);
    void set_count_and_prob(std::string& trace, std::vector<event_sequence>& seqs);
    bool resolve(vertex_sequence& illegal_sequence);
	bool almost_resolve(vertex_sequence& illegal_sequence);
    bool _try_split(vertex_sequence& illegal_seq, vertex vs, int pos);
    vertex iterate_modifications(vertex_sequence& illegal_seq);

    markov_algorithm(std::string& t, double p, int c, int to, bool split) : trace(t), probability_thd(p), count_thd(c), table_order(to), do_split(split)
    {
        get_all_subseq(trace, to, all_event_seq, input_types);
        set_count_and_prob(trace, all_event_seq);
    }
};