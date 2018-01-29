#include <functional>
#include <vector>
#include <iostream>
#include <memory>

template<typename K, typename D>
struct node
{
    using KeyType   =   K;
    using DataType  =   D;
    using sPointer  =   std::shared_ptr<node<K, D>>;     //shared_ptr多个对象可以使用一个指针,use_count方法统计使用的个数
    using wPointer  =   std::weak_ptr<node<K, D>>;       //weak_ptr只能一个对象单独使用

    /**
     * @brief ctor
     */
    explicit node(const KeyType& k, const DataType d):
        key(k), data(d), prev(), next(nullptr)
    {}

    /**
     * @brief members
     */
    KeyType     key;
    DataType    data;
    wPointer    prev;
    sPointer    next;
};

template<typename K, typename D>
class hash_table
{
public:
    using SizeType  =   K;
    using KeyType   =   K;
    using DataType  =   D;
    using Node      =   node<K,D>;
    using sPointer  =   std::shared_ptr<Node>;
    using wPointer  =   std::weak_ptr<Node>;
    using Hash      =   std::function<K(K)>;
    using Container =   std::vector<sPointer>;

    hash_table()    =   delete;

    /**
     * @brief Ctor
     */
    hash_table(const SizeType& sz, const Hash& h):
        hash(h), container(sz,sPointer())
    {}

    /**
     * @brief insert
     * @param key
     * @param data
     * @complexity  O(1)
     *
     * refer to CLRS 11.2 for compexity proof.
     */
    void insert(const KeyType& key, const DataType& data)
    {
        sPointer&   head        =   container[hash(key)];
        sPointer    inserted    =   std::make_shared<Node>(key, data);   //利用链接法解决散列表的冲突问题
        if(!head)
            head    =   inserted;
        else
        {
            inserted->next  =   head;          
            head->prev      =   inserted;    
            head            =   inserted;
        }
    }

    /**
     * @brief search
     * @param key
     * @return shared pointer
     *
     * @complexity  O(1)
     * refer to CLRS 11.2 for compexity proof.
     */
    sPointer search(const KeyType key)
    {
        sPointer ret = nullptr;
        sPointer curr = container[hash(key)];
        while(curr)
        {
            if(curr->key    ==   key)
            {
                ret = curr;
                break;
            }
        }
        return ret;
    }

    /**
     * @brief erase
     *
     * @complexity  O(1)
     *
     * refer to figure 11.3 on page 257 for all 4 cases
     */
    void erase(sPointer target)
    {
        sPointer&   head = container[hash(target->key)];
        sPointer&   next = target->next;
        wPointer&   prev = target->prev;

        if(!next && !prev.lock())       //only one node
            head    =   nullptr;
        else if(next && !prev.lock())   //the first one of multiple nodes
        {
            next->prev.reset();
            head    =   next;
        }
        else if (!next && prev.lock())  //the last one of multiple nodes
            prev.lock()->next   =   nullptr;
        else                            //the middle one of multiple nodes
        {
            next->prev  =   prev;
            prev.lock()->next   =   next;
        }
    }

    /**
     * @brief print
     *
     * @complexity  O(n)
     */
    void print() const
    {
        for(const auto& elem : container)
        {
            sPointer curr = elem;
            while(curr)
            {
                std::cout << "key=" << curr->key
                          << " data=" << curr->data
                          << std::endl;

                curr = curr->next;
            }
        }
    }

private:
    Hash        hash;
    Container   container;           //hash_table底层用vector实现的

};

int main(int argc, char const *argv[])
{
    auto hash = [](const int key)->int {
       return key % 9; };
    hash_table<int, std::string> table(10, hash);

    table.insert(6,"first try");
    table.insert(88,"try 2");
    table.insert(101,"try");
    table.insert(9,"nine");
    table.insert(18,"eghiteen");    //出现了冲突
    table.print();

    table.erase(table.search(101));
    table.erase(table.search(6));
    table.erase(table.search(88));
    std::cout << "erase" << std::endl;

    table.print();

    std::cout << "end" << std::endl;

    return 0;
}
