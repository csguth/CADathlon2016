class Node:
    id = -1

    def __init__(self, id):
        self.id = id

    def __hash__(self):
        return hash(self.id)

    def __eq__(self, other_node):
        return self.id == other_node.id

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
        return edge
