#include <memory>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

template<typename T>
struct node;

template<typename T>
class binary_tree;


/**
 * @brief node
 *
 * used for binary tree.
 */
template<typename T>
struct node
{
    using ValueType =   T;
    using Node      =   node<ValueType>;
    using sPointer  =   std::shared_ptr<Node>;
    using wPointer  =   std::weak_ptr<Node>;

    /**
     * @brief default ctor
     */
    node(){};

    /**
     * @brief ctor with a key
     */
    explicit node(const ValueType& val):
        key(val)
    {}

    ValueType   key     = ValueType();
    wPointer    parent;
    sPointer    left    = nullptr;
    sPointer    right   = nullptr;
};

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

/**
 * @brief binary tree
 */
template<typename T>
class binary_tree
{
public:
    using ValueType =   T;
    using Node      =   node<ValueType>;
    using sPointer  =   std::shared_ptr<Node>;
    using wPointer  =   std::weak_ptr<Node>;

    /**
     * @brief insert interface
     */
    void insert(const ValueType& val)
    {
        recur_add(root, val);
    }

    /**
     * @brief print_by_recursion
     *
     * just an interface,check recur_print() for real work.
     */
    void print_by_recursion() const
    {
        recur_print(root);
    }

    /**
     * @brief print_with_stack
     *
     * just an interface, check recur_print() for real work.
     */
    void print_with_stack() const
    {
        nonrecur_print(root);
    }

    /**
     * @brief print_with_prev
     *
     * just an interface, checking_prev_print() for real work.
     */
    void print_with_prev() const
    {
        checking_prev_print(root);
    }


private:
    sPointer root;

    /**
     * @brief recur_add
     *
     * actual work for insert interface
     */
    void recur_add(sPointer& current, const ValueType& val, sPointer prev = nullptr)
    {
        if(current == nullptr)
        {
            current = std::make_shared<Node>(val);
            current->parent = prev;
            return;
        }
        else
        {
            if(current->key > val)
                recur_add(current->left, val, current);
            else
                recur_add(current->right, val, current);
        }
    }

    /**
     * @brief recur_print
     *
     * @complexity O(n)
     *
     * implemented for ex10.4-2
     * check above for pseudocode, below for test code.
     */
    void recur_print(sPointer node) const
    {
        if(node != nullptr)
        {
            std::cout << node->key << std::endl;
            recur_print(node->left);
            recur_print(node->right);
        }
    }

    /**
     * @brief nonrecur_print
     *
     * @complexity  O(n)
     *
     * implemented for ex10.4-3
     * check above for pseudocode, below for test code.
     */
    void nonrecur_print(sPointer node) const
    {
        if(node)
        {
            stack<sPointer> stack;
            stack.push(node);
            while(!stack.empty())
            {
                sPointer current = stack.top();
                stack.pop();
                std::cout << current->key << std::endl;

                if(current->left)
                    stack.push(current->left);
                if(current->right)
                    stack.push(current->right);
            }
        }
    }

    /**
     * @brief checking_prev_print
     *
     * @complexity  O(2n) = O(n)
     *
     * for ex10.4-5 *
     * check above for pseudocode, below for test code.
     */
    void checking_prev_print(sPointer node) const
    {
        sPointer prev    = nullptr;
        sPointer current = node;

        while(current)
        {
            if(prev == current->parent.lock())
            {
                std::cout << current->key << std::endl;
                prev = current;

                current = current->left?    current->left :
                    current->right? current->right  :   current->parent.lock();
            }
            else if(prev == current->left && current->right)
            {
                prev = current;
                current = current->right;
            }
            else
            {
                prev = current;
                current = current->parent.lock();
            }
        }
    }
};

int main(int argc, char const *argv[])
{
    node<int> n;
    binary_tree<int> tree;
    vector<int> iv{1,5,7,3,2,8,4,98};
    for(auto& x : iv)
        tree.insert(x);

    tree.print_by_recursion();

    return 0;
}
