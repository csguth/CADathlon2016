#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <limits>
#include <set>

using node_id = int;
using edge_id = int;
using edge_type = std::pair<node_id, node_id>;
using adjacency_list = std::vector<size_t>;
using nodes_container = std::vector<adjacency_list>;
using edges_container = std::vector<edge_type>;

static constexpr int INVALID_NODE = -1;


class Directed_Graph
{
    private:
        edges_container edges;  
        nodes_container nodes;
    public:
        Directed_Graph(unsigned num_nodes, unsigned num_edges)
        {
            nodes.reserve(num_nodes);
            edges.reserve(num_edges);
        }
        
        size_t nodes_size() const {nodes.size();}
        size_t edges_size() const {edges.size();}
        node_id add_node()
        {
            nodes.push_back(adjacency_list(0));
            return nodes.size()-1;
        }
        edge_id add_edge(node_id from, node_id to) 
        {
            adjacency_list & list = nodes.at(from);
            edges.push_back({from, to});
            list.push_back(edges.size()-1);
            return edges.size()-1;
        }
        const adjacency_list & out_edges(node_id node) const
        {
            return nodes.at(node);
        }
        edge_type edge(edge_id edge) const {return edges.at(edge);}

};

//struct compare  
//{  
//    bool operator()(const std::pair<int, node_id>& l, const std::pair<int, node_id>& r) {return l.first > r.first;}  
//};

//std::priority_queue<std::pair<int,node_id>, std::vector<std::pair<int,node_id>>, compare> queue;

using cost_type = int;

//dijkstra min-priority queue
void shortest_path(const Directed_Graph & graph, node_id source, node_id target, const std::vector<cost_type> & edges_cost, std::vector<cost_type> & min_distance, std::vector<node_id> & prev)
{
    min_distance.resize(graph.nodes_size(), std::numeric_limits<cost_type>::max());    
    prev.resize(graph.nodes_size(), INVALID_NODE);    
    min_distance.at(source) = 0; 

    std::set<std::pair<cost_type,node_id>> nodes_to_process;
    nodes_to_process.insert({0, source});
    
    while(!nodes_to_process.empty())
    {
        node_id u = nodes_to_process.begin()->second;
        if(u == target)
            return;

        nodes_to_process.erase(nodes_to_process.begin());
        for(auto e : graph.out_edges(u))
        {
            node_id v = graph.edge(e).second;
            int cur_dist = min_distance.at(u) +  edges_cost.at(e);
            if(cur_dist < min_distance.at(v))
            {
                nodes_to_process.erase({ min_distance.at(v), v});
                nodes_to_process.insert({ cur_dist, v});
                min_distance.at(v) = cur_dist;
                prev.at(v) = u;
            }
        }
    }
}

#endif
