#include "problemsolver.h"

// Добавление подмножества в решение
void ProblemSolver::Add(const Subset subset)
{
    m_unsolved ^= subset;
    m_result.push_back(subset);
}

// Быстрая сортировка по убыванию
void ProblemSolver::QuickSort(Base *set, int left_border, int right_border)
{
    if(left_border >= right_border)
        return;
    int i = left_border + 1;
    int j = right_border;
    Base median = set[left_border];
    while(i <= j)
    {
        while(i <= j && set[i] >= median)
            i++;
        while(i <= j && set[j] < median)
            j--;
        if(i < j)
        {
            set[i] ^= set[j];
            set[j] ^= set[i];
            set[i] ^= set[j];
            i++;
            j--;
        }
    }
    if(left_border != j)
    {
        set[left_border] ^= set[j];
        set[j] ^= set[left_border];
        set[left_border] ^= set[j];
    }
    --j;
    QuickSort(set, left_border, j);
    QuickSort(set, i, right_border);
}

// Нахождение и добавление в решение таких подмножеств, которые можно сразу добавить в решение
void ProblemSolver::IncludeProvenSubsets1()
{
    int i = 0;
    int j = m_set_size - 1;
    while (i < j)
    {
        while(i < j && !m_unsolved.Contain(i)) ++i;
        while(i < j && !m_unsolved.Contain(j)) --j;

        if(i == j) break;

        if(m_set[i] + m_set[j] == m_limit)
            Add(Subset(m_set, m_set_size, ST_Empty).Add(i).Add(j));
        else if(m_set[i] + m_set[j] < m_limit)
            --j;
        else
            ++i;
    }
}

// То же что и выше, но другим способом (находит другие подмножества)
void ProblemSolver::IncludeProvenSubsets2()
{
    int i = 0;
    int j = m_set_size - 1;
    Base sum = 0;
    Subset subset(m_set, m_set_size, ST_Empty);
    while(i < j)
    {
        while(i < j && (m_set[j] <= sum || !m_unsolved.Contain(j)))
        {
            sum += m_set[j];
            subset.Add(j--);
        }

        while(i < j && (m_set[i] + sum > m_limit || !m_unsolved.Contain(i)))
            ++i;

        if(i == j) break;

        if(m_set[i] + m_set[j] > m_limit)
        {
            subset.Add(i);
            Add(subset);
            subset.Clear();
            sum = 0;
        }
        else
        {
            sum += m_set[j];
            subset.Add(j--);
        }
    }
}

// Получение эмпирического решения, первый способ
// Позволяет получить оценку сверху для количества подмножеств в решении
std::list<Subset> ProblemSolver::GetUpperBound1()
{
    Subset tmp_unsolved(m_unsolved);
    std::list<Subset> result;

    while(tmp_unsolved.Size() > 0)
    {
        Subset subset(m_set, m_set_size, ST_Empty);
        Base sum = 0;

        for(int i = 0; i < m_set_size; ++i)
            if(tmp_unsolved.Contain(i) && sum + m_set[i] <= m_limit)
            {
                subset.Add(i);
                sum += m_set[i];
            }

        result.push_back(subset);
        tmp_unsolved ^= subset;
    }

    return result;
}

// Получение эмпирического решения, второй способ
std::list<Subset> ProblemSolver::GetUpperBound2()
{
    Subset tmp_unsolved(m_unsolved);
    std::list<Subset> result;
    int i = 0;
    int j = m_set_size - 1;

    while(tmp_unsolved.Size() > 0)
    {
        Subset subset(m_set, m_set_size, ST_Empty);
        Base sum = 0;

        while(i < j && (m_set[i] + sum <= m_limit || !tmp_unsolved.Contain(i)))
        {
            if(tmp_unsolved.Contain(i))
            {
                subset.Add(i);
                sum += m_set[i];
            }
            ++i;
        }

        while(i <= j && m_set[j] + sum <= m_limit)
        {
            if(tmp_unsolved.Contain(j))
            {
                subset.Add(j);
                sum += m_set[j];
            }
            --j;
        }

        result.push_back(subset);
        tmp_unsolved ^= subset;
    }

    return result;
}

// Нахождение нижней границы (для количества подмножеств в решении)
void ProblemSolver::GetLowerBound()
{
    m_lower_bound = (m_unsolved.Sum() - 1) / m_limit + 1;
}

// Перебор возможных решений
// depth - глубина дерева обхода, а так же и количество подмножеств, вошедших в частичное решение
// unsolved - подмножество элементов, не вошедших в частичное решение
// found - флаг, по который проверяется при возврате из функции: если true - было найдено решение, не превышающее найденой ранее верхней границы
std::list<Subset> ProblemSolver::BruteForce(int depth, Subset unsolved, bool &found)
{
    std::list<Subset> result;

    // При достижении состояния, когда до верхней границы остается всего 1 место,
    // Собираем все оставшиеся элементы в одно подмножество и возвращаем его
    // Проверяем "влезает" ли это множество в контейнер и возвращаем соответствующий флаг
    if(depth + 1 == m_upper_bound)
    {
        found = (unsolved.Sum() <= m_limit);
        result.push_back(unsolved);
        return result;
    }

    // Генерируем "первое" подмножество в переборе
    // sum - сумма элементов подмножества
    // last_inc - индекс последнего элемента в подмножестве (нужен для перебора)
    Subset var(m_set, m_set_size, ST_Empty);
    Base sum = 0;
    int last_inc = 0;

    for(int i = 0; i < m_set_size; ++i)
        if(unsolved.Contain(i) && ((sum + m_set[i]) <= m_limit))
        {
            var.Add(i);
            sum += m_set[i];
            last_inc = i;
        }

    // Если в него попали все оставшиеся элементы, то возращаем его как решение
    // При этом опускаем верхнюю границу
    if(var.Size() == unsolved.Size())
    {
        m_upper_bound = depth + 1;
        result.push_back(var);
        found = true;
        return result;
    }

    // Создаем список всех вариантов
    std::list<Subset> variants;
    variants.push_back(var);

    // Перебираем все возможные подмножества
    while(var.Size() > 1)
    {
        // Удаляем последний элемент из подмножества и корректируем сумму
        var.Delete(last_inc);
        sum -= m_set[last_inc];

        // Проверяем был ли этот элемент последним
        int t;

        for(t = last_inc + 1; t < m_set_size && !unsolved.Contain(t); ++t);

        // И если был, то удаляем все элементы, стоявшие перед ним, которые входят во множество
        // Пересчитываем last_inc заного и продолжаем перебор
        if(t == m_set_size)
        {
            --last_inc;
            while(var.Size() > 1 && (var.Contain(last_inc) || !unsolved.Contain(last_inc)))
            {
                if(var.Contain(last_inc))
                {
                    var.Delete(last_inc);
                    sum -= m_set[last_inc];
                }
                --last_inc;
            }
            while(!unsolved.Contain(last_inc) || !var.Contain(last_inc)) --last_inc;
            continue;
        }

        // Добавляем элементы в подмножество
        for(int i = last_inc + 1; i < m_set_size; ++i)
            if(unsolved.Contain(i) && sum + m_set[i] <= m_limit)
            {
                var.Add(i);
                sum += m_set[i];
                last_inc = i;
            }

        // И проверяем не "перекрывается" ли оно уже полученным подмножеством
        std::list<Subset>::iterator iter = variants.begin();
        while(iter != variants.end() && !var.IsReplaceableBy(*iter)) ++iter;

        // Если не перекрывается, то добавляем его в список вариантов
        if(iter == variants.end())
            variants.push_back(var);
    }

    // m_total_vaiants - переменная, равная количеству листьев в "дереве перебора"
    // Пока что используется для нахождения "интересных случаев"
    // !!! В ФИНАЛЬНОЙ ВЕРСИИ НУЖНО БУДЕТ УДАЛИТЬ !!!
    if(variants.size() > 1)
        m_total_variants += variants.size() - 1;

    // Изначально полагаем, что не нашли решения
    found = false;

    // Пробуем решить задачу дальше для каждого полученного подмножества
    for(auto variant: variants)
    {
        unsolved ^= variant;
        bool flag;
        std::list<Subset> tmpres = BruteForce(depth + 1, unsolved, flag);
        unsolved ^= variant;
        if(!flag) continue; // Если решение с текущим подмножеством не было найдено, то проверяем остальные подмножества
        if(!found) // Если решение было найдено впервые, то фиксируем его как основное решение
        {
            found = true;
            result = tmpres;
            result.push_front(variant);
        }
        else if(tmpres.size() + 1 < result.size()) // Если получилось решение меньше основонго, то основное заменяем на полученное
        {
            result = tmpres;
            result.push_front(variant);
        }
        if(m_upper_bound == m_lower_bound) // В ходе решения верхняя граница может изменится, здесь это быстрее всего обгаружить и при необходимости остановить перебор (если нашли решение, удовлетворяющее нижней границе)
            return result;
    }
    return result; // Отправляем основное решение
}

// Конструктор "решателя" задачи
ProblemSolver::ProblemSolver(Base *set, int size, Base limit):
    m_set(set),
    m_set_size(size),
    m_result(),
    m_unsolved(set, size, ST_Full),
    m_limit(limit),
    m_total_variants(1)
{
    QuickSort(set, 0, size - 1);
}

// Полное решение
std::list<Subset> ProblemSolver::Solve()
{
    // Сразу добавляем те подмножества в решение, которые точно можем
    IncludeProvenSubsets1();
    IncludeProvenSubsets2();

    // Находим нижнюю границу
    GetLowerBound();

    // Находим верхнюю границу
    std::list<Subset> upper_bound_1 = GetUpperBound1();
    std::list<Subset> upper_bound_2 = GetUpperBound2();

    m_upper_bound = std::min(upper_bound_1.size(), upper_bound_2.size());

    // И если они совпадают, то возращаем эмпирическое решение
    if(m_upper_bound == m_lower_bound)
    {
        if(m_upper_bound == upper_bound_1.size())
            m_result.splice(m_result.end(), upper_bound_1);
        else
            m_result.splice(m_result.end(), upper_bound_2);

        return m_result;
    }

    // Иначе перебираем все варианты
    bool flag;
    std::list<Subset> result = BruteForce(0, m_unsolved, flag);
    m_result.splice(m_result.end(), result);

    // И возращаем наилучший из полученных
    return m_result;
}
