#include <iostream>
#include <GTL/graph.h>
#include "algorithms.cpp"

int main() {
    std::cout << "\n RUN p3-LogicAndHighLevelSynthesis-TechnologyMapping \n" << std::endl;

    graph G;
    node_map<cell_type> node_map_G;
    node and_G = G.new_node();
    node inv_G = G.new_node();
    node a_G = G.new_node();
    node b_G = G.new_node();
    node_map_G[and_G] = AND2;
    node_map_G[inv_G] = INV;
    node_map_G[a_G] = INPUT;
    node_map_G[b_G] = INPUT;
    G.new_edge(a_G, and_G);
    G.new_edge(b_G, inv_G);
    G.new_edge(inv_G, and_G);

    graph H;
    node_map<cell_type> node_map_H;
    node and_H = H.new_node();
    node inv_H = H.new_node();
    node a_H = H.new_node();
    node b_H = H.new_node();
    node_map_H[and_H] = AND2;
    node_map_H[inv_H] = INV;
    node_map_H[a_H] = INPUT;
    node_map_H[b_H] = INPUT;
    H.new_edge(b_H, inv_H);
    H.new_edge(inv_H, and_H);
    H.new_edge(a_H, and_H);


    if(match(and_G, and_H, node_map_G, node_map_H)){
        std::cout << "match TRUE" << std::endl;
    }else{
        std::cout << "match FALSE" << std::endl;
    }

    return 0;
}
