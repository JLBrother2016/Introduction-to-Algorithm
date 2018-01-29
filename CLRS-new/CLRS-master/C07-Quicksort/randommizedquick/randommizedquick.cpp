#include <bits/stdc++.h>
using namespace std;

template<typename Iter>
inline Iter partition(Iter first, Iter last)
{
    Iter pivot = last - 1;
    Iter smaller = first - 1;
    for(Iter iter = first; iter != pivot; ++iter)
    {
        if(*iter <= *pivot)       //<=递增排序
        {
            ++smaller;
            std::swap(*smaller, *iter);
        }
    }
    std::swap(*++smaller, *pivot);
    pivot = smaller;

    return pivot;
}

template<typename Iter>
inline Iter randomized_partition(Iter first, Iter last)
{
    assert(last > first);
    static std::default_random_engine e;
    static std::uniform_real_distribution<float> ud(0, 100);

    auto range = last - first;
    Iter pivot = first + ud(e) * range/100;
    std::swap(*pivot, *(last - 1));
    return partition(first, last);
}
/**
 * @brief randomized_quicksort
 *
 * @complexity      worst-cast  :   theta(n^2)
 *                  average     :   theta(n lg n)
 */
template<typename Iter>
inline void randomized_quicksort(Iter first, Iter last)
{
    if(last - first > 1)
    {
        Iter pivot = randomized_partition(first, last);
        randomized_quicksort(first, pivot);
        randomized_quicksort(pivot + 1, last);
    }
}

template<typename Iter>
inline Iter hoare_partition(Iter first, Iter last)
{
    auto pivot      = *first;
    Iter forward    = first - 1;
    Iter backward   = last;
    while(true)
    {
        while(*--backward > pivot);
        while(*++forward  < pivot);

        if(forward < backward)
            std::swap(*forward, *backward);
        else
            return backward;
    }
}

/**
 * @brief hoare_quicksort
 *
 * @complexity      worst-cast  :   theta(n^2)
 *                  average     :   theta(n lg n)
 */
template<typename Iter>
inline void hoare_quicksort(Iter first, Iter last)
{
    if(last - first > 1)
    {
        Iter middle = hoare_partition(first, last);
        hoare_quicksort(first, middle + 1);
        hoare_quicksort(middle + 1, last);
    }
}

int main(int argc, char const *argv[])
{
    vector<int> iv{7, 5, 3, 4, 8, 9, 1, 2, 6};
    hoare_quicksort(iv.begin(), iv.end());
    for(auto x : iv) 
        cout << x << ' ';
    cout << endl;

    return 0;
}
