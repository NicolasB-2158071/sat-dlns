#include "occurence_list.hpp"

OccurenceIterator::OccurenceIterator(std::vector<ClauseRef> &vec, Lit lit, int index) : m_vec{vec}, m_lit{lit}, m_index{index} {}

Clause &OccurenceIterator::operator*()
{
    return *(m_vec[m_index].clause);
}

Clause *OccurenceIterator::operator->()
{
    return m_vec[m_index].clause;
}

OccurenceIterator &OccurenceIterator::operator++()
{
    ++m_index;
    return *this;
}

bool OccurenceIterator::operator!=(const OccurenceIterator &other) const
{
    return m_index != other.m_index;
}

void OccurenceList::init(int n_variables)
{
    m_list = std::vector<std::vector<ClauseRef>>(2 * n_variables);
    m_dirty = std::vector<bool>(2 * n_variables, false);
}

void OccurenceList::insert(Lit lit, Clause *clause)
{
    std::vector<ClauseRef> &list{m_list[2 * (lit.index() - 1) + lit.sign()]};
    list.push_back(ClauseRef{clause, false});
}

void OccurenceList::erase(OccurenceIterator it)
{
    m_dirty[2 * (it.m_lit.index() - 1) + it.m_lit.sign()] = true;
    it.m_vec[it.m_index].deleted = true;
}

OccurenceIterator OccurenceList::begin(Lit lit)
{
    if (m_dirty[2 * (lit.index() - 1) + lit.sign()])
    {
        clean(lit);
    }
    return OccurenceIterator{m_list[2 * (lit.index() - 1) + lit.sign()], lit, 0};
}

OccurenceIterator OccurenceList::end(Lit lit)
{
    int size{int(m_list[2 * (lit.index() - 1) + lit.sign()].size())};
    return OccurenceIterator{m_list[2 * (lit.index() - 1) + lit.sign()], lit, size};
}

void OccurenceList::clean(Lit lit)
{
    std::vector<ClauseRef> &list{m_list[2 * (lit.index() - 1) + lit.sign()]};
    std::vector<ClauseRef>::iterator i, j;
    for (i = j = list.begin(); i != list.end(); ++i)
    {
        if (!i->deleted)
        {
            *(j++) = *i;
        }
    }

    list.erase(j, i);
    m_dirty[2 * (lit.index() - 1) + lit.sign()] = false;
}