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
and3 = netlist.add_node()
f2 = netlist.add_node()
output_pin = netlist.add_node()

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

netlist.add_edge(inv_f0, and3)
netlist.add_edge(f1, and3)

netlist.add_edge(and3, f2)

netlist.add_edge(f2, output_pin)

cell_types = {
    input_pin: "INPUT",
    f0: "DFF",
    f1: "DFF",
    f2: "DFF",
    inv_f0: "INV",
    inv_f1: "INV",
    or1: "OR2",
    and1: "AND2",
    and2: "AND3",
    and3: "AND2",
    output_pin: "OUTPUT"
}

state_registers = identify_state_registers(netlist, cell_types)

functions = {
    input_pin: "input",
    f0: "input",
    f1: "input",
    f2: "input",
    inv_f0: "inv",
    inv_f1: "inv",
    or1: "or",
    and1: "and",
    and2: "and",
    and3: "and"
}

expression_texts = extract_expressions(state_registers, functions)

expressions = [parse_expression(expression_text) for expression_text in expression_texts]

registers_node_id = [register.id for register in state_registers]

state_codes, edges = build_fsm(expressions, registers_node_id)

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
