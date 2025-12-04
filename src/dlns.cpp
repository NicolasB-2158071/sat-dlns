#include "dlns.hpp"
#include "random.hpp"

DLNS::DLNS() : m_trail_level{}, m_conflict{}, m_output{OUTPUT::UNKNOWN} {};

void DLNS::init(int n_variables, int n_clauses)
{
    m_clauses.reserve(n_clauses);
    m_assigns = std::vector<Lbool>(n_variables, l_undef);
    m_occ.init(n_variables);
}

void DLNS::solve(const std::vector<Lit> &assumptions)
{
    for (std::vector<Lit>::const_iterator it = assumptions.begin(); it != assumptions.end(); ++it)
    {
        assign(*it, true);
    }

    search();
}

void DLNS::search()
{
    while (m_output == OUTPUT::UNKNOWN)
    {
        propagate();
        if (m_conflict)
        {
            if (m_decision_variables.size() == 0)
            {
                m_output = OUTPUT::UNSATISFIABLE;
            }
            else
            {
                int last_decision_variable{*m_decision_variables.rbegin()};
                bool last_decision_assignment{m_assigns[last_decision_variable - 1] == l_true};

                undo();
                assign(Lit{last_decision_variable, last_decision_assignment});
                m_conflict = false;
            }
        }
        else
        {
            if (n_assigns() == int(m_assigns.size()))
            {
                m_output = OUTPUT::SATISFIABLE;
            }
            else
            {
                assign(Lit{get_random_free_variable(), false}, true);
            }
        }
    }
}

void DLNS::propagate()
{
    while (m_trail_level < int(m_trail.size()))
    {
        Lit lit{m_trail[m_trail_level].neg()};
        for (OccurenceIterator c = m_occ.begin(lit); c != m_occ.end(lit); ++c)
        {
            if (c->lits[0] == lit)
            {
                std::swap(c->lits[0], c->lits[1]);
            }

            // First watcher is already satisfied
            if (value(c->lits[0]) == l_true)
            {
                goto end;
            }

            // Find new literal
            for (std::vector<Lit>::iterator l = c->lits.begin() + 2; l != c->lits.end(); ++l)
            {
                if (value(*l) != l_false)
                {
                    std::swap(c->lits[1], *l);
                    m_occ.erase(c);
                    m_occ.insert(c->lits[1], &(*c));
                    goto end;
                }
            }

            // Unit clause: lits[0] is unassigned or unsatisfied
            if (value(c->lits[0]) == l_false)
            {
                raise_conflict();
                return;
            }
            assign(c->lits[0]);
        end:;
        }
        ++m_trail_level;
    }
}

void DLNS::add_clause(Clause &&clause)
{
    if (clause.lits.size() == 1)
    {
        assign(clause.lits[0]);
        return;
    }

    m_clauses.push_back(clause);
    m_occ.insert(m_clauses.rbegin()->lits[0], &(*m_clauses.rbegin()));
    m_occ.insert(m_clauses.rbegin()->lits[1], &(*m_clauses.rbegin()));
}

void DLNS::assign(Lit lit, bool decision)
{
    if (decision)
    {
        m_decision_variables.push_back(lit.index());
    }
    m_assigns[lit.index() - 1] = Lbool{!lit.sign()};
    m_trail.push_back(std::move(lit));
}

int DLNS::n_assigns()
{
    int defined{};
    for (std::vector<Lbool>::iterator it = m_assigns.begin(); it != m_assigns.end(); ++it)
    {
        defined += *it == l_undef ? 0 : 1;
    }
    return defined;
}

Lbool DLNS::value(Lit lit) const
{
    return m_assigns[lit.index() - 1] ^ lit.sign();
}

void DLNS::undo()
{
    std::reverse_iterator<std::vector<Lit>::iterator> it;
    for (it = m_trail.rbegin(); it != m_trail.rend(); ++it)
    {
        m_assigns[it->index() - 1] = l_undef;
        if (it->index() == *m_decision_variables.rbegin())
        {
            break;
        }
    }

    m_trail.erase(it.base() - 1, m_trail.end());
    m_trail_level = int(m_trail.size());
    m_decision_variables.pop_back();
}

void DLNS::raise_conflict()
{
    m_conflict = true;
}

int DLNS::get_random_free_variable() const
{
    int random_variable{Random::get_uniform(1, m_assigns.size())};
    while (m_assigns[random_variable - 1] != l_undef)
    {
        random_variable = Random::get_uniform(1, m_assigns.size());
    }
    return random_variable;
}

std::string DLNS::output() const
{
    std::string model{"v "};
    int var{1};
    for (std::vector<Lbool>::const_iterator it = m_assigns.begin(); it != m_assigns.end(); ++it)
    {
        model.append(((*it) == l_true ? "" : "-") + std::to_string(var++) + " ");
    }
    model.pop_back();

    switch (m_output)
    {
    case OUTPUT::SATISFIABLE:
        return "s SATISFIABLE \n" + model;
    case OUTPUT::UNSATISFIABLE:
        return "s UNSATISFIABLE";
    case OUTPUT::UNKNOWN:
        return "s UNKNOWN";
    }

    return "";
}