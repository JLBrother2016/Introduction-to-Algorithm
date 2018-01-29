#include <bits/stdc++.h>
using namespace std;

template<typename Container>
typename Container::value_type binarysearch(Container const &seq, typename Container::value_type v) {
     typename Container::value_type low = 0, high = seq.size() - 1;
    while(low <= high) {
         typename Container::value_type mid = low + (high - low) / 2;
        if(seq[mid] == v)
            return mid;
        else if(seq[mid] < v) low++;
        else high++;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    vector<int> iv{1, 2, 3, 4, 5, 6};
    cout << binarysearch(iv, 6) << endl;

    return 0;
}