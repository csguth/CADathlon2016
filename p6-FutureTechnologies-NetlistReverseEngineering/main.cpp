#include <iostream>
#include <GTL/graph.h>
#include <queue>
#include <set>

using evaluation = std::vector<bool>;

struct expression {
  std::vector<string> symbols;

  bool evaluate(evaluation & variables_evaluation) {
    return true;
  }

  expression simplify(evaluation & variables_evaluation) const {
    return expression();
  }
};

void fetch(const std::vector<expression> & expressions, std::vector<evaluation> & possible_evaluations) {

}

void get_register_states(const std::vector<expression> & expressions, graph & fsm) {
  node_map<evaluation> state_codes;

  unsigned current_node_index = 0;
  auto reset_state = fsm.new_node();

  evaluation reset_state_code = evaluation(expressions.size(), 0);
  state_codes[reset_state] = reset_state_code;

  std::set<evaluation> visited_states;
  visited_states.insert(reset_state_code);

  std::queue<node> nodes;
  nodes.push(reset_state);
  while (!nodes.empty()) {
    auto current_node = nodes.front();
    nodes.pop();

    std::vector<expression> new_expressions;
    new_expressions.reserve(expressions.size());
    for (auto & current_expression : expressions) {
      new_expressions.push_back(current_expression.simplify(state_codes[current_node]));
    }

    std::vector<evaluation> possible_evaluations;
    fetch(new_expressions, possible_evaluations);

    for (auto & possible_evaluation : possible_evaluations) {
      if (visited_states.find(possible_evaluation) != visited_states.end()) {
        auto new_node = fsm.new_node();
        state_codes[new_node] = possible_evaluation;
        visited_states.insert(possible_evaluation);
        fsm.new_edge(current_node, new_node);
      }
    }
  }
}

int main() {
    std::cout << "hello world" << std::endl;

    graph G;
    G.new_node();

    return 0;
}
