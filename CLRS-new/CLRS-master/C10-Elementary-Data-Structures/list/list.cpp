#include <bits/stdc++.h>
#include <memory>
#include <iostream>
#include <assert.h>
using namespace std;

template<typename T>
struct node;

template<typename T>
class double_list;

/**
 * @brief The node struct
 *
 * @attention next : shared_ptr
 *            prev : weak_ptr
 */
template<typename T>
struct node
{
    using ValueType = T;
    using sPointer  = std::shared_ptr<node<ValueType>>;
    using wPointer  = std::weak_ptr<node<ValueType>>;

    /**
     * @brief ctor
     */
    explicit node(const ValueType& val):
        key(val), prev(), next(nullptr)
    {}

    ValueType   key;
    wPointer    prev;
    sPointer    next;
};

template<typename T>
class single_list_ring
{
public:
    using ValueType = T;
    using Node      = node<ValueType>;
    using sPointer  = std::shared_ptr<Node>;
    using SizeType  = std::size_t;

    /**
     * @brief default ctor
     */
    single_list_ring():
       count(0), nil(std::make_shared<Node>(12345))
    {
        nil->next   =   nil;
    }

    /**
     * @brief search
     *
     * @complexity  O(n)
     */
    sPointer search(const ValueType& val) const
    {
        sPointer ptr = nil->next;
        while(ptr != nil && ptr->key != val)
            ptr = ptr->next;

        return ptr;
    }

    /**
     * @brief insert
     *
     * @complexity O(1)
     *
     *
     * example: insert(3)
     * before inserting:    [nil][7][8][9]
     *                            ^
     *                            begin
     * after inserting:     [nil][3][7][8][9]
     *                            ^
     *                            begin
     *
     *
     * as ex10.2-1 required.
     */
    void insert(const ValueType& val)
    {
        sPointer inserted   =   std::make_shared<Node>(val);
        inserted->next      =   nil->next;
        nil->next           =   inserted;

        ++count;
    }

    /**
     * @brief remove
     *
     * @param target    shared pointer
     *
     * @complexity  O(n)
     *
     * check ex10.2-1 for detail.
     */
    void remove(sPointer target)
    {
        assert(target != nil && !empty());
        prev(target)->next    =   target->next;

        --count;
    }

    /**
     * @brief reverse
     *
     * @complexity O(n)
     */
    void reverse()
    {
        sPointer current = begin();
        nil->next   =   nil;
        while(current != nil)
        {
            insert(current->key);
            current = current->next;
        }
    }

    /**
     * @brief size
     *
     * @complexity  O(1)
     */
    SizeType size() const
    {
        return count;
    }

    /**
     * @brief empty
     */
    bool empty() const
    {
        return begin() == end();
    }

    /**
     * @brief begin
     */
    const sPointer& begin() const
    {
        return nil->next;
    }

    /**
     * @brief end
     */
    const sPointer& end() const
    {
        return nil;
    }

private:

    /**
     * @brief size counter
     */
    SizeType count;

    /**
     * @brief nil
     *
     * used as a dummy sentinel on the list
     */
    sPointer nil;

    /**
     * @brief search and return the predecessor of target
     */
    sPointer prev(sPointer target) const
    {
        sPointer ptr = nil;
        while(ptr->next != target)
            ptr = ptr->next;

        return  ptr;
    }
};

/**
 * @brief doubly linked list without sentinel
 */
template<typename T>
class double_list                                   //没有使用哨兵
{
public:
    using ValueType = T;
    using Node      = node<ValueType>;
    using sPointer  = std::shared_ptr<Node>;
    using wPointer  = std::weak_ptr<Node>;
    using SizeType  = std::size_t;

    double_list() {};

    /**
     * @brief operator []
     */
    ValueType& operator[](SizeType index) const
    {
        assert(index < size());

        SizeType count = 0;
        sPointer ptr = head;
        while(count++ != index)
            ptr = ptr->next;

        return ptr->key;
    }

    /**
     * @brief operator +
     *
     * @complexity O(1)
     *
     * append the rhs to this list, implemented for ex10.2-6.
     */
    double_list<ValueType>& operator + (double_list<ValueType>& rhs)
    {
        if(rhs.empty())
            return *this;
        else
        {
            if(this->empty())
                return rhs;
            else    //neither is empty
            {
                sPointer tail   =   this->tail();

                tail->next      =   rhs.head;
                rhs.head->prev  =   tail;

                return *this;
            }
        }
    }

    /**
     * @brief insert
     *
     * @complexity O(1)
     *
     * based on LIST-INSERT, page 238.
     */
    void insert(const ValueType& val)
    {
        sPointer inserted = std::make_shared<Node>(val);

        inserted->next = head;

        if(!empty())
            head->prev  = inserted;

        head = inserted;
    }

    /**
     * @brief size
     *
     * @complexity  O(size)
     */
    SizeType size() const
    {
        SizeType size = 0;
        sPointer ptr = head;
        while(ptr != nullptr)
        {
            ++size;
            ptr = ptr->next;
        }
        return size;
    }

    /**
     * @brief search
     *
     * @param key
     *
     * @complexity  O(1)
     */
    sPointer search(const ValueType& key) const
    {
        sPointer ret = head;
        while(ret != nullptr && ret->key != key)
            ret = ret->next;
        return ret;
    }

    /**
     * @brief begin
     *
     * @complexity  O(1)
     *
     * a const ref to head
     */
    const sPointer& begin() const
    {
        return head;
    }

    /**
     * @brief empty
     */
    bool empty()const
    {
        return head == nullptr;
    }

    /**
     * @brief remove
     *
     * @param target    a shared_ptr pointing to a node on the list
     *
     * @complexity  O(1)
     *
     * based on LIST-DELETE page 238.
     */
    void remove(sPointer target)
    {
        assert(target != nullptr);

        if(target->prev.lock() != nullptr)
            target->prev.lock()->next = target->next;
        else
            head = target->next;

        if(target->next != nullptr)
            target->next->prev = target->prev;
    }

private:    
    sPointer head   =   nullptr;

    /**
     * @brief return tail
     *
     * @complexity  O(n)
     */
    sPointer tail() const
    {
        if(empty())
            return head;
        else
        {
            sPointer ret = head;
            while(ret->next != nullptr)
                ret = ret->next;

            return ret;
        }
    }

};//class double_list

/**
 * @brief operator <<
 */
template<typename T>
inline std::ostream& operator <<(std::ostream& os, const double_list<T>& l)
{
    using sPointer = typename double_list<T>::sPointer;

    sPointer ptr = l.begin();
    while(ptr != nullptr)
    {
        os << ptr->key << std::endl;
        ptr = ptr->next;
    }
    return os;
}

template<typename T>
class queue_by_list
{
public:
    using ValueType = T;
    using Node      = node<ValueType>;
    using sPointer  = std::shared_ptr<Node>;
    using SizeType  = std::size_t;

    queue_by_list(){};

    /**
     * @brief enqueue
     *
     * @complexity  O(1)
     *
     * as required in ex10.2-3
     */
    void enqueue (const ValueType& val)
    {
        sPointer added = std::make_shared<Node>(val);
        if(count == 0)
        {
            head = tail = added;
            count = 1;
        }
        else
        {
            tail->next  =   added;
            tail        =   added;
            ++count;
        }
    }


    /**
     * @brief dequeue
     *
     * @complexity  O(1)
     *
     * as required in ex10.2-3
     */
    ValueType dequeue()
    {
        assert( count > 0);

        ValueType ret = head->key;
        head = head->next;
        --count;

        return ret;
    }

    SizeType size() const
    {
        return count;
    }

    bool empty() const
    {
        return head == tail == nullptr;
    }

private:
    sPointer head;
    sPointer tail;
    SizeType count = 0;
};

template<typename T>
class stack_by_list
{
public:
    using ValueType =   T;
    using Node      =   node<ValueType>;
    using sPointer  =   std::shared_ptr<Node>;
    using SizeType  =   std::size_t;
    using List      =   single_list_ring<ValueType>;

    /**
     * @brief default ctor
     */
    stack_by_list(){};

    SizeType size() const
    {
        return data.size();
    }

    bool empty() const
    {
        return data.empty();
    }

    void print() const
    {
        std::cout << data;
    }

    /**
     * @brief enqueue
     *
     * @complexity  O(1)
     *
     * as required in ex10.2-2
     */
    void enqueue(const ValueType& val)
    {
        data.insert(val);
    }

    /**
     * @brief dequeue
     *
     * @complexity  O(1)
     *
     * as required in ex10.2-2
     */
    ValueType dequeue()
    {
        ValueType top = data.begin()->key;
        data.remove(data.begin());           //出栈从最开始移除

        return top;
    }
private:
    List data;
};

int main(int argc, char const *argv[])
{
    double_list<int> s1,s2, uni;
    for(int i = 0; i !=10 ; ++i){
       s1.insert(i);
       s2.insert(i*10);
    }

   uni = s1 + s2;

   std::cout << s1.empty() << std::endl;
   std::cout << s2 << std::endl;

   std::cout << "uni =\n" << uni << std::endl;

   queue_by_list<long> q;
    for(int i = 0; i != 10; ++i)
        q.enqueue(i);

    for(int i = 0; i != 10; ++i)
        std::cout << q.dequeue() << std::endl;

    cout << "stack_by_list" << endl;
    stack_by_list<long> s;
    for(int i = 0; i != 10; ++i)
        s.enqueue(i);

    for(int i = 0; i != 10; ++i)
        std::cout << s.dequeue() << std::endl;

    return 0;
}
