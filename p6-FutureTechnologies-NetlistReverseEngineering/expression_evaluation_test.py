from Expression import *

x1 = Term(1, False)
not_x1 = Term(1, True)
x2 = Term(2, False)
not_x2 = Term(2, True)
x3 = Term(3, False)
not_x3 = Term(3, True)
x4 = Term(4, False)
not_x4 = Term(4, True)

clause1 = Clause([x1, not_x2, x4])
clause2 = Clause([not_x1, x2, x3, x4])
clause3 = Clause([not_x3, x4])

expression = Expression([clause1, clause2, clause3])

test_evaluation = {}
test_evaluation[1] = True

new_expression = expression.evaluate(test_evaluation)

assert len(new_expression.clauses) == 2
assert len(new_expression.clauses[0].terms) == 3
assert new_expression.clauses[0].terms[0] == x2
assert new_expression.clauses[0].terms[1] == x3
assert new_expression.clauses[0].terms[2] == x4
assert len(new_expression.clauses[1].terms) == 2
assert new_expression.clauses[1].terms[0] == not_x3
assert new_expression.clauses[1].terms[1] == x4

test_evaluation[2] = True

new_expression = expression.evaluate(test_evaluation)

assert len(new_expression.clauses) == 1
assert len(new_expression.clauses[0].terms) == 2
assert new_expression.clauses[0].terms[0] == not_x3
assert new_expression.clauses[0].terms[1] == x4

test_evaluation[3] = True

new_expression = expression.evaluate(test_evaluation)

assert len(new_expression.clauses) == 1
assert len(new_expression.clauses[0].terms) == 1
assert new_expression.clauses[0].terms[0] == x4

test_evaluation[4] = True

new_expression = expression.evaluate(test_evaluation)

assert len(new_expression.clauses) == 0
assert new_expression.value == True

print "working! ;D"
