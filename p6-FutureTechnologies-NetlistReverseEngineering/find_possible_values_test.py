from sets import Set
from Expression import *
from Algorithms import *

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

possible_values = find_possible_values(expression, Set())

assert len(possible_values) == 2
assert True in possible_values
assert False in possible_values

print "working! ;D"
