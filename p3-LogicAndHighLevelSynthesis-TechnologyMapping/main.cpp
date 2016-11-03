#include <GTL/graph.h>

enum node_type {ROOT, LEAF, INTERMEDIATE};
enum cell_type {INV, NAND2, NAND3, NAND4, NOR2, NOR3, NOR4, AOI21, AOI22, OAI21, OAI22, XOR2, INPUT, OUTPUT};

void decompose_nand(node cell_node, graph & G, node_map<cell_type> cell_types) {

}

void decompose_nor(node cell_node, graph & G, node_map<cell_type> cell_types) {

}

void decompose_aoi(node cell_node, graph & G, node_map<cell_type> cell_types) {

}

void decompose_oai(node cell_node, graph & G, node_map<cell_type> cell_types) {

}

void decompose_xor(node cell_node, graph & G, node_map<cell_type> cell_types) {

}

void decompose_graph(graph & G, node_map<cell_type> cell_types) {
  for (auto node_it = G.nodes_begin(); node_it != G.nodes_end(); ++node_it) {
    switch(cell_types[*node_it]) {
      case INV:
      break;
      case INPUT:
      break;
      case OUTPUT:
      break;
      case NAND2:
      decompose_nand(*node_it, G, cell_types);
      break;
      case NAND3:
      decompose_nand(*node_it, G, cell_types);
      break;
      case NAND4:
      decompose_nand(*node_it, G, cell_types);
      break;
      case NOR2:
      decompose_nor(*node_it, G, cell_types);
      break;
      case NOR3:
      decompose_nor(*node_it, G, cell_types);
      break;
      case NOR4:
      decompose_nor(*node_it, G, cell_types);
      break;
      case AOI21:
      decompose_aoi(*node_it, G, cell_types);
      break;
      case AOI22:
      decompose_aoi(*node_it, G, cell_types);
      break;
      case OAI21:
      decompose_oai(*node_it, G, cell_types);
      break;
      case OAI22:
      decompose_oai(*node_it, G, cell_types);
      break;
      case XOR2:
      decompose_xor(*node_it, G, cell_types);
      break;
    }
  }
}

void build_tree(node & parent, node & current_node, graph & tree) {
  auto new_node = tree.new_node();
  tree.new_edge(new_node, parent);

  if (current_node.indeg() == 0 || current_node.outdeg() > 1) {
    return;
  }

  for (auto edge_it = current_node.in_edges_begin(); edge_it != current_node.in_edges_end(); ++edge_it) {
    auto child = edge_it->source();
    build_tree(new_node, child, tree);
  }
}

void partition_graph(const graph & G, std::vector<graph> & forest) {
  std::vector<node> root_nodes;

  for (auto node_it = G.nodes_begin(); node_it != G.nodes_end(); ++node_it) {
    if (node_it->outdeg() > 1 || node_it->outdeg() == 0) {
      root_nodes.push_back(*node_it);
    }
  }

  for (auto root : root_nodes) {
    graph tree;
    auto new_root = tree.new_node();
    for (auto edge_it = root.in_edges_begin(); edge_it != root.in_edges_end(); ++edge_it) {
      auto child = edge_it->source();
      build_tree(new_root, child, tree);
    }
    forest.push_back(tree);
  }
}
