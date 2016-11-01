#include "code.cpp"

int main() {
    term term1(1, false);
    term term1_negated(1, true);
    term term2(2, false);
    term term2_negated(2, true);
    term term3(3, false);
    term term3_negated(3, true);
    term term4(4, false);
    term term4_negated(4, true);

    std::vector<term> clause1_terms{term1, term2_negated, term4};
    clause clause1(clause1_terms);
    std::vector<term> clause2_terms{term1_negated, term2, term3, term4};
    clause clause2(clause2_terms);
    std::vector<term> clause3_terms{term3_negated, term4};
    clause clause3(clause3_terms);

    std::vector<clause> expression_clauses{clause1, clause2, clause3};
    expression expression1(expression_clauses);

    std::set<bool> possible_values;
    find_possible_values(expression1, possible_values);

    assert(possible_values.size() == 2);

    std::cout << "working ;D" << std::endl;
}
