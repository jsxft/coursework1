#include "generators.h"

Base* GenerateSet1(Base MaxValue, int count)
{
    Base *result = new Base[count];

    for(int i = 0; i < count; ++i)
        result[i] = rand() % MaxValue + 1;

    return result;
}

Base *GenerateSet2(Base MaxValue, int count)
{
    Base *result = new Base[count];

    int i = 0;

    while(i < count / 4)
        result[i++] = rand() % MaxValue + 1;

    while(i < count / 2)
        result[i++] = rand() % (MaxValue / 2) + 1;

    while(i < count * 3 / 4)
        result[i++] = rand() % (MaxValue / 3) + 1;

    while(i < count)
        result[i++] = rand() % (MaxValue / 4) + 1;

    return result;
}
