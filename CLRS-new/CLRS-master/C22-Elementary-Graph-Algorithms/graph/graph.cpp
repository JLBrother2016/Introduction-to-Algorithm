#include <string>
#include <limits>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <map>

using std::vector;
using std::pair;
using std::string;
using std::list;
using std::find_if;
using std::ostream;
using std::endl;
using std::to_string;
using std::map;

namespace ch22 {
using std::string;
enum {White, Gray, Black};
/**
 * @brief Vertex
 */
template <typename Key, typename Data>
struct Vertex
{
    Vertex() = default;

    Vertex(int c, long long d, const Vertex* p, Key k, Data dt = Data{}):
        color_{c},distance_{d},parent_{p},key_{k}, data_{dt}
    {}

    explicit Vertex(Key k) :
        Vertex{White, std::numeric_limits<long long>::max(), nullptr, k, Data{}}
    {}

    string str() const
    {
        return "[" + std::to_string(key_) + "]";
    }

    int color_ {White};
    long long distance_ {std::numeric_limits<long long>::max()};
    const Vertex* parent_{0};
    const Key key_{0};
    Data data_{};
};

template<typename K, typename D>
inline bool
operator <(Vertex<K,D> const& lhs, Vertex<K,D> const& rhs)
{
    return lhs.key_ < rhs.key_;
}

template<typename K, typename D>
inline bool
operator ==(Vertex<K,D> const& lhs, Vertex<K,D> const& rhs)
{
    return lhs.key_ == rhs.key_;
}

template<typename K,typename D>
struct Edge
{
    Vertex<K,D> u_,v_;
};
}

namespace ch22 {
template<typename Key,typename Data>
class AdjList;
template<typename Key,typename Data>
ostream& operator<<(ostream& os, AdjList<Key, Data> const&);
/**
 * @brief Adjacency list
 * @note abstract base class for both Directed and Undirected Graph
 *
 * @concepts:
 *      Container::push_back()
 */
template<typename Key,typename Data>
class AdjList
{
    friend ostream&
    operator<< <Key, Data> (ostream& os, AdjList<Key, Data> const&);

    using V     =   Vertex<Key,Data>;
    using E     =   Edge<Key,Data>;
    struct List
    {
        void add(Key k){    neighbours_.push_back(k);   }
        V vertex_;
        vector<Key> neighbours_;
    };
    using Adj   =   vector<List>;

public:

    using SizeType  =   typename Adj::size_type;
    using Iter      =   typename Adj::iterator;
    using ConstIter =   typename Adj::const_iterator;

    AdjList() = default;

    void add_vertex(V const& v)
    {
        if(end() == find(v))
            adj_.push_back(List{v, {}});
    }

    void add_edge(E const& edge)
    {
        do_add_edge(edge);
    }

    Iter find(V const& v)
    {
        return find_if(adj_.begin(), adj_.end(),[&v](List const& li){
            return  li.vertex_ == v;
        });
    }

    ConstIter   begin()     const   {   return adj_.cbegin();   }
    ConstIter   end()       const   {   return adj_.cend();     }

    Iter        begin()             {   return adj_.begin();    }
    Iter        end()               {   return adj_.end();      }

    SizeType    size()      const   {   return adj_.size();     }
    bool        empty()     const   {   return adj_.empty();    }

    virtual ~AdjList(){}
protected:
    Adj adj_;
private:
     virtual void do_add_edge(E const&) = 0;
};
template<typename Key,typename Data>
ostream& operator<<(ostream& os, AdjList<Key, Data> const& g)
{
    for(auto const& li : g)
    {
        auto ver = li.vertex_.str();
        string line(10, '-');
        for(unsigned i=0; i!=ver.size(); ++i)   line[i] = ver[i];
        line += "{";
        for(auto const& key : li.neighbours_)   line += to_string(key) + ",";
        if(! li.neighbours_.empty())
            line.back() = '}';
        else
            line += "}";
        os << line << endl;
    }
    return os;
}
/**
 * @brief Directed Graph
 *
 * Adjacency list representation.
 */
template<typename Key, typename Data>
class DirectedGraph : public AdjList<Key, Data>
{
    using Super = AdjList<Key,Data>;
    virtual void do_add_edge(Edge<Key,Data> const& edge) override
    {
        Super::add_vertex(edge.u_);
        Super::add_vertex(edge.v_);
        Super::find(edge.u_)->add(edge.v_.key_);
    }

public:
    using Table = map<Key, typename Super::SizeType>;

    DirectedGraph() = default;
    template<typename Iter>
    DirectedGraph(Iter first, Iter last) : Super{}
    {
        for(auto it = first; it != last; ++it)    Super::add_edge(*it);
    }

    Table outdegree() const
    {
        Table ret;
        for(auto const& li : *this)
            ret[li.vertex_.key_] = li.neighbours_.size();
        return ret;
    }

    Table indegre() const
    {
        Table ret;
        for(auto const& li : *this)
            ret[li.vertex_.key_] = 0;
        for(auto const& li : *this)
            for(auto const& key : li.neighbours_)
                ++ret[key];
        return ret;
    }
};
/**
 * @brief The UndirectedGraph class
 *
 * Adjacency list representation.
 */
template<typename Key, typename Data>
class UndirectedGraph : public AdjList<Key, Data>
{
    using Super = AdjList<Key,Data>;
    virtual void do_add_edge(Edge<Key,Data> const& edge) override
    {
        Super::add_vertex(edge.u_);
        Super::add_vertex(edge.v_);
        Super::find(edge.u_)->add(edge.v_.key_);
        Super::find(edge.v_)->add(edge.u_.key_);
    }

public:
    UndirectedGraph() = default;
    template<typename Iter>
    UndirectedGraph(Iter first, Iter last) : Super{}
    {
        for(auto it = first; it != last; ++it)    Super::add_edge(*it);
    }
};
}

using namespace std;
using V  = ch22::Vertex<int,std::string>;
using E  = ch22::Edge<int,std::string>;
using DG = ch22::DirectedGraph<int,std::string>;
using UG = ch22::UndirectedGraph<int,std::string>;

/**
 * @brief operator << for degree table
 */
inline ostream& operator<<(ostream& os, typename DG::Table const& table)
{
    for(auto const& pair : table)
        os << pair.first << " : " << pair.second << endl;
    return os;
}


/**
 * @brief make 3 edges
 *          {1  -> 2, 1  -> 5, 2  -> 5}    seen by directed    graph
 *          {1 <-> 2, 1 <-> 5, 2 <-> 5}    seen by unddirected graph
 * @return
 */
vector<E> make_edges()
{
    V v1{1}, v2{2}, v5{5};
    E e12{v1, v2}, e15{v1, v5}, e25{v2, v5};
    return {e12, e15, e25};
}

int main()
{
    vector<E> es = make_edges();
    cout << "testing Adjacency List \n\n";

    DG dg(es.cbegin(), es.cend());
    cout << "Directed Graph :\n";
    cout << "empty() = " << dg.empty()      << endl;
    cout << "size()  = " << dg.size()       << endl;
    cout << (dg.find(V{1}) != dg.end() ? "found" : "no such thing") << endl;
    cout << dg;
    cout << "deg+ : \n" << dg.outdegree()   << endl;
    cout << "deg- : \n" << dg.indegre()     << endl;


    cout << "-------------------\n\n";


    UG ug(es.cbegin(),es.cend());
    cout << "Undirected Graph :\n";
    cout << "empty() = " << ug.empty()      << endl;
    cout << "size()  = " << ug.size()       << endl;
    cout << (ug.find(V{1}) != ug.end() ? "found" : "no such thing") << endl;
    cout << ug ;

    return 0;
}

