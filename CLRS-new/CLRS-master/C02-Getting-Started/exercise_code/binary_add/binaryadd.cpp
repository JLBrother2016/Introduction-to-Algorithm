#include <bits/stdc++.h>
using namespace std;

template<typename Container>
Container add_binary_numbers(Container const& lhs, Container const& rhs)
{
    Container sum(lhs.size() + 1);
    auto l = lhs.crbegin();
    auto r = rhs.crbegin();
    typename Container::value_type carry = 0;                     //进位
    for (auto curr = sum.rbegin(); curr != sum.rend() - 1; ++curr) {
        unsigned bit_sum = *l++ + *r++ + carry;                   //计算每一位   
        *curr = bit_sum % 2;
        carry = bit_sum / 2;
    }
    sum[0] = carry;

    return sum;
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
    vector<int> iv1{1, 1};
    vector<int> iv2{1, 1};
    cout << add_binary_numbers(iv1, iv2) << endl;
    
    return 0;
}