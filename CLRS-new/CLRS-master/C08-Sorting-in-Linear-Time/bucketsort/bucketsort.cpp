#include <bits/stdc++.h>
using namespace std;

template<typename Iterator,typename CompareType=std::less<typename std::iterator_traits<Iterator>::value_type>>
Iterator partition(const Iterator begin,const Iterator end,const Iterator partition_iter,CompareType compare=CompareType())
{
    //typedef typename std::iterator_traits<Iterator>::value_type T;// 迭代器指向对象的值类型
    auto size=std::distance(begin,end);
    assert(size>=0);
    if(size==0) return end;  //空序列，返回end
//            if(size<=1)
//                return begin;
    assert(std::distance(begin,partition_iter)>=0 &&std::distance(partition_iter,end)>0);
    auto smaller_next=begin;  //指向比key小的元素区间的下一个(即大于等于key元素区间的第一个），其中key为序列最后一个元素
    auto current=begin;  //指向当前待处理的元素
    std::swap(*partition_iter,*(end-1));//交换partition元素到末尾
    while(current!=end-1)
    {
        if(compare(*current,*(end-1)))
        {
            std::swap(*smaller_next,*current);
            smaller_next++;
        }
        current++;
    }
    std::swap(*smaller_next,*(end-1));//交换partition元素到它的位置
    return smaller_next;
}

template<typename Iterator,typename CompareType=std::less<typename std::iterator_traits<Iterator>::value_type>>
void quick_sort(const Iterator begin,const Iterator end, CompareType compare=CompareType())
{
    //typedef typename std::iterator_traits<Iterator>::value_type T;// 迭代器指向对象的值类型
    auto size=std::distance(begin,end);
    if(size<=1)
        return;
    auto partitioned_iter=partition(begin,end,end-1,compare); // 以最后一个元素作为划分元素，返回其顺序统计量
    quick_sort(begin,partitioned_iter,compare);
    quick_sort(partitioned_iter+1,end,compare);
}

template<typename Iterator>
void bucket_sort(const Iterator begin,const Iterator end,const typename std::iterator_traits<Iterator>::value_type& min_val,
                 const typename std::iterator_traits<Iterator>::value_type& max_val)
{
    assert(min_val<max_val);  //确保最小值小于最大值
    typedef typename std::iterator_traits<Iterator>::value_type T; // 迭代器指向对象的值类型
    std::size_t real_bucket_num=10; //划分10个区间
    std::vector<T> buckets[real_bucket_num];       //每个表项是一个vector

    Iterator iter=begin;
    while(iter!=end)
    {
        auto value=*iter;
        auto index=(value-min_val)*real_bucket_num/(max_val-min_val); // 这里采取最简单的hash：线性分布,哈希函数
        if(index<real_bucket_num)
            buckets[index].push_back(value); //放元素到桶中
        else
            buckets[real_bucket_num-1].push_back(value); //最后一个区间放超大数
        iter++;
    }
    std::size_t inserted_total=0;
    for(std::size_t i=0;i<real_bucket_num;i++)
    {
        quick_sort<typename std::vector<T>::iterator>(buckets[i].begin(),buckets[i].end());//对某个区间排序
        std::copy(buckets[i].begin(),buckets[i].end(),begin+inserted_total);//桶排序需要大量的桶空间（因此是非原地）
        inserted_total+=buckets[i].size();
    }
}

int main(int argc, char const *argv[])
{
    vector<int> iv{912,901,890,789,678,567,456,345,234,123};
    bucket_sort(iv.begin(), iv.end(), 100, 1000);
    for(auto x : iv)
        cout << x << ' ';
    cout << endl;

    return 0;
}
