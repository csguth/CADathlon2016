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

assert len(state_registers) == 2
assert f0 in state_registers
assert f1 in state_registers
assert f2 not in state_registers

print "working! ;D"
