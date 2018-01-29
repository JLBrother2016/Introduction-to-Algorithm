#include <bits/stdc++.h>
using namespace std;

template<typename IntType> IntType  
radom_index(IntType begin,IntType end)      //产生随机整数
{
    assert(end>=begin);
    static std::random_device rd; //每一个std::random_device维持相同的伪随机序列，因此需要用静态变量保证只有一个std::random_device
    std::mt19937 gen(rd());
    std::uniform_int_distribution<IntType> dis(begin, end);
    return dis(gen);
}

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
typename std::iterator_traits<Iterator>::value_type
randomized_select(const Iterator begin,const Iterator end,typename std::iterator_traits<Iterator>::difference_type rank,CompareType compare=CompareType())
{
    //typedef typename std::iterator_traits<Iterator>::value_type T;
    auto size=std::distance(begin,end);
    assert(size>=0);
    assert(rank<size);
    auto partitioned_iter=partition(begin,end,begin+radom_index(0,size-1),compare); //随机划分
    auto distance=std::distance(begin,partitioned_iter);
    if(distance== rank)  //找到了该排位的数
        return *partitioned_iter;
    else if(distance<rank) //已知某排位的数位次较低，则指定排位数的元素在它右侧
        return randomized_select(partitioned_iter+1,end,rank-distance-1,compare); //从partitioned_iter+1，则找右侧的第rank-distance-1位
    else                   //已知某排位的数位次较高，则指定排位数的元素在它左侧
        return randomized_select(begin,partitioned_iter,rank,compare);            //递归调用
}

int main(int argc, char const *argv[])
{
    int data1[10]={1,2,3,4,5,6,7,8,9,10};
    auto val=randomized_select(data1+0,data1+10,0);
    cout << val << endl;

    return 0;
}
