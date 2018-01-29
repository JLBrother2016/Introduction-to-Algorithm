#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int> &num, int target) {
    unordered_map<int, int> hash;
    vector<int> result;
    for(int i = 0; i < num.size(); ++i) {
        int remain = target - num[i];
        if(hash.count(remain)) {
            result.push_back(hash[remain]);
            result.push_back(i);
            return result;
        }
        hash[num[i]] = i;
    }
    return result;
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
    vector<int> iv{2, 4, 5, 7, 2, 6, 3};
    cout << twoSum(iv, 8) << endl;

    return 0;
}