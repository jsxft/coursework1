#ifndef SUBSET_H
#define SUBSET_H

#include <vector>
#include <cstring>
#include <iostream>
#include <assert.h>

typedef unsigned int Base;
typedef unsigned char MaskBase;
#define MaskBaseLen (sizeof(MaskBase))
#define MaskBaseBitLen (sizeof(MaskBase)*8)

enum SubsetType
{
    ST_Empty,
    ST_Full
};

class Subset
{
private:
    Base* m_set;
    int m_set_size;
    MaskBase* m_mask;
    int m_mask_size;
    int m_subset_size;

    void RecalculateSize();

public:
    Subset(Base* set, int size, SubsetType type);
    Subset(const Subset & arg);
    ~Subset() { delete m_mask; }

    void Print() const;
    Subset Add(int index);
    Subset Delete(int index);
    Subset Clear();
    bool Contain(int index) const;
    int Size() const;
    Base Sum() const;
    bool IsReplaceableBy(const Subset arg) const;

    Subset operator |=(const Subset arg);
    Subset operator &=(const Subset arg);
    Subset operator ^=(const Subset arg);
    Subset operator =(const Subset arg);

    Subset operator |(const Subset arg) const;
    Subset operator &(const Subset arg) const;
    Subset operator ^(const Subset arg) const;
    Subset operator -(const Subset arg) const;

    Subset operator -() const;
    bool operator ==(const Subset arg) const;
};

#endif // SUBSET_H
