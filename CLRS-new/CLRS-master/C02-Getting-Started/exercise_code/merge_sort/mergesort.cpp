#include <bits/stdc++.h>
using namespace std;

template<typename Container>
Container merge(Container const lhs, Container const rhs)
{
    Container ret;
    auto l = lhs.cbegin();
    auto r = rhs.cbegin();
    
    //compare and copy, until either one exausted
    while (l != lhs.cend() && r != rhs.cend())                  //合并
        ret.push_back(*(*r < *l ? r : l)++);

    //copy the rest
    if (l == lhs.cend())
        while (r != rhs.cend())                                 //右边更长
            ret.push_back(*r++);
    else
        while (l != lhs.cend())                                 //左边更长
            ret.push_back(*l++);                                //直接拷贝过来

    return ret;
}

template<typename Container>
Container merge_sort(Container const& seq)
{
    if (seq.size() > 1)
    {
        auto mid = seq.cbegin() + seq.size() / 2;
        auto lhs = merge_sort(Container(seq.cbegin(), mid));    //分解
        auto rhs = merge_sort(Container(mid, seq.cend()));
        return merge(lhs, rhs);
    }
    return seq;
}

// 打印vector中的所有元素
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v)
{
    for (int i = 0; i < (int)v.size(); ++i) {
        i == 0 ? out << v[i] : cout << ' ' << v[i];
    }
    return out;
}

int main(int argc, char const *argv[])
{
    vector<int> iv{1, 8, 3, 7, 5, 2, 3, 4, 9, 6};
    // cout << merge_sort(iv) <<endl;

    auto it = iv.begin();
    // cout << *it++ << endl;   //先取迭代器的值，迭代器再自加
    // cout << *it << endl;
 
    return 0;
}
