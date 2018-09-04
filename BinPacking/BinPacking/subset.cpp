#include "subset.h"

void Subset::RecalculateSize()
{
    m_subset_size = 0;
    for(int i = 0; i < m_mask_size; ++i)
        for(MaskBase x = m_mask[i]; x; x >>= 1)
            if(x & 1)
                ++m_subset_size;
}

Subset::Subset(Base *set, int size, SubsetType type = ST_Empty):
    m_set(set),
    m_set_size(size)
{
    m_mask_size = (size - 1) / MaskBaseBitLen + 1;
    m_mask = new MaskBase[m_mask_size];
    switch (type)
    {
    case ST_Empty:
        memset(m_mask, 0, m_mask_size);
        m_subset_size = 0;
        break;
    case ST_Full:
        memset(m_mask, -1, m_mask_size - 1);
        m_mask[m_mask_size - 1] = ((MaskBase)1 << ((size - 1) % MaskBaseBitLen + 1)) - 1;
        m_subset_size = size;
    }
}

Subset::Subset(const Subset &arg):
    m_set(arg.m_set),
    m_set_size(arg.m_set_size),
    m_mask_size(arg.m_mask_size),
    m_subset_size(arg.m_subset_size)
{
    m_mask = new MaskBase[m_mask_size];
    memcpy(m_mask, arg.m_mask, m_mask_size);
}

void Subset::Print() const
{
    for(int i = 0; i < m_mask_size; ++i)
    {
        MaskBase x = m_mask[i];
        for(int j = i * MaskBaseBitLen; x; x >>= 1, ++j)
            if(x & 1)
                std::cout << m_set[j] << " ";
    }

    std::cout << std::endl;
}

Subset Subset::Add(int index)
{
    if(!this->Contain(index))
    {
        m_mask[index / MaskBaseBitLen] |= (1 << (index % MaskBaseBitLen));
        ++m_subset_size;
    }

    return *this;
}

Subset Subset::Delete(int index)
{
    if(this->Contain(index))
    {
        m_mask[index / MaskBaseBitLen] ^= (1 << (index % MaskBaseBitLen));
        --m_subset_size;
    }

    return *this;
}

Subset Subset::Clear()
{
    memset(m_mask, 0, m_mask_size);
    m_subset_size = 0;

    return *this;
}

bool Subset::Contain(int index) const
{
    return ((1 << (index % MaskBaseBitLen)) & m_mask[index / MaskBaseBitLen]) != 0;
}

int Subset::Size() const
{
    return m_subset_size;
}

Base Subset::Sum() const
{
    Base sum = 0;
    for(int i = 0; i < m_mask_size; ++i)
    {
        MaskBase x = m_mask[i];
        for(int j = i * MaskBaseBitLen; x; x >>= 1, ++j)
            if(x & 1)
                sum += m_set[j];
    }
    return sum;
}

bool Subset::IsReplaceableBy(const Subset arg) const
{
    if(m_subset_size == 2 && (*this & arg).Size() > 0 && Sum() >= arg.Sum())
        return true;

    if(m_subset_size < arg.m_subset_size && Size() == (*this & arg).Size())
        return true;

    if(m_subset_size == arg.m_subset_size)
    {
        Subset tmp(*this);
        tmp ^= arg;
        int dif = 0;
        for(int i = 0; i < m_mask_size; ++i)
        {
            MaskBase x = tmp.m_mask[i];
            MaskBase y = m_mask[i];
            for(int j = i * MaskBaseBitLen; x; x >>= 1, y >>= 1, ++j)
                if(x & 1)
                {
                    if(y & 1)
                    {
                        --dif;
                        if(dif < 0) break;
                    }
                    else ++dif;
                }
            if(dif < 0) break;
        }
        if(dif == 0)
            return true;
    }

    return false;
}

Subset Subset::operator |=(const Subset arg)
{
    assert(m_set == arg.m_set && m_set_size == arg.m_set_size);

    for(int i = 0; i < m_mask_size; ++i)
        m_mask[i] |= arg.m_mask[i];

    RecalculateSize();

    return *this;
}

Subset Subset::operator &=(const Subset arg)
{
    assert(m_set == arg.m_set && m_set_size == arg.m_set_size);

    for(int i = 0; i < m_mask_size; ++i)
        m_mask[i] &= arg.m_mask[i];

    RecalculateSize();

    return *this;
}

Subset Subset::operator ^=(const Subset arg)
{
    assert(m_set == arg.m_set && m_set_size == arg.m_set_size);

    for(int i = 0; i < m_mask_size; ++i)
        m_mask[i] ^= arg.m_mask[i];

    RecalculateSize();

    return *this;
}

Subset Subset::operator =(const Subset arg)
{
    memcpy(m_mask, arg.m_mask, m_mask_size);
    m_subset_size = arg.m_subset_size;
    return *this;
}

Subset Subset::operator |(const Subset arg) const
{
    return Subset(*this) |= arg;
}

Subset Subset::operator &(const Subset arg) const
{
    return Subset(*this) &= arg;
}

Subset Subset::operator ^(const Subset arg) const
{
    return Subset(*this) ^= arg;
}

Subset Subset::operator -(const Subset arg) const
{
    return Subset(*this) ^= (arg & (*this));
}

Subset Subset::operator -() const
{
    return Subset(m_set, m_set_size, ST_Full) ^ (*this);
}

bool Subset::operator ==(const Subset arg) const
{
    for(int i = 0; i < m_set_size; ++i)
        if(m_set[i] != arg.m_set[i])
            return false;
    return true;
}
