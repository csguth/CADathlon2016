class Term:
    id = -1
    negated = False

    def __init__(self, id, negated):
        self.id = id
        self.negated = negated

    def __eq__(self, other_term):
        return (self.id == other_term.id) and (self.negated == other_term.negated)

class Clause:
    terms = []
    value = None

    def __init__(self, terms):
        self.terms = terms

    def evaluate(self, terms_evaluation):
        new_terms = []
        for current_term in self.terms:
            if (current_term.id in terms_evaluation):
                term_evaluation = not terms_evaluation[current_term.id] if current_term.negated else terms_evaluation[current_term.id]
                if (term_evaluation):
                    new_clause = Clause([])
                    new_clause.value = True
                    return new_clause
            else:
                new_terms.append(current_term)

        new_clause = Clause(new_terms)
        if (len(new_terms) == 0):
            new_clause.value = False
        return new_clause

class Expression:
    clauses = []
    value = None

    def __init__(self, clauses):
        self.clauses = clauses

    def evaluate(self, terms_evaluation):
        new_clauses = []
        for current_clause in self.clauses:
            new_clause = current_clause.evaluate(terms_evaluation)
            if (len(new_clause.terms) == 0):
                if (not new_clause.value):
                    new_expression = Expression([])
                    new_expression.value = False
                    return new_expression
            else:
                new_clauses.append(new_clause)

        new_expression = Expression(new_clauses)
        if (len(new_clauses) == 0):
            new_expression.value = True
        return new_expression
