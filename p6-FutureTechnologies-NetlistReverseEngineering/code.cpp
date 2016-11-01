#include <iostream>
#include <GTL/graph.h>
#include <queue>
#include <set>
#include <unordered_map>

using evaluation = std::unordered_map<unsigned, bool>;
using state_code = std::vector<bool>;

struct term {
  unsigned m_id;
  bool m_negated;

  term(unsigned id, bool negated)
  : m_id(id), m_negated(negated){

  }

  bool operator==(const term & other_term) const {
    return m_id == other_term.m_id && m_negated == other_term.m_negated;
  }
};

struct clause {
  std::vector<term> m_terms;
  bool m_value;

  clause(bool value)
  : m_value(value) {

  }

  clause(std::vector<term> & terms)
  : m_terms(terms){

  }

  clause evaluate(evaluation & terms_evaluation) const {
    std::vector<term> new_terms;
    new_terms.reserve(m_terms.size());
    for (auto & current_term : m_terms) {
      if (terms_evaluation.find(current_term.m_id) != terms_evaluation.end()) {
        bool term_evaluation = (current_term.m_negated) ? !terms_evaluation.at(current_term.m_id) : terms_evaluation.at(current_term.m_id);
        if (term_evaluation) {
          clause new_clause(true);
          return new_clause;
        }
      } else {
        new_terms.push_back(current_term);
      }
    }
    new_terms.shrink_to_fit();
    if (new_terms.empty()) {
      clause new_clause(false);
      return new_clause;
    } else {
      clause new_clause(new_terms);
      return new_clause;
    }
  }
};

struct expression {
  std::vector<clause> m_clauses;
  bool m_value;

  expression(bool value)
  : m_value(value) {

  }

  expression(std::vector<clause> & clauses)
  : m_clauses(clauses){

  }

  expression evaluate(evaluation & variables_evaluation) const {
    std::vector<clause> new_clauses;
    new_clauses.reserve(m_clauses.size());
    for (auto & current_clause : m_clauses) {
      clause new_clause = current_clause.evaluate(variables_evaluation);
      if (new_clause.m_terms.empty()) {
        if (!new_clause.m_value) {
          expression new_expression(false);
          return new_expression;
        }
      } else {
        new_clauses.push_back(new_clause);
      }
    }
    new_clauses.shrink_to_fit();
    if (new_clauses.empty()) {
      expression new_expression(true);
      return new_expression;
    } else {
      expression new_expression(new_clauses);
      return new_expression;
    }
  }
};

void state_code_to_evaluation(const state_code & current_state_code, evaluation & state_evaluation) {
  for (unsigned register_index = 0; register_index < current_state_code.size(); ++register_index) {
    state_evaluation[register_index] = current_state_code.at(register_index);
  }
}

void find_possible_values(const expression & current_expression, std::set<bool> & possible_values) {
  if (possible_values.size() == 2) {
    return;
  }
  if (current_expression.m_clauses.empty()) {
    possible_values.insert(current_expression.m_value);
  } else {
    evaluation new_evaluation;
    new_evaluation[current_expression.m_clauses.front().m_terms.front().m_id] = true;

    expression new_expression1 = current_expression.evaluate(new_evaluation);
    find_possible_values(new_expression1, possible_values);

    new_evaluation[current_expression.m_clauses.front().m_terms.front().m_id] = false;
    expression new_expression2 = current_expression.evaluate(new_evaluation);
    find_possible_values(new_expression2, possible_values);
  }
}

void generate_states(const std::vector<expression> & expressions, std::vector<state_code> & new_states) {
  for (auto & current_expression : expressions) {
    std::set<bool> possible_values;
    find_possible_values(current_expression, possible_values);
    for (auto possible_state : new_states) {
      std::vector<state_code> next_states;
      next_states.reserve(new_states.size());
      for (auto possible_value : possible_values) {
        possible_state.push_back(possible_value);
        next_states.push_back(possible_state);
      }
      new_states = next_states;
    }
  }
}

void build_fsm(const std::vector<expression> & expressions, graph & fsm) {
  node_map<state_code> state_codes;

  unsigned current_node_index = 0;
  auto reset_state = fsm.new_node();

  state_code reset_state_code = state_code(expressions.size(), 0);
  state_codes[reset_state] = reset_state_code;

  std::set<state_code> visited_states;
  visited_states.insert(reset_state_code);

  std::queue<node> nodes;
  nodes.push(reset_state);
  while (!nodes.empty()) {
    auto current_node = nodes.front();
    nodes.pop();

    std::vector<expression> new_expressions;
    new_expressions.reserve(expressions.size());
    for (auto & current_expression : expressions) {
      evaluation state_evaluation;
      state_code_to_evaluation(state_codes[current_node], state_evaluation);
      new_expressions.push_back(current_expression.evaluate(state_evaluation));
    }

    std::vector<state_code> new_states;
    generate_states(new_expressions, new_states);

    for (auto & state : new_states) {
      if (visited_states.find(state) != visited_states.end()) {
        auto new_node = fsm.new_node();
        state_codes[new_node] = state;
        visited_states.insert(state);
        fsm.new_edge(current_node, new_node);
      }
    }
  }
}
