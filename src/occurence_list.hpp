#include "clause.hpp"

#include <vector>

struct ClauseRef
{
    Clause *clause;
    bool deleted;
};

// A custom std::vector<ClauseRef>::iterator to deal with reallocations
class OccurenceIterator
{
public:
    OccurenceIterator(std::vector<ClauseRef> &vec, Lit lit, int index);

    Clause &operator*();
    Clause *operator->();
    OccurenceIterator &operator++();

    bool operator!=(const OccurenceIterator &other) const;

    friend class OccurenceList;

private:
    std::vector<ClauseRef> &m_vec;
    Lit m_lit;
    int m_index;
};

class OccurenceList
{
public:
    void init(int n_variables);
    void insert(Lit lit, Clause *clause);
    void erase(OccurenceIterator it);

    OccurenceIterator begin(Lit lit);
    OccurenceIterator end(Lit lit);

private:
    std::vector<std::vector<ClauseRef>> m_list;
    std::vector<bool> m_dirty;

    void clean(Lit lit);
};