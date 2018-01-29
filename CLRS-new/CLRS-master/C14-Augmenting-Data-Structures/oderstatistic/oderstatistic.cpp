#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <stdlib.h>
#include <cassert>

template<typename K, typename D>
class Node;

template<typename K, typename D>
class RedBlackTree;

template<typename K, typename D>
class OrderStatisticTree;

namespace debug{

/**
 * @brief green
 */
std::string green(const std::string& str)
{
    return "\033[1;32m" + str + "\033[0m";
}

/**
 * @brief red
 */
std::string red(const std::string& str)
{
    return "\033[1;31m" + str + "\033[0m";
}

/**
 * @brief yellow
 */
std::string yellow(const std::string& str)
{
    return "\033[1;33m" + str + "\033[0m";
}

}//namespace

/**
 * @brief The Color enum
 *
 * for red black tree
 */
enum class Color
{
    RED,
    BLACK
};

/**
 * @brief Node
 *
 * used for Red Black Trees.
 */
template<typename K, typename D>
class Node
{
    friend class RedBlackTree<K,D>;
    friend class OrderStatisticTree<K,D>;

public:
    using KeyType   =   K;
    using DataType  =   D;
    using SizeType  =   std::size_t;
    using NodeType  =   Node<K,D>;
    using sPointer  =   std::shared_ptr<NodeType>;
    using wPointer  =   std::weak_ptr<NodeType>;

    Node(){}

    /**
     * @brief Node
     * @param key
     */
    explicit Node(const KeyType& key):
        key(key),left(),right(), parent(), color(Color::BLACK),data(),size(0)
    {}

    /**
     * @brief Node
     * @param key
     * @param data
     */
    Node(const KeyType &key, const DataType& data):
        key(key),left(),right(), parent(), color(Color::BLACK),data(data),size(0)
    {}

    /**
     * @brief is_left
     *
     * Since no parent can be used for persistent trees, this metod can not
     * used directly
     */
    bool is_left() const
    {
        assert(parent.lock());
        return this == parent.lock()->left.get();
    }

    /**
     * @brief is_right
     *
     * Since no parent can be used for persistent trees, this metod can not
     * used directly
     */
    bool is_right() const
    {
        assert(parent.lock());
        return this == parent.lock()->right.get();
    }

    /**
     * @brief return the rank within the subtree rooted at this node.
     *
     * @complx  O(1)
     *
     * implemented for Order Statistic Tree
     */
    SizeType rank() const
    {
        return this->left?  this->left->size + 1    :   0;
    }

    /**
     * @brief print
     */
    void print()const
    {
        std::cout << "key= " + debug::green(std::to_string(key)) + " ";
        std::cout << (color == Color::RED?   debug::red("red ")   :   "blk ");
        std::cout << "parent=" << parent.lock();
        std::cout << debug::green(" self=") << this
                  << " left="   << left
                  << " right="  << right


                  << debug::green(" size=")  << size
                  << std::endl;
    }

protected:
    KeyType     key;
    sPointer    left;
    sPointer    right;
    wPointer    parent;
    Color       color = Color::BLACK;
    DataType    data;
    SizeType    size;
};

template<typename K, typename D>
class RedBlackTree
{
public:
    using KeyType   =   K;
    using DataType  =   D;
    using NodeType  =   Node<K,D>;
    using sPointer  =   std::shared_ptr<NodeType>;
    using wPointer  =   std::weak_ptr<NodeType>;

    //! @brief  default Ctor
    RedBlackTree():
        nil(std::make_shared<NodeType>())
    {
        root = nil;
    }

    /**
     * @brief search
     * @param key
     *
     * @complx  O(h)
     */
    sPointer search(const KeyType& key)const
    {
        sPointer ret  = nullptr;
        sPointer curr = this->root;
        while(curr)
        {
            if(curr->key    ==  key)
            {
                ret = curr;
                break;
            }
            else
                curr = (curr->key > key?    curr->left  :   curr->right);
        }
        return ret;
    }

    /**
     * @brief print
     *
     * O(n)
     */
    void print()const
    {
        std::cout << "--------------------------------\n";
        std::cout << debug::yellow("root= ") << root
                  << " key = " << root->key  << std::endl;
        std::cout << debug::yellow("nil = ") << nil
                  << std::endl;

        if(root == nil)
        {
            std::cout << debug::yellow("this is an EMPTY TREE!\n");
            return;
        }

        std::function<void(sPointer node)> inorder_walk =
                [&inorder_walk,this](sPointer node)
        {
            if(node != nil)
            {
                inorder_walk(node->left);
                node->print();
                inorder_walk(node->right);
            }
        };

        inorder_walk(root);
    }

    /**
     * @brief insert
     * @param key
     *
     * just an interface
     */
    void insert(const KeyType& key)
    {
        sPointer added = std::make_shared<NodeType>(key);
        insert(added);
    }

    /**
     * @brief insert
     * @param key
     * @param data
     *
     * just an interface
     */
    void insert(const KeyType &key, const DataType& data)
    {
        sPointer added = std::make_shared<NodeType>(key,data);
        insert(added);
    }

    /**
     * @brief remove
     *
     * @compx   O(lg n)
     *
     * @page    324
     */
    void remove(sPointer target)
    {
        sPointer x,y;
        Color  y_original_color;

        if(target->left == this->nil)
        {
            y = target;
            y_original_color = y->color;
            x = y->right;
            transplant(target,x);
        }
        else if(target->right   ==  this->nil)
        {
            y = target;
            y_original_color = y->color;
            x = y->left;
            transplant(target,x);
        }
        else
        {
            y = minimum(target->right);
            y_original_color = y->color;
            x = y->right;

            if(y->parent.lock() ==  target)
                x->parent   =   y;
            else
            {
                transplant(y,x);
                y->right    =   target->right;
                y->right->parent    =   y;
            }

            transplant(target, y);
            y->left         =   target->left;
            y->left->parent =   y;
            y->color        =   target->color;
        }

        if(y_original_color ==  Color::BLACK)     
            remove_fixup(x);

    }

    virtual ~RedBlackTree(){}
protected:
    sPointer root;
    sPointer nil;

    /**
     * @brief minimum
     */
    sPointer minimum(sPointer node)
    {
        assert(node != this->nil);

        sPointer tracker = node;
        while(node != this->nil)
        {
            tracker =   node;
            node    =   node->left;
        }

        return tracker;
    }

    /**
     * @brief transplant
     * @param to
     * @param from
     *
     * @note    a bit different from the BST version
     * @complx  O(1)
     * @page    323
     */
    void transplant(sPointer to, sPointer from)
    {
        if(ascend(to,1) == this->nil)
            this->root  =   from;
        else
            (to->is_left()?     ascend(to,1)->left    :   ascend(to,1)->right)
                        =   from;

        from->parent    =   to->parent;
    }

    /**
     * @brief insert_first_part
     * @param added
     *
     * @complx  O(h)
     */
    void insert_first_part(sPointer added)
    {
        sPointer tracker = nil;
        sPointer curr = root;
        while(curr != nil)
        {
            tracker =   curr;
            curr    =   curr->key > added->key?     curr->left  :   curr->right;
        }

        added->parent   =   tracker;
        if(tracker == nil)
            root    =   added;
        else
            (added->key < tracker->key?     tracker->left   :   tracker->right)
                    =   added;

        added->left =   added->right    =   nil;
        added->color=   Color::RED;
    }

    /**
     * @brief insert
     * @param added
     *
     * @page    315
     * O(h)
     */
    virtual void insert(sPointer added)
    {
        insert_first_part(added);
        insert_fixup(added);
    }

    /**
     * @brief left_rotate
     * @param x
     *
     *      [x]                    [y]
     *         \         =>       /
     *          [y]            [x]
     *
     * @complx  o(1)
     *
     * @page    313
     *
     * @attention modified to be virtual for this chapter
     */
    virtual void left_rotate(sPointer x)
    {
        sPointer y = x->right;      //  set y

        x->right    =   y->left;    //  turn y's left into x's right
        if(y->left  !=  this->nil)
            y->left->parent = x;

        y->parent   =   x->parent;  //  link x's parent to y
        if(x->parent.lock()    ==  this->nil)
            this->root  =   y;
        else
            (x->is_left()?  x->parent.lock()->left  :  x->parent.lock()->right)
                        =   y;

        y->left     =   x;
        x->parent   =   y;
    }

    /**
     * @brief right_rotate
     * @param y
     *
     *          [y]             [x]
     *          /        =>        \
     *        [x]                   [y]
     *
     * @complx  O(1)
     */
    virtual void right_rotate(sPointer y)
    {
        sPointer x = y->left;

        y->left = x->right;
        if(x->right != this->nil)
            x->right->parent    =   y;

        x->parent   =   y->parent;
        if(y->parent.lock() ==  this->nil)
            root    =   x;
        else
            (y->is_left()?  y->parent.lock()->left  :  y->parent.lock()->right)
                    =   x;

        x->right    =   y;
        y->parent   =   x;
    }

    /**
     * @brief ascend
     *
     * used for insert_fixup
     */
    sPointer ascend(sPointer node, int level)
    {
        while(level--)
            node = node->parent.lock();
        return node;
    }

    /**
     * @brief insert_fixup
     *
     * @complx  O(1)
     *
     * based on the pyseudocode on Page 316
     */
    virtual void insert_fixup(sPointer added)
    {
        while(ascend(added,1)->color   ==  Color::RED)
        {
            if(ascend(added,1)->is_left())
            {
                sPointer uncle = ascend(added,2)->right;
                if(uncle->color ==  Color::RED)
                {
                    uncle->color            =   Color::BLACK;
                    ascend(added,1)->color  =   Color::BLACK;
                    ascend(added,2)->color  =   Color::RED;
                    added   =   ascend(added,2);
                }
                else
                {
                    if(added->is_right())
                    {
                        added   =   ascend(added,1);
                        left_rotate(added);
                    }
                    ascend(added,1)->color  =   Color::BLACK;
                    ascend(added,2)->color  =   Color::RED;
                    right_rotate(ascend(added,2));
                }
            }
            else
            {
                sPointer uncle = ascend(added,2)->left;
                if(uncle->color ==  Color::RED)
                {
                    uncle->color            =   Color::BLACK;
                    ascend(added,1)->color  =   Color::BLACK;
                    ascend(added,2)->color  =   Color::RED;
                    added   =   ascend(added,2);
                }
                else
                {
                    if(added->is_left())
                    {
                        added   =   ascend(added,1);
                        right_rotate(added);
                    }
                    ascend(added,1)->color  =   Color::BLACK;
                    ascend(added,2)->color  =   Color::RED;
                    left_rotate(ascend(added,2));
                }
            }
        }
        root->color = Color::BLACK;
    }

    /**
     * @brief return the argument's sibling node
     */
    sPointer sibling(sPointer node)
    {
        assert(node != this->root);

        sPointer pnt = node->parent.lock();
        return node->is_left()?     pnt->right  :   pnt->left;
    }

    /**
     * @brief remove_fixup
     * @param x
     *
     * @complx  O(lg n)
     * @page    326
     */
    void remove_fixup(sPointer x)
    {
        while(x != root   &&   x->color == Color::BLACK)
        {
            if(x->is_left())
            {
                sPointer sister = sibling(x);

                //! case 1
                if(sister->color    ==  Color::RED)
                {
                    sister->color       =   Color::BLACK;
                    ascend(x,1)->color  =   Color::RED;
                    left_rotate(ascend(x,1));
                    sister              =   ascend(x,1)->right;
                }

                //! case 2
                if(sister->left->color  ==  Color::BLACK
                        &&
                            sister->right->color  ==  Color::BLACK)
                {
                    sister->color   =   Color::RED;
                    x   =   ascend(x,1);
                }
                else
                {
                    //! case 3
                    if(sister->right->color ==  Color::BLACK)
                    {
                        sister->left->color =   Color::BLACK;
                        sister->color       =   Color::BLACK;
                        right_rotate(sister);
                        sister              =   sibling(x);
                    }

                    //! case 4
                    sister->color           =   ascend(x,1)->color;
                    ascend(x,1)->color      =   Color::BLACK;
                    sister->right->color    =   Color::BLACK;
                    left_rotate(ascend(x,1));
                    x   =   root;
                }
            }
            else
            {
                sPointer sister = sibling(x);

                //! case 1
                if(sister->color    ==  Color::RED)
                {
                    sister->color       =   Color::BLACK;
                    ascend(x,1)->color  =   Color::RED;
                    right_rotate(ascend(x,1));
                    sister              =   ascend(x,1)->left;
                }

                //! case 2
                if(sister->left->color  ==  Color::BLACK
                        &&
                            sister->right->color  ==  Color::BLACK)
                {
                    sister->color   =   Color::RED;
                    x   =   ascend(x,1);
                }

                else
                {
                    //! case 3
                    if(sister->left->color ==  Color::BLACK)
                    {
                        sister->right->color    =   Color::BLACK;
                        sister->color           =   Color::BLACK;
                        left_rotate(sister);
                        sister  =  sibling(x);
                    }

                    //! case 4
                    sister->color           =   ascend(x,1)->color;
                    ascend(x,1)->color      =   Color::BLACK;
                    sister->left->color     =   Color::BLACK;
                    right_rotate(ascend(x,1));
                    x   =   root;
                }
            }
        }
        x->color  =  Color::BLACK;
    }
};

template<typename K, typename D = int>
std::size_t
find_inversions(const std::vector<K>& v);

/**
 * @brief The OrderStatisticTree class
 *
 * inherited from Red Black Tree
 */
template<typename K, typename D>
class OrderStatisticTree : public RedBlackTree<K,D>
{
private:
    using B         =               RedBlackTree<K,D>;
    using KeyType   =   typename    B::KeyType;
    using sPointer  =   typename    B::sPointer;
    using SizeType  =               std::size_t;

public:
    using B::insert;
//! ^^^^^^^^^^^^^^^ -- needed to call this virtual function from base class

    /**
     * @brief select
     *
     * @complx  O(lg n)
     *
     * @page    341
     */
    sPointer select(sPointer target, SizeType rank)
    {
        //! must be within the range.
        assert(rank <= target->size + 1);

        SizeType curr    =   target->rank();
        if(rank == curr)
            return target;
        else
            return rank < curr?
                        select(target->left,rank)
                            :   select(target->right, rank - curr);
    }

    /**
     * @brief select_nonrecur
     *
     * @complx  O(lg n)
     *
     * ex14-1.3
     */
    sPointer select_nonrecur(sPointer target, SizeType rank)
    {
        //! must be within the range.
        assert(rank <= target->size + 1);

        SizeType r = target->rank();
        while(r != rank)
        {
            if(rank < r)
                target  =   target->left;
            else
            {
                target  =   target->right;
                rank    -=  r;
            }
            r   =   target->rank();
        }

        return target;
    }

    /**
     * @brief rank
     *          return the rank within the whole tree
     *
     * @complx  O(lg n)
     *
     * @page    342
     */
    SizeType rank(sPointer target)
    {
        SizeType ret    =   target->rank();
        sPointer curr   =   target;
        while(curr != root)
        {
            if(curr->is_right())
                ret +=  sibling(curr)->size +   1;
            curr    =   curr->parent.lock();
        }

        return ret;
    }

    /**
     * @brief find_rank_with_key
     * @param key
     * @return the rank looked for
     *
     * @complx  O(lg n)
     *
     * as required in ex14-1.4
     */
    SizeType find_rank_with_key(const KeyType& key) const     //动态集合中的秩
    {
        //! define a lambda to do the job recursively
        std::function<SizeType(sPointer)> find_by_recur=
            [&](sPointer curr)
        {
            assert(curr != nil);
            assert(curr != nullptr);

            //! search for the node with the key specified
            //! and count the rank simultaneously.
            if(curr->key == key)
                return curr->rank();
            else
                return curr->key > key?
                            find_by_recur(curr->left)
                            :   find_by_recur(curr->right)  +   curr->rank();

        };

        //! call the lambda
        return find_by_recur(root);
    }

    /**
     * @brief find_ith_successor
     * @param target
     * @param ith
     *
     * @complx  O(lg n)
     *
     * for ex14.1-5
     */
    sPointer find_ith_successor(sPointer target, SizeType ith)
    {

        if(ith  ==  0)
            return target;
        else
        {
            if(ith <= target->right->size)
                return select(target->right,ith);
            else
            {
                ith -=  target->right->size;

                sPointer trackor;
                do{
                    trackor =   target;
                    target  =   target->parent.lock();
                }while(trackor->is_right());
                --ith;

                return find_ith_successor(target,ith);
            }
        }
    }


    //! Dtor
    virtual ~OrderStatisticTree(){  }

private:
    using B::root;
    using B::nil;
    using B::ascend;
    using B::sibling;
    using B::insert_fixup;

    /**
     * @brief left_rotate
     * @param x
     *
     * @attention   virtual!
     *
     * @complx  O(1)
     *
     * based on the pseudocode P343
     */
    virtual void left_rotate(sPointer x) override
    {
        B::left_rotate(x);

        sPointer parent =   x->parent.lock();
        parent->size    =   x->size;
        x->size         =   x->left->size   +   x->right->size  +   1;
    }

    /**
     * @brief right_rotate
     * @param y
     *
     * @attention   virtual!
     *
     * @complx  O(1)
     */
    virtual void right_rotate(sPointer y) override
    {
        B::right_rotate(y);

        sPointer parent =   y->parent.lock();
        parent->size    =   y->size;
        y->size         =   y->left->size   +   y->right->size  +   1;
    }

    /**
     * @brief insert
     * @param added
     *
     * @attention   virtual!
     *
     * @complx  O(h)
     */
    virtual void insert(sPointer added) override
    {
        added->size =   1;  //The new node added gets a size of 1.(P343)

        sPointer tracker = nil;
        sPointer curr = root;
        while(curr != nil)
        {
            ++curr->size;
       //!  ^^^^^^^^^^^^^   --  added for o s tree

            tracker =   curr;
            curr    =   curr->key > added->key?     curr->left  :   curr->right;
        }

        added->parent   =   tracker;
        if(tracker == nil)
            root    =   added;
        else
            (added->key < tracker->key?     tracker->left   :   tracker->right)
                    =   added;

        added->left =   added->right    =   nil;
        added->color=   Color::RED;

        //! fixup
        insert_fixup(added);
    }
};

/**
 * @brief find_inversions
 * @param vector
 *
 * @complx  O(n lg n)
 *
 * for ex14-1.7
 */
template<typename K, typename D>
inline std::size_t
find_inversions(const std::vector<K>& v)                        //查找逆序对
{
    //! types def and initialization
    using OsTree    =   OrderStatisticTree<K,D>;
    using Tree      =   std::shared_ptr<OsTree>;
    Tree tree = std::make_shared<OsTree>();
    std::size_t count, inversions;
    count   =   inversions  =   0;

    //! counting inversions
    for(const auto& key :   v)                                  //  O(n)
    {
        tree->insert(key);
        std::size_t rank    =   tree->find_rank_with_key(key);  //  O(lg n)
        inversions  +=  ++count - rank;
    }

    return inversions;
}


int main()
{
   auto tree = new OrderStatisticTree<int, std::string>;
   std::vector<int> v = {11,22,33,44,55,66,77,88};
   for(auto i : v)
      tree->insert(i);
   tree->print();

   std::cout << debug::red("testing select_nonrecur:\n");
   auto node   =   tree->search(44);
   auto ret    =   tree->select_nonrecur(node,7);
   ret->print();

   delete tree;
   std::cout << debug::green("\nend\n");
   return 0;
}

