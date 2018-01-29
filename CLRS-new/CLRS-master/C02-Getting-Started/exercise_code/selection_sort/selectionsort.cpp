#include <bits/stdc++.h>
using namespace std;

template<typename Container>
Container selection_sort(Container seq)
{
    if (seq.size() < 2) return seq;

    for (int i = 0; i != seq.size() - 1; ++i)
    {
        int index_for_min = i;
        for (int j = i + 1; j != seq.size(); ++j)
            if (seq[index_for_min] > seq[j])    index_for_min = j;
        std::swap(seq[i], seq[index_for_min]);
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
    vector<int> iv{10, 4, 9, 3, 5, 7, 2, 1};
    cout << selection_sort(iv) << endl;

    return 0;
}