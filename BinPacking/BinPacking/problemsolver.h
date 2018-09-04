#ifndef PROBLEMSOLVER_H
#define PROBLEMSOLVER_H
#include "subset.h"
#include <list>

class ProblemSolver
{
private:
    Base* m_set;
    int m_set_size;
    std::list<Subset> m_result;
    Subset m_unsolved;
    Base m_limit;
    int m_upper_bound;
    int m_lower_bound;
    int m_total_variants;

    void Add(const Subset subset);
    void QuickSort(Base* set, int left_border, int right_border);

    void IncludeProvenSubsets1();
    void IncludeProvenSubsets2();

    std::list<Subset> GetUpperBound1();
    std::list<Subset> GetUpperBound2();

    void GetLowerBound();

    std::list<Subset> BruteForce(int depth, Subset unsolved, bool &found);

public:
    ProblemSolver(Base* set, int size, Base limit);
    std::list<Subset> Solve();
    int TotalVariants() { return m_total_variants; }
};

#endif // PROBLEMSOLVER_H
