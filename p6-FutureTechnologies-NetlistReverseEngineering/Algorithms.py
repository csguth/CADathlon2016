from sets import Set
from Queue import Queue
from Graph import *

def find_possible_values(expression, possible_values):
    if (len(possible_values) == 2):
        return possible_values

    if (len(expression.clauses) == 0):
        possible_values.add(expression.value)
    else:
        new_evaluation = {}
        new_evaluation[expression.clauses[0].terms[0].id] = True

        new_expression1 = expression.evaluate(new_evaluation)
        possible_values = find_possible_values(new_expression1, possible_values)

        new_evaluation[expression.clauses[0].terms[0].id] = False

        new_expression2 = expression.evaluate(new_evaluation)
        possible_values = find_possible_values(new_expression2, possible_values)

    return possible_values

def generate_states(expressions):
    new_states = []
    for expression in expressions:
        possible_values = find_possible_values(expression, Set())
        if (len(new_states) == 0):
            for value in possible_values:
                value_string = "1" if value else "0"
                new_states.append(value_string)
        else:
            next_states = []
            for value in possible_values:
                for possible_state in new_states:
                    value_string = "1" if value else "0"
                    next_state = possible_state + value_string
                    next_states.append(next_state)
            new_states = next_states
    return new_states

def build_fsm(expressions):
    state_codes = Set()

    reset_state_code = "0" * len(expressions)
    state_codes.add(reset_state_code)

    states_to_visit = Queue()
    states_to_visit.put(reset_state_code)

    edges = Set()

    while (not states_to_visit.empty()):
        current_state = states_to_visit.get()

        new_expressions = []
        for expression in expressions:
            state_evaluation = {}
            for index, value in enumerate(current_state):
                state_evaluation[index] = True if value == "1" else False
            new_expressions.append(expression.evaluate(state_evaluation))

        new_states = generate_states(new_expressions)

        for state in new_states:
            edges.add((current_state, state))
            if (state not in state_codes):
                state_codes.add(state)
                states_to_visit.put(state)

    return state_codes, edges

def extract_expression(node, functions):
    if (functions[node] == "input"):
        return str(node.id)
    elif (functions[node] == "inv"):
        return "-" + extract_expression(node.input_nodes[0], functions)
    else:
        operator = " | " if functions[node] == "or" else " & "
        expression = "("
        for input_node in node.input_nodes[:-1]:
            expression += extract_expression(input_node, functions) + operator
        expression += extract_expression(node.input_nodes[-1], functions) + ")"
        return expression

def extract_expressions(registers, functions):
    expressions = []
    for register in registers:
        expression = extract_expression(register.input_nodes[0], functions)
        expressions.append(expression)
    return expressions
