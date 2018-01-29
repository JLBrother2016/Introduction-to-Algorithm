#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

template<typename T, typename C = std::vector<T> >
class stack
{
public:
    using ValueType = T;
    using Container = C;
    using SizeType  = typename Container::size_type;
    /**
     * @brief default ctor
     *
     * using the specified underlying container.
     */
    stack() :
        c()
    {}
    /**
     * @brief check if empty
     *
     * @complexity O(1)
     */
    bool empty()
    {
        return c.empty();
    }
    /**
     * @brief size
     */
    SizeType size() const
    {
        return c.size();
    }
    /**
     * @brief push
     *
     * @complexity O(1)
     */
    void push(const ValueType& val)
    {
        c.push_back(val);
    }
    /**
     * @brief pop
     *
     * @complexity O(1)
     */
    void pop()
    {
        assert(!c.empty());
        c.pop_back();
    }
    /**
     * @brief top
     *
     * @complexity O(1)
     */
    ValueType& top()
    {
        assert(!c.empty());
        return c.back();
    }
private:
    Container c;
};

int main(int argc, char const *argv[])
{
    stack<int> is;
    is.push(44);
    cout << is.top() << endl;


    return 0;
}
