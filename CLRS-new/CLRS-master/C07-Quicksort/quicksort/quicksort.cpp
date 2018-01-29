#include <bits/stdc++.h>
using namespace std;

template<typename Iter>
void quick_sort(Iter first, Iter last);

template<typename Iter>
Iter partition(Iter first, Iter last);

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
inline void quick_sort(Iter first, Iter last)
{
    if(last - first > 1)
    {
        Iter pivot = partition(first, last);
        quick_sort(first, pivot);           //小于主元的部分，重新用分治的思想递归
        quick_sort(pivot + 1, last);        //大于主元的部分
    }
}

int main(int argc, char const *argv[])
{
    vector<int> iv{7, 5, 3, 4, 8, 9, 1, 2, 6};
    quick_sort(iv.begin(), iv.end());
    for(auto x : iv) 
        cout << x << ' ';
    cout << endl;

    return 0;
}
