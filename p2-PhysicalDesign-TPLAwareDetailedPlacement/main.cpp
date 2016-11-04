#include <iostream>
#include "placement.h"
#include <assert.h>
#include "graph.h"

int main() {
    std::cout << "TPL Detailed Placement" << std::endl;

    Placement placement(5, 2, 5);
    //Add Rows
    id_type r1 = placement.rows.add(20, 0);
    id_type r2 = placement.rows.add(20, 0);
    id_type r3 = placement.rows.add(20, 0);
    id_type r4 = placement.rows.add(20, 0);
    id_type r5 = placement.rows.add(20, 0);
    //Add Cells
    id_type c1 = placement.cells.add({0,0}, 5);
    id_type c2 = placement.cells.add({10,0}, 8);
    id_type c3 = placement.cells.add({0,30}, 6);
    id_type c4 = placement.cells.add({10,30}, 5);
    id_type c5 = placement.cells.add({5,40}, 3);
    id_type c6 = placement.cells.add({14,40}, 5);
    //Add Nets
    id_type n1 = placement.nets.add({c1, c2});
    id_type n2 = placement.nets.add({c3, c4, c5, c6});

    assert(placement.hpwl(n1) == 10);
    assert(placement.hpwl(n2) == 24);

    //graph
    Directed_Graph graph(9, 14);
    std::vector<node_id> node_ids(6, INVALID_ID);
    for(size_t i = 0; i < node_ids.size(); ++i)
        node_ids.at(i) = graph.add_node();

    auto e0 = graph.add_edge(node_ids.at(0), node_ids.at(1));
    auto e1 = graph.add_edge(node_ids.at(0), node_ids.at(2));
    auto e2 = graph.add_edge(node_ids.at(1), node_ids.at(2));
    auto e3 = graph.add_edge(node_ids.at(1), node_ids.at(3));
    auto e4 = graph.add_edge(node_ids.at(1), node_ids.at(4));
    auto e5 = graph.add_edge(node_ids.at(2), node_ids.at(4));
    auto e6 = graph.add_edge(node_ids.at(3), node_ids.at(4));
    auto e7 = graph.add_edge(node_ids.at(3), node_ids.at(5));
    auto e8 = graph.add_edge(node_ids.at(4), node_ids.at(5));

    std::vector<cost_type> edge_costs;
    edge_costs.reserve(9);
    edge_costs.push_back(3);
    edge_costs.push_back(4);
    edge_costs.push_back(6);
    edge_costs.push_back(2);
    edge_costs.push_back(7);
    edge_costs.push_back(5);
    edge_costs.push_back(1);
    edge_costs.push_back(8);
    edge_costs.push_back(4);

    std::vector<node_id> prev;
    std::vector<cost_type> min_distance;
    shortest_path(graph, node_ids.front(), node_ids.back(), edge_costs, min_distance, prev);
    std::cout << "Shortest Path from node 0 -> 5 is: " << min_distance.back() << std::endl;
    std::cout << "Tour back from sink" << std::endl;
    node_id id = prev.back();
    std::cout << node_ids.back() << "->";;
    while(id != INVALID_ID)
    {
        std::cout << id << "->";;
        id = prev.at(id);
    }
    std::cout << std::endl;

    //lookup
    assert(lut_min_distance(1,0, 1,1) == 14);
    assert(lut_min_distance(1,1, 0,0) == 3);
    assert(lut_min_distance(0,1, 1,1) == 13);

    //TPL-aware
    Placement placement_1(2, 1, 1);
    //Add Rows
    r1 = placement.rows.add(5, 0);
    //Add Cells
    c1 = placement.cells.add({0,0}, 2);
    c2 = placement.cells.add({2,0}, 2);
    //Add Nets
    n1 = placement.nets.add({c1, c2});

    //assuming 2 colors
    size_t num_nodes = 5*placement_1.num_sites(r1) + 1;
    Directed_Graph graph_1(num_nodes,20);

    std::cout << "Done! " << std::endl;
    return 0;
}
