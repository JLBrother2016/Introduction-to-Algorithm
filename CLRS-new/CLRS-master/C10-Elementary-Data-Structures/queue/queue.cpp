#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

template<typename T, typename C = std::vector<T> >
class queue;
template<typename T>
inline bool
operator ==(const queue<T>& lhs, const queue<T>& rhs)
{
    return (lhs.c == rhs.c) && (lhs.head == rhs.head) && (lhs.tail == rhs.tail);
}

template<typename T, typename C>
class queue
{
    friend bool operator ==<T>(const queue<T>&, const queue<T>&);
public:
    using Container = C;
    using Iter      = typename Container::iterator;
    using SizeType  = typename Container::size_type;

    /**
     * @brief ctor
     * @param size
     */
    explicit queue(std::size_t size):
        c(size)
    {
         head = tail = c.begin();
    }

    /**
     * @brief enqueue
     * @param val
     *
     * based on Page 235
     */
    void enqueue(const T& val)
    {
        assert(!full() && "overflow");

        if(tail == c.end())
            tail = increment(tail);

        *tail = val;
        tail = increment(tail);
    }

    /**
     * @brief dequeue
     *
     * based on Page 235
     */
    T dequeue()
    {
        assert(!empty() && "underflow");

        T ret = *head;
        head = increment(head);

        if(head == c.end())
            head = increment(head);

        return ret;
    }

    /**
     * @brief empty
     */
    bool empty() const
    {
        return head == tail;
    }

    /**
     * @brief full
     */
    bool full()
    {
        return head == increment(tail) ;
    }

    /**
     * @brief return the max number of elements that can be held.
     */
    SizeType capacity() const
    {
        return c.size();
    }

    /**
     * @brief return how many elements are being held
     */
    SizeType size() const
    {
        return ( tail >= head ) ? tail - head : capacity() - (head - tail);
    }

private:
    Container c;
    Iter head;
    Iter tail;

    /**
     * @brief increment
     * @param iter
     *
     * increment iterator or go back to begin when reached end.
     */
    Iter increment(Iter iter)
    {
        return (iter == c.end()) ? c.begin() : iter + 1 ;
    }
};


int main() {
    queue<int> myqueue(3);
    myqueue.enqueue(1);
    myqueue.enqueue(3);
    myqueue.enqueue(45);
    cout << myqueue.dequeue() << endl;
    cout << myqueue.dequeue() << endl;
}


