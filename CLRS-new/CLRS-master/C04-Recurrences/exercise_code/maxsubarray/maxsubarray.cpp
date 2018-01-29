#include <bits/stdc++.h>
using namespace std;

#include <iterator>

namespace clrs {

/**
 *  @brief  aliasing for value type an iterator points to
 */
template<typename Iter>
using IterValue = typename std::iterator_traits<Iter>::value_type;

}

template<typename Iter>
struct Record
{
    Iter first;
    Iter last;
    clrs::IterValue<Iter> sum;
};

template<typename Iter>
inline bool
operator >=(const Record<Iter>& lhs, const Record<Iter>& rhs)
{
    return lhs.sum  >= rhs.sum;
}

template<typename Iter>
inline bool
operator <(const Record<Iter>& lhs, const Record<Iter>& rhs)
{
    return lhs.sum  < rhs.sum;
}

template<typename Iter>
Record<Iter>
brute_force_find_max_subarray(Iter first, Iter last)
{
    Record<Iter> max{first, first + 1, *first};
    for(auto head = first; head != last; ++head)
        for(auto tail = last; tail != head ; --tail)
        {
            auto sum = std::accumulate(head, tail, 0);
            if(sum > max.sum)
                max = { head, tail, sum };
        }
    return max;
}

template<typename Iter>
Record<Iter>
find_max_crossing_subarray(Iter first, Iter mid, Iter last)
{
    //! left subarray   <--
    auto left_sum = *(mid - 1);
    clrs::IterValue<Iter> sum = 0;
    auto max_left = mid - 1;
    for(auto l = mid - 1; l != first - 1; --l){
        sum += *l;
        if(sum > left_sum)
        {
            left_sum = sum;
            max_left = l;
        }
    }

    //! right subarray  -->
    auto right_sum = *mid;
    sum = 0;
    auto max_right = mid;
    for(auto r = mid; r != last; ++r){
        sum += *r;
        if(sum > right_sum)
        {
            right_sum = sum;
            max_right = r;
        }
    }

    return {max_left, max_right, left_sum + right_sum};
}

template<typename Iter>
Record<Iter> find_max_subarray(Iter first, Iter last)                       //时间复杂度为O(nlgn)
{
    //! base case
    if(first == last - 1)
        return {first, last, *first};                                       //只有一个元素

    //! recur
    Iter mid = first + (last - first) / 2;                                  //求得中点
    Record<Iter> left   =   find_max_subarray(first, mid);                  //T(n)，递归左边
    Record<Iter> right  =   find_max_subarray(mid, last);                   //T(n)，递归右边
    Record<Iter> cross  =   find_max_crossing_subarray(first, mid, last);   //theta(n)，计算中间

    //! return the largest one
    if(left >= right && left >= cross)          return left;                //查找最大值
    else if(right >= left && right >= cross)    return right;
    else                                        return cross;
}

template<typename Iter>
Record<Iter> find_max_subarray_linear(Iter first, Iter last)
{
    using Vec = std::vector<Record<Iter>>;
    Vec v(last - first);
    v[0] = {first, first + 1, *first};
    for(int curr = 1; curr != v.size(); ++curr)
    {
        if(v[curr - 1].sum   <=  0)                          //前一天是没有挣钱
            v[curr] =
            {
                first + curr,
                first + curr + 1,
                *(first + curr)
            };
        else
            v[curr] =
            {
                v[curr - 1].first,
                first + curr + 1,
                v[curr - 1].sum + *(first + curr)            //把前面的记录了下来，下一次就不用再重复计算
            };
    }
    return *std::max_element(v.begin(),v.end());
}

int main(int argc, char const *argv[])
{
    // vector<int> iv{13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    // auto ret = brute_force_find_max_subarray(iv.begin(), iv.end());
    vector<int> iv{-1, -2, -3, -4, -6, -5, -1};
    auto ret = find_max_subarray(iv.begin(), iv.end()); 
    cout << *ret.first << " " <<  ret.sum << " " << *ret.last << endl;
    std::vector<int> v{13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    auto ret1 = find_max_subarray_linear(v.begin(), v.end());
    cout << *ret1.first << " " <<  ret1.sum << " " << *ret1.last << endl;
    
    return 0;
}
