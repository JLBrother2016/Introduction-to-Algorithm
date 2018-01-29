#include <bits/stdc++.h>
using namespace std;

template<typename Iterator>
void count_sort(const Iterator begin,const Iterator end,const typename std::iterator_traits<Iterator>::value_type& max_val)
{
    typedef typename std::iterator_traits<Iterator>::value_type T;                       // 迭代器指向对象的值类型
    static_assert(std::is_integral<T>::value, "sequence to be sorted must be integer!"); //必须针对整数进行计数排序

    auto size=std::distance(begin, end);
    if(size <=1) return;

    std::vector<T> CounterArray(max_val+1);                                              //存放计数结果
    std::vector<T> ResultArray(size);                                                    //暂存排序结果
    std::size_t n=0;
    while(begin+n!=end) {                                                                //计个数
       CounterArray.at(*(begin+n))++;
       n++;
    }
    for(std::size_t i=1;i<CounterArray.size();i++)                                       //计排位数
    {
          CounterArray.at(i) += CounterArray.at(i-1);
    }
    int index=ResultArray.size()-1;
    while(index>=0)
    {
         auto data=*(begin+index);                                                       //待排序的元素
         T less_data_num=CounterArray[data]-1;                                           //比它小的元素的个数
         ResultArray[less_data_num]=data;                                                //直接定位
         CounterArray[data]--;                                                           //此行为了防止重复元素的定位
         index--;
    }
    std::copy(ResultArray.begin(), ResultArray.end(), begin);
}

template<typename T> T digi_on_N(T num,std::size_t n)
{
   static_assert(std::is_integral<T>::value, "T must be integer!"); //必须针对整数才能取指定位数上的数字
   return num/(T)std::pow(10,n)-num/(T)std::pow(10,n+1)*10;
}

template<typename Iterator,typename CompareType=std::less<typename std::iterator_traits<Iterator>::value_type>>
                void insert_sort(const Iterator begin,const Iterator end,CompareType compare=CompareType())
     {
            //typedef typename std::iterator_traits<Iterator>::value_type T;// 迭代器指向对象的值类型
            auto size=std::distance(begin,end);
            if(size<=1)
                return;
            Iterator current=begin;
            while(++current!=end)
            {
                auto small_next=current; //指向比*current小的元素中最大的那个元素
                while(small_next!=begin && compare(*current,*(small_next-1))) //current较小，则向前寻找插入的位置插入
                {
                    small_next--;
                }

                auto key=*current;
                auto iter=current;
                while(iter!=small_next) //插入
                {
                    *iter=*(iter-1); //后移
                    iter--;
                }
                *(iter)=key;
            }
    }


template<typename Iterator>
void radix_sort(const Iterator begin,const Iterator end,std::size_t radix_width)
{
    typedef typename std::iterator_traits<Iterator>::value_type T;
    assert(radix_width!=0); //位数为有效的，0位数无效
    static_assert(std::is_integral<T>::value, "sequence to be sorted must be integer!"); //必须针对整数进行基数排序
    auto size=std::distance(begin,end);
    if(size<=1)
        return;
    for(std::size_t i=0; i < radix_width;i++) //从最低位(第0位为个位）到 （RADIXWITH-1）位的位数进行排序（一共RADIXWITH位）
    {
        auto compare = [i](T little, T big){return digi_on_N(little,i)<digi_on_N(big,i);}; //必须用 < 比较才是稳定排序，否则是不稳定排序，不能用快排（非稳定）
        insert_sort<Iterator,decltype(compare)>(begin,end,compare);
    }
}

int main(int argc, char const *argv[])
{
    vector<int> iv1{555,555,544,554,545,444,455,445,454,444};
    vector<int> iv{912,901,890,789,678,567,456,345,234,123};
    radix_sort(iv.begin(), iv.end(), 3);
    for(auto x : iv)
        cout << x << ' ';
    cout << endl;

    return 0;
}
