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

    evaluation test_evaluation;
    test_evaluation[1] = true;

    expression new_expression1 = expression1.evaluate(test_evaluation);
    assert(new_expression1.m_clauses.size() == 2);
    assert(new_expression1.m_clauses.at(0).m_terms.size() == 3);
    assert(new_expression1.m_clauses.at(0).m_terms.at(0) == term2);
    assert(new_expression1.m_clauses.at(0).m_terms.at(1) == term3);
    assert(new_expression1.m_clauses.at(0).m_terms.at(2) == term4);
    assert(new_expression1.m_clauses.at(1).m_terms.size() == 2);
    assert(new_expression1.m_clauses.at(1).m_terms.at(0) == term3_negated);
    assert(new_expression1.m_clauses.at(1).m_terms.at(1) == term4);

    test_evaluation[2] = true;
    expression new_expression2 = expression1.evaluate(test_evaluation);
    assert(new_expression2.m_clauses.size() == 1);
    assert(new_expression2.m_clauses.at(0).m_terms.size() == 2);
    assert(new_expression2.m_clauses.at(0).m_terms.at(0) == term3_negated);
    assert(new_expression2.m_clauses.at(0).m_terms.at(1) == term4);

    test_evaluation[3] = true;
    expression new_expression3 = expression1.evaluate(test_evaluation);
    assert(new_expression3.m_clauses.size() == 1);
    assert(new_expression3.m_clauses.at(0).m_terms.size() == 1);
    assert(new_expression3.m_clauses.at(0).m_terms.at(0) == term4);

    test_evaluation[4] = true;
    expression new_expression4 = expression1.evaluate(test_evaluation);
    assert(new_expression4.m_clauses.size() == 0);
    assert(new_expression4.m_value == true);

    std::cout << "working ;D" << std::endl;
}
