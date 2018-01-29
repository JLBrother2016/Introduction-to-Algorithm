#include <bits/stdc++.h>

using namespace std;
template<typename Iter>
using IterValue = typename std::iterator_traits<Iter>::value_type;
/**
 * @brief parent
 */
namespace ch6 {
template<typename Iter>
inline Iter parent(Iter first, Iter curr) {    //如果是整数的话，可以用左移做乘法，右乘做除
    return first + (curr - first - 1) / 2; 
}
}

/**
 * @brief left child
 */
template<typename Iter>
inline Iter left(Iter first, Iter curr) {
    return first + 2 * (curr - first) + 1;
}

/**
 * @brief right child
 */
template<typename Iter>
inline Iter right(Iter first, Iter curr) {
    return first + 2 * (curr - first) + 2;
}

 /**
 * @recurrence T(n) <= T(2n/3) + theta(1)
 * @complx  O(lg n)
 */
template<typename Iter, typename Comp = std::greater<IterValue<Iter> > >
void heapify(Iter first, Iter last, Iter curr, Comp&& compare = Comp())
{
    //! left and right child
    Iter l = left(first, curr), r = right(first, curr);                 //当前节点的左孩子，和当前节点的右孩子

    //! find the max or min depending on Comp object
    auto max_min = (l < last &&  compare(*l, *curr)) ? l : curr;        //找到左右孩子、当前节点的最大值
    if(r < last && compare(*r, *max_min))
            max_min = r;
    //! recur
    if(max_min != curr) {                                               //把当前节点换为最大值
        std::swap(*max_min, *curr);                                     //把最大位置换为当前节点的值
        heapify(first, last, max_min, compare);                         //向下递归调用，直到所有的节点满足条件
    }
}
/*
 * @pseudocode  BUILD-MAX-HEAP, Page 157.
 * @complx  O(n)
 * @proof   Page 159.
 */
template<typename Iter,typename Comp = std::greater<IterValue<Iter> > >
inline void
build_heap(Iter first, Iter last, Comp&& compare = Comp())               //最大堆实现
{
    auto size = last - first;
    for(auto curr = first + size/2 - 1; curr != first - 1; --curr)
        heapify(first, last, curr, compare);
}
/*
 * @recurrence T(n) <= T(2n/3) + theta(1)
 * @complx  O(lg n)
 *
 * for ex6.2-5
 */
template<typename Iter, typename Comp = std::greater<IterValue<Iter> > >
void heapify_itera(Iter first, Iter last, Iter curr, Comp&& compare = Comp())
{
    for(;;) {
        Iter l = left(first, curr);
        Iter r = right(first, curr);

        //! find max(or min) amoung curr, left and right.
        auto max_min = (l < last && compare(*l, *curr)) ? l : curr;
        if(r < last && compare(*r, *max_min))
                max_min = r;
        //! exchange
        if(max_min != curr) {
            std::swap(*max_min, *curr);
            curr = max_min;
        }
        else break;
    }
}

// template<typename Iter, typename Comp = std::greater<IterValue<Iter> > >
// void heapDelete(Iter first, Iter last, Iter curr, Comp&& compare = Comp()) 
// {
    // HEAP-DELETE(A, i):
    // if A[i] < A[A.heap-size]
    //     HEAP-INCREASE-KEY(A, i, A[A.heap-size])
    //     A.heap-size -= 1
    // else
    //     A[i] = A[A.heap-size]
    //     A.heap-size -= 1
    //     MAX-HEAPIFY(A,i)
// }

template<typename Range>
inline void
print_container(const Range& rng)
{
    for(const auto& elem : rng)
        std::cout << elem << " ";
}

template<typename Iter>
inline void heap_sort(Iter first, Iter last)
{
    build_heap(first, last);                                    //堆排序时，先建立最大堆
    for(auto curr = last - 1; curr != first; --curr)
    {
        std::swap(*first, *curr);                               //把最大堆的根节点取出来，放到容器的尾部
        heapify(first, curr, first);                            //只用维护剩余部分的最大堆，并且维护的当前节点是第一个元素
    }
}

/**
 * @brief The PriorityQueue class template
 */
template<typename T>
class PriorityQueue
{
public:
    using ValueType     =   T;
    using Vector        =   std::vector<T>;
    using SizeType      =   typename Vector::size_type;
    using Iter          =   typename Vector::iterator;
    using Compare       =   std::function<bool(ValueType,ValueType)>;

    /**
     * @brief default Ctor
     */
    explicit PriorityQueue(Compare&& c = std::greater<ValueType>()):
        vec{}, compare{c}
    {}

    /**
     * @brief Ctor
     * @param list
     * @param c
     */
    explicit PriorityQueue(std::initializer_list<ValueType>&& list,             //基于初始化链表实现
                           Compare&& c = std::greater<ValueType>()) noexcept:   //不抛异常   
        vec{std::move(list)}, compare{c}
    {
        build_heap(begin(), end(), compare);                                    //建立一个最大堆
    }

    /**
     * @brief Ctor
     * @param first
     * @param last
     * @param c
     */
    template<typename Iterator>
    PriorityQueue(Iterator first,
                  Iterator last,
                  Compare&& c = std::greater<ValueType>()):
        vec{first, last}, compare{c}
    {
        build_heap(begin(), end(), compare);
    }

    /**
     * @brief top
     */
    const ValueType& top()const
    {
        return vec.front();             //返回最大值                                     
    }

    /**
     * @brief size
     */
    SizeType size()const
    {
        return vec.size();
    }

    /**
     * @brief empty
     */
    bool empty()const
    {
        return vec.empty();
    }

    /**
     * @brief pop
     */
    void pop()
    {
        extract();
    }

    /**
     * @brief push
     * @param added
     */
    void push(const ValueType& added)
    {
        insert(added);
    }

    /**
     * @brief push
     * @param added
     * @note  move semantics
     */
    void push(ValueType&& added) noexcept
    {
        insert(std::move(added));
    }

    /**
     * @brief begin
     */
    Iter begin()
    {
        return vec.begin();
    }

    /**
     * @brief end
     */
    Iter end()
    {
        return vec.end();
    }

private:
    Vector vec;
    Compare compare;

    /**
     * @brief parent
     */
    Iter parent(Iter it)
    {
        return ch6::parent(begin(), it);     //返回最大堆的父节点
    }

    /**
     * @brief extract_max
     *
     * @pseudocode HEAP-EXTRACT-MAX, Page 163
     * @complx  O(lg n)
     */
    ValueType extract()      //去掉并返回最大键值的元素         
    {
        if(empty())
            throw std::underflow_error{"heap underflow."};

        auto front = vec.front();          //取出最大的元素
        vec.front() = vec.back();          //堆末尾的元素赋值给堆首
        vec.resize(size() - 1);            //把堆的大小减一

        heapify(begin(), end(), begin(), compare);  //重新维护一次最大堆
        return front;
    }

    /**
     * @brief insert
     * @param added
     * @pseudocode  MAX-HEAP-INSERT,    page 164
     *              HEAP-INCREASE-KEY,  page 164
     * @complx  O(lg n)
     * modified for ex6.5-6 that requires using use
     * the idea of the inner loop of INSERTION-SORT .
     */
    void insert(const ValueType& added)
    {
        //! find the right place for added
        vec.resize(size() + 1);
        auto curr = end() - 1;      //curr是一个迭代器
        while(curr > begin() && compare(added, *parent(curr))) {   //向上循环找到合适的位置放added
            *curr = *parent(curr);
            curr = parent(curr);
        }
        //! insert added.
        (curr < begin() ? vec.front() : *curr) = added;     //如果added比所有元素大，放堆的根节点，否则放到curr指向的节点
    }
};


int main(int argc, char const *argv[])
{
    vector<int> iv {4,1,3,2,16,9,10,14,8,7};
    heap_sort(iv.begin(), iv.end());
    print_container(iv);
    cout << endl;

    // PriorityQueue<int> queue {{1,2,3,44,5,6}, std::less<int>()};
    PriorityQueue<int> queue {{1,2,3,44,5,6}};
    queue.push(-43);
    cout << queue.top() << endl;
    queue.push(-42);
    queue.pop();
    for(auto elem : {11,13,15,16,19})
        queue.push(elem);
    std::cout << queue.top() << endl;

    return 0;
}


/*  习题6.5-9
struct cmp{
    bool operator() (ListNode *n1, ListNode *n2) {
        return n1->val >= n2->val;
    }
};

class Solution {
public:
    ListNode *mergeKLists(vector<ListNode *> &lists) {
        priority_queue<ListNode*, vector<ListNode*>, cmp> qs;
        for (int i = 0;i < lists.size();i++)
            if (lists[i]) 
                qs.push(lists[i]);
        if (qs.empty()) return nullptr;
        ListNode* head(qs.top()), *backup(head);
        
        while (!qs.empty()) {
            ListNode *node = qs.top();
            qs.pop();
            if(node->next) qs.push(node->next);
            head->next = qs.top();
            head = head->next;
        }
        head->next = nullptr;
        return backup;
    }
};


*/
