#include <iostream>
#include <GTL/graph.h>
#include "algorithms.cpp"

void nand2_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = NAND2;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 5);
  assert(cell_graph.number_of_edges() == 4);
}

void nand3_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = NAND3;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 7);
  assert(cell_graph.number_of_edges() == 6);
}

void nand4_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = NAND4;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node input_node4 = cell_graph.new_node();
  cell_types[input_node4] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(input_node4, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 9);
  assert(cell_graph.number_of_edges() == 8);
}

void nor2_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = NOR2;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 5);
  assert(cell_graph.number_of_edges() == 4);
}

void nor3_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = NOR3;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 7);
  assert(cell_graph.number_of_edges() == 6);
}

void nor4_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = NOR4;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node input_node4 = cell_graph.new_node();
  cell_types[input_node4] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(input_node4, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 9);
  assert(cell_graph.number_of_edges() == 8);
}

void aoi21_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = AOI21;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 7);
  assert(cell_graph.number_of_edges() == 6);
}

void aoi22_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = AOI22;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node input_node4 = cell_graph.new_node();
  cell_types[input_node4] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(input_node4, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 9);
  assert(cell_graph.number_of_edges() == 8);
}

void oai21_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = OAI21;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 7);
  assert(cell_graph.number_of_edges() == 6);
}

void oai22_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = OAI22;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node input_node3 = cell_graph.new_node();
  cell_types[input_node3] = INPUT;
  node input_node4 = cell_graph.new_node();
  cell_types[input_node4] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(input_node3, cell_node);
  cell_graph.new_edge(input_node4, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 9);
  assert(cell_graph.number_of_edges() == 8);
}

void xor2_test() {
  node_map<cell_type> cell_types;

  graph cell_graph;
  node cell_node = cell_graph.new_node();
  cell_types[cell_node] = XOR2;
  node input_node1 = cell_graph.new_node();
  cell_types[input_node1] = INPUT;
  node input_node2 = cell_graph.new_node();
  cell_types[input_node2] = INPUT;
  node output_node = cell_graph.new_node();
  cell_types[output_node] = OUTPUT;

  cell_graph.new_edge(input_node1, cell_node);
  cell_graph.new_edge(input_node2, cell_node);
  cell_graph.new_edge(cell_node, output_node);

  decompose_graph(cell_graph, cell_types);

  assert(cell_graph.number_of_nodes() == 8);
  assert(cell_graph.number_of_edges() == 9);
}

int main() {

    nand2_test();
    nand3_test();
    nand4_test();
    nor2_test();
    nor3_test();
    nor4_test();
    aoi21_test();
    aoi22_test();
    oai21_test();
    oai22_test();
    xor2_test();

    std::cout << "working! ;D" << std::endl;

    return 0;
}
