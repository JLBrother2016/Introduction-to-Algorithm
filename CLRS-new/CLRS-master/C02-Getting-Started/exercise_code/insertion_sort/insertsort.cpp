#include <bits/stdc++.h>
using namespace std;

template<typename Iterator, typename CompareType = std::less<typename std::iterator_traits<Iterator>::value_type>>
                void insert_sort(const Iterator begin, const Iterator end, CompareType compare=CompareType())
    {
        //typedef typename std::iterator_traits<Iterator>::value_type T;// 迭代器指向对象的值类型
        auto size = std::distance(begin, end);                               //获得容器的长度
        if(size <= 1)
            return;
        Iterator current = begin;                                            //从第一个元素开始迭代
        while(++current != end) {                                            //从第二个元素到最后一个元素
            auto small_next = current;                                       //指向比*current小的元素中最大的那个元素
            while(small_next != begin && compare(*current, *(small_next-1))) //current较小，则向前寻找插入的位置插入
            {
                small_next--;                                                //找到要插入的位置
            }

            auto key = *current;                                             //取出要插入的值
            auto iter = current;
            while(iter != small_next) {    //插入                            //移动比插入值更大的值
                *iter = *(iter-1);         //后移
                iter--;
            }
            *(iter) = key;
        }
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
    vector<int> iv{10,9,8,7,5,6,4,3,2,1};
    insert_sort(iv.begin(), iv.end(), greater<int>());
    cout << iv << endl;

    vector<int> iv2{31, 41, 59, 26, 41, 58};
    insert_sort(iv2.begin(), iv2.end());
    cout << iv2 << endl;

    return 0;
}
