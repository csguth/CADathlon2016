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
expressions = [expression1, expression2]

registers_node_id = [0, 1]

state_codes, edges = build_fsm(expressions)

assert len(state_codes) == 3
assert "00" in state_codes
assert "01" in state_codes
assert "10" in state_codes

assert len(edges) == 6
assert ("00", "00") in edges
assert ("00", "01") in edges
assert ("01", "00") in edges
assert ("01", "10") in edges
assert ("10", "10") in edges
assert ("10", "00") in edges

print "working! ;D"
