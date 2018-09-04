#include <iostream>
#include "problemsolver.h"
#include "generators.h"

int main(int argc, char *argv[])
{
    /*
    Base MaxValue = 1000;
    int size = 100;
    Base *x = GenerateSet1(MaxValue, size);

    for(int i = 0; i < size; ++i)
        std::cout << x[i] << " ";
    std::cout << std::endl << std::endl;

    ProblemSolver problemSolver(x, size, MaxValue);
    auto solution = problemSolver.Solve();

    for(auto subset: solution)
        subset.Print();

    */
    Base M = 100;
    int N = 30;
    int C = 500;

    srand(time(NULL));

    for(int i = 0; i < C; ++i)
    {
        int n = rand() % N;
        Base *x = GenerateSet2(M, n);
        ProblemSolver ps(x, n, M);
        ps.Solve();

        if(ps.TotalVariants() > 1)
        {
            std::cout << ps.TotalVariants() << " " << n << ": ";
            for(int i = 0; i < n; ++i)
                std::cout << x[i] << ", ";
            std::cout << std::endl;
        }

        delete [] x;
    }

    return 0;
}
