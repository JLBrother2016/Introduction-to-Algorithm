#include <bits/stdc++.h>
using namespace std;


int linear_search(vector<int>& A, int v) {
    for(int i = 0; i < A.size(); ++i)
        if(A[i] == v)
            return i;
    return 0;
}

int main(int argc, char const *argv[])
{
    vector<int> iv{1, 2, 4, 6, 9};
    cout << linear_search(iv, 3) << endl;
    cout << linear_search(iv, 6) << endl;

    return 0;
}