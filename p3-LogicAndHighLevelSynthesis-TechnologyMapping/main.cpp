#include <iostream>
#include <GTL/graph.h>

enum logic_function{
    INV, AND, OR, INPUT
};

//verify if the two graph is isomorfic and each node are same logic function
bool match(const node &u, const node &v, const node_map<logic_function> &map_u, const node_map<logic_function> &map_v){
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

int main() {
    std::cout << "\n RUN p3-LogicAndHighLevelSynthesis-TechnologyMapping \n" << std::endl;

    graph G;
    node_map<logic_function> node_map_G;
    node and_G = G.new_node();
    node inv_G = G.new_node();
    node a_G = G.new_node();
    node b_G = G.new_node();
    node_map_G[and_G] = logic_function::AND;
    node_map_G[inv_G] = logic_function::INV;
    node_map_G[a_G] = logic_function::INPUT;
    node_map_G[b_G] = logic_function::INPUT;
    G.new_edge(a_G, and_G);
    G.new_edge(b_G, inv_G);
    G.new_edge(inv_G, and_G);

    graph H;
    node_map<logic_function> node_map_H;
    node and_H = H.new_node();
    node inv_H = H.new_node();
    node a_H = H.new_node();
    node b_H = H.new_node();
    node_map_H[and_H] = logic_function::AND;
    node_map_H[inv_H] = logic_function::INV;
    node_map_H[a_H] = logic_function::INPUT;
    node_map_H[b_H] = logic_function::INPUT;
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
