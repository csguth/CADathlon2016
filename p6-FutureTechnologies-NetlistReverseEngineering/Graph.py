class Node:
    id = -1
    output_nodes = []
    input_nodes = []

    def __init__(self, id):
        self.id = id
        self.output_nodes = []
        self.input_nodes = []

    def __hash__(self):
        return hash(self.id)

    def __eq__(self, other_node):
        return self.id == other_node.id

    def __repr__(self):
        return repr(self.id)

class Edge:
    source = None
    target = None

    def __init__(self, source, target):
        self.source = source
        self.target = target

class Graph:
    last_node_id = 0
    nodes = []
    edges = []

    def add_node(self):
        node = Node(self.last_node_id)
        self.nodes.append(node)
        self.last_node_id += 1
        return node

    def add_edge(self, source, target):
        edge = Edge(source, target)
        self.edges.append(edge)
        source.output_nodes.append(target)
        target.input_nodes.append(source)
        return edge
