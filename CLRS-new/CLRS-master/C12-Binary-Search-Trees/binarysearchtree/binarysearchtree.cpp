#include <stack>
#include <functional>
#include <memory>
#include <iostream>

template<typename K, typename D>
struct node
{
    using KeyType   =   K;
    using DataType  =   D;
    using Node      =   node<K,D>;
    using sPointer  =   std::shared_ptr<Node>;
    using wPointer  =   std::weak_ptr<Node>;

    /**
     * @brief default ctor
     */
    node() = default;

    /**
     * @brief Ctor
     * @param key
     */
    explicit node(const KeyType& key):
        data(), key(key), parent(), left(), right(), successor()
    {}

    /**
     * @brief Ctor
     * @param key
     * @param data
     */
    node(const KeyType& key, const DataType& data):
        data(data), key(key), parent(), left(), right(), successor()
    {}

    /**
     * @brief print
     */
    void print() const
    {
        std::cout   << "key = "   <<  key
                    << "  data = "<<  data
                    << std::endl;
    }

    /**
     * @brief data  member
     */
    DataType    data;
    KeyType     key;
    wPointer    parent;
    sPointer    left;
    sPointer    right;

    wPointer    successor;
};

template<typename K, typename D>
class binary_search_tree
{
public:
    using KeyType   =   K;
    using DataType  =   D;
    using Node      =   node<K,D>;
    using sPointer  =   std::shared_ptr<Node>;
    using wPointer  =   std::weak_ptr<Node>;

    /**
     * @brief insert
     * @param key
     * @param data
     *
     * just an interface
     */
    void insert(const KeyType& key, const DataType& data)
    {
        sPointer inserted = std::make_shared<Node>(key, data);
        insert(inserted);
    }

    /**
     * @brief insert
     * @param inserted  i.e. the new node
     *
     * @complexity  O(lg n)
     */
    void insert(sPointer inserted)
    {
        sPointer last = nullptr;
        sPointer curr = root;

        //! find the appropiate position
        while(curr)
        {
            last = curr;
            curr = (inserted->key < curr->key)?  curr->left  :  curr->right;
        }

        //! graft on the inserted.
        inserted->parent    =   last;
        if(!last)
            root = inserted;
        else if(inserted->key   <   last->key)
            last->left      =   inserted;
        else
            last->right     =   inserted;
    }

    /**
     * @brief insert_recur
     * @param key
     * @param data
     *
     * just an interface    for ex12.3-1
     */
    void insert_recur(const KeyType& key, const DataType& data)
    {
        sPointer inserted   =   std::make_shared<Node>(key, data);
        insert_recur(inserted);
    }

    /**
     * @brief insert_recur
     * @param inserted  i.e. the new node
     *
     * @complexity  O(h) = O(lg n)
     *
     * for ex12.3-1
     */
    void insert_recur(sPointer inserted)
    {
        KeyType key = inserted->key;

        //! recursion lambda for finding parent
        std::function<sPointer(sPointer, sPointer)> find =
                [key, &find](sPointer node, sPointer tracker)->sPointer
        {
            if(!node)
                return tracker;
            else
            {
                sPointer next = key < node->key?  node->left  :  node->right;
                return find(next, node);
            }
        };

        //! graft on the inserted.
        sPointer parent;
        inserted->parent    =   parent  =   find(root, nullptr);
        if(!parent)
            root    =   inserted;
        else
            (inserted->key < parent->key?   parent->left    :   parent->right)
                    =   inserted;
    }

    /**
     * @brief inorder_print
     *
     * i.e. the interface for inorder_print.
     */
    void inorder_print()const
    {
        inorder_tree_walk(root);
    }

    /**
     * @brief inorder_print_nonrecur
     *
     * for ex12.1-3
     */
    void inorder_print_nonrecur()const
    {
        sPointer prev = nullptr;
        sPointer curr = root;
        while(curr)
        {
            //! print
            if(!curr->left  ||  prev==curr->left)
                curr->print();

            //! move curr to next node.
            if(prev == curr->parent.lock())
            {
                prev    =   curr;
                curr    =   curr->left?    curr->left   :
                            curr->right?   curr->right  :   curr->parent.lock();
            }
            else if(prev == curr->left && curr->right)
            {
                prev    =   curr;
                curr    =   curr->right;
            }
            else
            {
                prev    =   curr;
                curr    =   curr->parent.lock();
            }
        }
    }

    /**
     * @brief inoder_print_nonrecur_with_stack
     *
     * i.e. the interface for inorder_tree_walk_nonrecur_with_stack
     */
    void inoder_print_nonrecur_with_stack()const
    {
        inorder_tree_walk_nonrecur_with_stack(root);
    }

    /**
     * @brief preorder_print
     */
    void preorder_print()const
    {
        preorder_tree_walk(root);
    }

    /**
     * @brief postorder_print
     */
    void postorder_print()const
    {
        postorder_tree_walk(root);
    }

    /**
     * @brief search_iterative
     * @param key
     *
     * interface
     */
    sPointer search_recur(const KeyType& key) const
    {
        return search_recur(root, key);
    }

    /**
     * @brief search_itera
     * @param key
     *
     * interface
     */
    sPointer search_itera(const KeyType& key) const
    {
        return search_itera(root, key);
    }

    /**
     * @brief maximum_itera
     * @param node
     *
     * @complexity  O(h)=O(lg n)
     */
    sPointer maximum_itera(sPointer node) const
    {
        while(node->right)
            node    =   node->right;
        return node;
    }

    /**
     * @brief maximum_recur
     * @param node
     *
     * @complexity  O(h)=O(lg n)
     *
     * for ex12.2-2
     */
    sPointer maximum_recur(sPointer node) const
    {
        std::function<sPointer(sPointer)> maximum =

                [&maximum](sPointer node)->sPointer
        {
            if(node->right)
                return  maximum(node->right);
            else
                return  node;
        };

        return maximum(node);
    }

    /**
     * @brief mimimum_itera
     * @param node
     *
     * @complexity  O(h)=O(lg n)
     */
    sPointer minimum_itera(sPointer node) const
    {
        while(node->left)
            node    =   node->left;
        return node;
    }


    /**
     * @brief minimum_recur
     * @param node
     *
     * @complexity  O(h)=O(lg n)
     *
     * for ex12.2-2
     */
    sPointer minimum_recur(sPointer node) const
    {
        std::function<sPointer(sPointer)> minimum =
                [&minimum](sPointer node)->sPointer
        {
            if(node->left)
                return minimum(node->left);
            else
                return node;
        };

        return minimum(node);
    }

    /**
     * @brief successor
     * @param node
     *
     * @complexity  O(h)=O(lg n)
     *
     * for ex12.2-3
     */
    sPointer successor(sPointer node) const
    {
        if(node->right)     // case 1 : find it by going upward.
            return minimum_itera(node->right);
        else                // case 2 : find it by going downward.
        {
            sPointer up = node->parent.lock();
            while(up  &&  node == up->right)
            {
                node    =   up;
                up      =   up->parent.lock();
            }
            return up;
        }
    }

    /**
     * @brief predecessor
     * @param node
     * @return
     */
    sPointer predecessor(sPointer node) const
    {
        if(node->left)
            return maximum_itera(node->left);
        else
        {
            sPointer up = node->parent.lock();
            while(up    &&  node == up->left)
            {
                node    =   up;
                up      =   up->parent.lock();
            }
            return up;
        }
    }

    /**
     * @brief remove
     * @param target
     *
     * i.e. the delete() method
     */
    void remove(sPointer target)
    {
        if(!target->left)
            transplant(target, target->right);
        else if(! target->right)
            transplant(target, target->left);
        else
        {
            //! use successor as replacer
            sPointer replacer = minimum_itera(target->right);
            if(replacer->parent.lock() != target)
            {
                transplant(replacer, replacer->right);
                replacer->right         =   target->right;
                replacer->right->parent =   replacer;
            }
            transplant(target, replacer);
            replacer->left          =   target->left;
            replacer->left->parent  =   replacer;
        }
    }

    /**
     * @brief remove
     * @param key
     */
    void remove(const KeyType& key)
    {
        remove(search_itera(key));
    }
private:
    sPointer root;

    /**
     * @brief inorder_tree_walk
     * @param node
     *
     * @complexity  O(n)
     */
    void inorder_tree_walk(sPointer node)const
    {
        if(node)
        {
            inorder_tree_walk(node->left);
            node->print();
            inorder_tree_walk(node->right);
        }
    }

    /**
     * @brief inorder_tree_walk_nonrecur_with_stack
     * @param node
     *
     * for ex 12-1.2
     */
    void inorder_tree_walk_nonrecur_with_stack(sPointer node)const
    {
        if(node)
        {
            std::stack<sPointer> stk;
            while(node  ||  !stk.empty())
            {
                while(node)
                {
                    stk.push(node);
                    node    =   node->left;
                }
                if(!stk.empty())
                {
                    node    =   stk.top();
                    stk.pop();
                    node->print();
                    node    =   node->right;
                }
            }
        }
    }

    /**
     * @brief preorder_tree_walk
     */
    void preorder_tree_walk(sPointer node) const
    {
        if(node)
        {
            node->print();
            inorder_tree_walk(node->left);
            inorder_tree_walk(node->right);
        }
    }

    /**
     * @brief postorder_tree_walk
     */
    void postorder_tree_walk(sPointer node)const
    {
        if(node)
        {
            inorder_tree_walk(node->left);
            inorder_tree_walk(node->right);
            node->print();
        }
    }

    /**
     * @brief search_iterative
     * @param node
     * @param key
     *
     * @complexity  O(h)    =   O(lg n)
     */
    sPointer search_recur(sPointer node, const KeyType key) const
    {
        if(!node  ||  key == node->key)
            return  node;

        sPointer next   =   (key < node->key)?  node->left  :   node->right;
        return  search_recur(next, key);
    }

    /**
     * @brief search_itera
     * @param node
     * @param key
     *
     * @complexity  O(h)    =   O(lg n)
     */
    sPointer search_itera(sPointer node, const KeyType& key) const
    {
        while(node  &&  key != node->key)
            node    =   key < node->key?    node->left  :   node->right;
        return node;
    }

    /**
     * @brief transplant
     * @param from
     * @param to
     *
     * @complexity  O(1)
     *
     * used for delete()
     */
    void transplant(sPointer to, sPointer from)
    {
        sPointer parent  =   to->parent.lock();
        if(!parent)
            root    =   from;
        else
            (to == parent->left?    parent->left    :   parent->right)
                    =   from;
        if(from)
            from->parent    =   to->parent;

    }
};//class binary_search_tree

int main(int argc, char const *argv[])
{
   binary_search_tree<int, std::string> tree; 
   tree.insert(1,"001");
   tree.insert(2,"002");
   tree.insert(5,"005");
   tree.insert(199,"199");
   tree.insert(6,"006");
   tree.insert(123,"123");
   tree.insert(127,"127");
   tree.insert(666,"666");
   tree.insert(999,"99");
   tree.insert(23,"23");

   tree.inorder_print();

   tree.preorder_print();
   tree.postorder_print();

    return 0;
}
