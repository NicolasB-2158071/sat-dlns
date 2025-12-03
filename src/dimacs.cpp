#include "dimacs.hpp"
#include "dlns.hpp"

#include <stdexcept>
#include <fstream>

void Dimacs::parse(const std::string &path, DLNS &solver)
{
    std::ifstream file(path);
    while (file)
    {
        std::string word{next_word(file)};
        if (word.empty())
        {
            break;
        }

        if ("c" == word)
        {
            skip_line(file);
        }
        else if ("p" == word)
        {
            if (m_problem_read)
            {
                throw std::runtime_error{"Problem already defined"};
            }
            read_problem(file, solver);
        }
        else
        {

            read_clause(file, word, solver);
        }
    }
}

std::string Dimacs::next_word(std::ifstream &file) const
{
    std::string word;
    if (!(file >> word))
    {
        return "";
    }
    return word;
}

void Dimacs::skip_line(std::ifstream &file) const
{
    std::string tmp;
    std::getline(file, tmp);
}

void Dimacs::read_problem(std::ifstream &file, DLNS &solver)
{
    if (next_word(file) != "cnf")
    {
        throw std::runtime_error{"Expected cnf after reading 'p'"};
    }

    int problem[2];
    for (int i = 0; i < 2; ++i)
    {
        problem[i] = std::stoi(next_word(file));
        if (problem[i] < 0)
        {
            throw std::runtime_error{"Negative value in problem description"};
        }
    }

    solver.init(problem[0], problem[1]);
}

void Dimacs::read_clause(std::ifstream &file, const std::string &first_word, DLNS &solver) const
{
    std::vector<Lit> lits;
    int lit;

    lit = std::stoi(first_word);
    lits.push_back(Lit{std::abs(lit), lit < 0});

    std::string word;
    while ((word = next_word(file)) != "" && (lit = std::stoi(word)) != 0) // May not end on 0
    {
        lits.push_back(Lit{std::abs(lit), lit < 0});
    }

    solver.add_clause(Clause{std::move(lits)});
}