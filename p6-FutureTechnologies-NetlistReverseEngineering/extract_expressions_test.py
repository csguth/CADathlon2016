from Expression import *
from Algorithms import *
from Graph import *

netlist = Graph()

input_pin = netlist.add_node()
f0 = netlist.add_node()
f1 = netlist.add_node()
inv_f0 = netlist.add_node()
inv_f1 = netlist.add_node()
or1 = netlist.add_node()
and1 = netlist.add_node()
and2 = netlist.add_node()

netlist.add_edge(f0, inv_f0)
netlist.add_edge(f1, inv_f1)

netlist.add_edge(f0, or1)
netlist.add_edge(f1, or1)
netlist.add_edge(or1, and1)
netlist.add_edge(input_pin, and1)
netlist.add_edge(and1, f0)

netlist.add_edge(inv_f0, and2)
netlist.add_edge(inv_f1, and2)
netlist.add_edge(input_pin, and2)
netlist.add_edge(and2, f1)

registers = [f0, f1]

functions = {
    input_pin: "input",
    f0: "input",
    f1: "input",
    inv_f0: "inv",
    inv_f1: "inv",
    or1: "or",
    and1: "and",
    and2: "and"
}

expressions = extract_expressions(registers, functions)

assert "((1 | 2) & 0)" in expressions
assert "(-1 & -2 & 0)" in expressions

print "working! ;D"
