#include "code.cpp"

//test with expressions f0 = (f0 | f1) & a and f1 = (~f0 | ~f1) & a
int main() {
    term f0(0, false);
    term not_f0(0, true);
    term f1(1, false);
    term not_f1(1, true);
    term a(2, false);

    std::vector<term> clause1_terms{f0, f1};
    clause clause1(clause1_terms);
    std::vector<term> clause2_terms{a};
    clause clause2(clause2_terms);
    std::vector<term> clause3_terms{not_f0};
    clause clause3(clause3_terms);
    std::vector<term> clause4_terms{not_f1};
    clause clause4(clause4_terms);

    std::vector<clause> expression1_clauses{clause1, clause2};
    expression expression1(expression1_clauses);

    std::vector<clause> expression2_clauses{clause2, clause3, clause4};
    expression expression2(expression2_clauses);

    state_code reset_state_code{false, false};
    evaluation reset_state_evaluation;
    state_code_to_evaluation(reset_state_code, reset_state_evaluation);
    vector<expression> new_expressions;
    new_expressions.push_back(expression1.evaluate(reset_state_evaluation));
    new_expressions.push_back(expression2.evaluate(reset_state_evaluation));

    std::vector<state_code> new_states;
    generate_states(new_expressions, new_states);

    assert(new_states.size() == 2);

    std::cout << "working ;D" << std::endl;
}
