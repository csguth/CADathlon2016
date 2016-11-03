#include <iostream>
#include <GTL/graph.h>
#include "algorithms.cpp"

int main() {
    graph G;
    auto input1 = G.new_node();
    auto input2 = G.new_node();
    auto input3 = G.new_node();
    auto input4 = G.new_node();
    auto and1 = G.new_node();
    auto and2 = G.new_node();
    auto and3 = G.new_node();
    auto output1 = G.new_node();
    auto output2 = G.new_node();

    G.new_edge(input1, and1);
    G.new_edge(input2, and2);
    G.new_edge(input3, and2);
    G.new_edge(input4, and3);

    G.new_edge(and2, and1);
    G.new_edge(and2, and3);

    G.new_edge(and1, output1);
    G.new_edge(and3, output2);

    std::vector<graph> forest;
    partition_graph(G, forest);

    assert(forest.size() == 3);

    auto tree1 = forest.at(0);
    assert(tree1.number_of_nodes() == 3);
    assert(tree1.number_of_edges() == 2);

    auto tree2 = forest.at(1);
    assert(tree2.number_of_nodes() == 4);
    assert(tree2.number_of_edges() == 3);

    auto tree3 = forest.at(2);
    assert(tree2.number_of_nodes() == 4);
    assert(tree2.number_of_edges() == 3);

    std::cout << "working! ;D" << std::endl;

    return 0;
}
