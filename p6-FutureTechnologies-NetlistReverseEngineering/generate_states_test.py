from Expression import *
from Algorithms import *

f0 = Term(0, False)
not_f0 = Term(0, True)
f1 = Term(1, False)
not_f1 = Term(1, True)
a = Term(2, False)

clause1 = Clause([f0, f1])
clause2 = Clause([a])
clause3 = Clause([not_f0])
clause4 = Clause([not_f1])

expression1 = Expression([clause1, clause2])
expression2 = Expression([clause2, clause3, clause4])

reset_state_evaluation = {0: False, 1: False}

new_expressions = []
new_expressions.append(expression1.evaluate(reset_state_evaluation))
new_expressions.append(expression2.evaluate(reset_state_evaluation))

new_states = generate_states(new_expressions)

assert len(new_states) == 2
assert "00" in new_states
assert "01" in new_states

print "working! ;D"
