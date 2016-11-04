#include <iostream>
#include <GTL/graph.h>

enum cell_type {INV, AND2, OR2, NAND2, NAND3, NAND4, NOR2, NOR3, NOR4, AOI21, AOI22, OAI21, OAI22, XOR2, INPUT, OUTPUT};

//verify if the two graph is isomorfic and each node are same logic function
bool match(const node &u, const node &v, const node_map<cell_type> &map_u, const node_map<cell_type> &map_v){
    if(u.indeg() == 0){                                     /* Leaf of the pattern graph reached */
        return true;
    }else{
        if(map_u[u] != map_v[v])                            /* Logic function not equal*/
            return false;
        if(v.indeg() == 0)                                  /* Leaf of the subject graph reached */
            return false;

        if(u.indeg() != v.indeg())                          /* Degree mismatch */
            return false;

        if(v.indeg() == 1){                                 /* One child each: visit subtree recursively */
            node u_child = u.in_edges_begin()->source();
            node v_child = v.in_edges_begin()->source();
            if(map_u[u_child] == map_v[v_child]){
                return match(u_child, v_child, map_u, map_v);
            }else{
                return false;
            }
        }else{                                               /* Two children each: visit subtrees recursively */
            auto in_u_it = u.in_edges_begin();
            node u_child_l = in_u_it->source();
            in_u_it ++;
            node u_child_r = in_u_it->source();

            auto in_v_it = v.in_edges_begin();
            node v_child_l = in_v_it->source();
            in_v_it ++;
            node v_child_r = in_v_it->source();

            return ( match(u_child_l, v_child_l, map_u, map_v) && match(u_child_r, v_child_r, map_u, map_v) ||
                     match(u_child_r, v_child_l, map_u, map_v) && match(u_child_l, v_child_r, map_u, map_v) );
        }
    }
}

enum node_type {ROOT, LEAF, INTERMEDIATE};

void get_output_nodes(node current_node, std::vector<node> & output_nodes) {
  for (auto edge_it = current_node.out_edges_begin(); edge_it != current_node.out_edges_end(); ++edge_it) {
    output_nodes.push_back(edge_it->target());
  }
}

void get_input_nodes(node current_node, std::vector<node> & input_nodes) {
  for (auto edge_it = current_node.in_edges_begin(); edge_it != current_node.in_edges_end(); ++edge_it) {
    input_nodes.push_back(edge_it->source());
  }
}

void decompose_nand2(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node and_node = G.new_node();
  cell_types[and_node] = AND2;

  G.new_edge(and_node, inv_node);
  G.new_edge(input_nodes.at(0), and_node);
  G.new_edge(input_nodes.at(1), and_node);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }
  G.del_node(cell_node);
}

void decompose_nand3(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node and_node1 = G.new_node();
  cell_types[and_node1] = AND2;
  node and_node2 = G.new_node();
  cell_types[and_node2] = AND2;

  G.new_edge(and_node1, and_node2);
  G.new_edge(and_node2, inv_node);
  G.new_edge(input_nodes.at(0), and_node1);
  G.new_edge(input_nodes.at(1), and_node1);
  G.new_edge(input_nodes.at(2), and_node2);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }
  G.del_node(cell_node);
}

void decompose_nand4(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node and_node1 = G.new_node();
  cell_types[and_node1] = AND2;
  node and_node2 = G.new_node();
  cell_types[and_node2] = AND2;
  node and_node3 = G.new_node();
  cell_types[and_node3] = AND2;

  G.new_edge(and_node1, and_node3);
  G.new_edge(and_node2, and_node3);
  G.new_edge(and_node3, inv_node);
  G.new_edge(input_nodes.at(0), and_node1);
  G.new_edge(input_nodes.at(1), and_node1);
  G.new_edge(input_nodes.at(2), and_node2);
  G.new_edge(input_nodes.at(3), and_node2);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }
  G.del_node(cell_node);
}

void decompose_nor2(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node or_node = G.new_node();
  cell_types[or_node] = OR2;

  G.new_edge(or_node, inv_node);
  G.new_edge(input_nodes.at(0), or_node);
  G.new_edge(input_nodes.at(1), or_node);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }
  G.del_node(cell_node);
}

void decompose_nor3(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node or_node1 = G.new_node();
  cell_types[or_node1] = AND2;
  node or_node2 = G.new_node();
  cell_types[or_node2] = AND2;

  G.new_edge(or_node1, or_node2);
  G.new_edge(or_node2, inv_node);
  G.new_edge(input_nodes.at(0), or_node1);
  G.new_edge(input_nodes.at(1), or_node1);
  G.new_edge(input_nodes.at(2), or_node2);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }
  G.del_node(cell_node);
}

void decompose_nor4(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node or_node1 = G.new_node();
  cell_types[or_node1] = AND2;
  node or_node2 = G.new_node();
  cell_types[or_node2] = AND2;
  node or_node3 = G.new_node();
  cell_types[or_node3] = AND2;

  G.new_edge(or_node1, or_node3);
  G.new_edge(or_node2, or_node3);
  G.new_edge(or_node3, inv_node);
  G.new_edge(input_nodes.at(0), or_node1);
  G.new_edge(input_nodes.at(1), or_node1);
  G.new_edge(input_nodes.at(2), or_node2);
  G.new_edge(input_nodes.at(3), or_node2);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }
  G.del_node(cell_node);
}

void decompose_aoi21(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node and_node = G.new_node();
  cell_types[and_node] = AND2;
  node or_node = G.new_node();
  cell_types[or_node] = OR2;

  G.new_edge(and_node, inv_node);
  G.new_edge(or_node, and_node);
  G.new_edge(input_nodes.at(0), or_node);
  G.new_edge(input_nodes.at(1), or_node);
  G.new_edge(input_nodes.at(2), and_node);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }

  G.del_node(cell_node);
}

void decompose_aoi22(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node or_node = G.new_node();
  cell_types[or_node] = OR2;
  node and_node1 = G.new_node();
  cell_types[and_node1] = AND2;
  node and_node2 = G.new_node();
  cell_types[and_node2] = AND2;

  G.new_edge(or_node, inv_node);
  G.new_edge(and_node1, or_node);
  G.new_edge(and_node2, or_node);
  G.new_edge(input_nodes.at(0), and_node1);
  G.new_edge(input_nodes.at(1), and_node1);
  G.new_edge(input_nodes.at(2), and_node2);
  G.new_edge(input_nodes.at(3), and_node2);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }

  G.del_node(cell_node);
}

void decompose_oai21(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node and_node = G.new_node();
  cell_types[and_node] = AND2;
  node or_node = G.new_node();
  cell_types[or_node] = OR2;

  G.new_edge(or_node, inv_node);
  G.new_edge(and_node, or_node);
  G.new_edge(input_nodes.at(0), and_node);
  G.new_edge(input_nodes.at(1), and_node);
  G.new_edge(input_nodes.at(2), or_node);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }

  G.del_node(cell_node);
}

void decompose_oai22(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node inv_node = G.new_node();
  cell_types[inv_node] = INV;
  node and_node = G.new_node();
  cell_types[and_node] = AND2;
  node or_node1 = G.new_node();
  cell_types[or_node1] = OR2;
  node or_node2 = G.new_node();
  cell_types[or_node2] = OR2;

  G.new_edge(and_node, inv_node);
  G.new_edge(or_node1, and_node);
  G.new_edge(or_node2, and_node);
  G.new_edge(input_nodes.at(0), or_node1);
  G.new_edge(input_nodes.at(1), or_node1);
  G.new_edge(input_nodes.at(2), or_node2);
  G.new_edge(input_nodes.at(3), or_node2);
  for (auto output_node : output_nodes) {
    G.new_edge(inv_node, output_node);
  }

  G.del_node(cell_node);
}

void decompose_xor2(node cell_node, graph & G, std::vector<node> & input_nodes, std::vector<node> & output_nodes, node_map<cell_type> & cell_types) {
  node or_node = G.new_node();
  cell_types[or_node] = OR2;
  node and_node1 = G.new_node();
  cell_types[and_node1] = AND2;
  node and_node2 = G.new_node();
  cell_types[and_node2] = AND2;
  node inv_node1 = G.new_node();
  cell_types[inv_node1] = INV;
  node inv_node2 = G.new_node();
  cell_types[inv_node2] = INV;

  G.new_edge(and_node1, or_node);
  G.new_edge(and_node2, or_node);
  G.new_edge(inv_node1, and_node1);
  G.new_edge(inv_node2, and_node2);
  G.new_edge(input_nodes.at(0), and_node1);
  G.new_edge(input_nodes.at(0), inv_node2);
  G.new_edge(input_nodes.at(1), and_node2);
  G.new_edge(input_nodes.at(1), inv_node1);

  for (auto output_node : output_nodes) {
    G.new_edge(or_node, output_node);
  }

  G.del_node(cell_node);
}

void decompose_graph(graph & G, node_map<cell_type> & cell_types) {
  std::vector<node> nodes;
  nodes.reserve(G.number_of_nodes());
  for (auto node_it = G.nodes_begin(); node_it != G.nodes_end(); ++node_it) {
    nodes.push_back(*node_it);
  }

  for (auto current_node : nodes) {
    std::vector<node> output_nodes;
    get_output_nodes(current_node, output_nodes);
    std::vector<node> input_nodes;
    get_input_nodes(current_node, input_nodes);
    switch(cell_types[current_node]) {
      case INV:
      break;

      case INPUT:
      break;

      case OUTPUT:
      break;

      case AND2:
      break;

      case OR2:
      break;

      case NAND2:
      decompose_nand2(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case NAND3:
      decompose_nand3(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case NAND4:
      decompose_nand4(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case NOR2:
      decompose_nor2(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case NOR3:
      decompose_nor3(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case NOR4:
      decompose_nor4(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case AOI21:
      decompose_aoi21(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case AOI22:
      decompose_aoi22(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case OAI21:
      decompose_oai21(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case OAI22:
      decompose_oai22(current_node, G, input_nodes, output_nodes, cell_types);
      break;

      case XOR2:
      decompose_xor2(current_node, G, input_nodes, output_nodes, cell_types);
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
