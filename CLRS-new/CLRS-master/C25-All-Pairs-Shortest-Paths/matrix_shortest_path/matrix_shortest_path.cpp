#include<memory>
#include<array>
#include<algorithm>
#include<vector>
#include<ostream>


//!unlimit：返回正无穷的函数
/*!
*\return : 正无穷大的数
*
* 将本函数的返回值定义为正无穷。在算法导论图算法中，经常用到正无穷。通常对正无穷的操作是:
*
* - 将边的权重或者节点的`key`设为正无穷
* - 对正无穷加、减一个有限的数，结果还是正无穷
*
* 这里将`std::numeric_limits<T>::max()/2`设为正无穷，考虑到正无穷加上一个较大的数必须保证不能溢出
*
*/
template<typename T> T unlimit()
{
    return std::numeric_limits<T>::max()/2;
}


//!extend_path：扩展一条边，算法导论25章25.1节
/*!
* \param L:初始L矩阵
* \param W: 图的权重矩阵
* \return: 扩展之后的L矩阵
*
* 该函数作为 matrix_shortest_path 的辅助函数
* >这里要求 MatrixType是一个n*n的矩阵。如果MatrixType不是一个n*n的矩阵，则编译失败
*
* 算法步骤如下：
*
* - 外层循环 i 从 0...N-1(N次)
*   - 内层循环 j 从 0...N-1(N次)
*       - 将newL[i][j]设为正无穷，对所有的k,k 从 0...N-1(N次)，选取 L[i][k]+W[k][j]的最小值赋值给newL[i][j]
* - 最终返回 newL
*
* 性能：时间复杂度 O(n^3)
*
*/
template<typename MatrixType> MatrixType extend_path(const MatrixType&L,const MatrixType&W)
{
    typedef typename MatrixType::value_type::value_type ValueType;

    static_assert(std::tuple_size<MatrixType>::value==std::tuple_size<typename MatrixType::value_type>::value,"extend_path error:matrix must be N*N size!"); //必须是方阵
    std::size_t row_num=std::tuple_size<MatrixType>::value;
    std::size_t col_num=std::tuple_size<typename MatrixType::value_type>::value;

    MatrixType newL;
    for(int i=0;i<row_num;i++)
        for(int j=0;j<col_num;j++)
        {
            newL[i][j]=unlimit<ValueType>();
            for(int k=0;k<row_num;k++)
            {
                newL[i][j]=std::min(newL[i][j],L[i][k]+W[k][j]);
            }
        }
    return newL;
}



//!matrix_shortest_path：返回所有节点对的最短路径的矩阵乘法算法。算法导论25章25.1节
/*!
*
* \param graph:指定的有向图。它必须非空，否则抛出异常
* \return: 一个n*n的矩阵(d_i_j)，其中 d_i_j 代表的是结点i到j的一条最短路径的权重
*
* ## 所有结点对的最短路径
*
* 给定一个带权重的有向图G=(V,E)，其权重函数为w:E->R，该函数将边映射到实值权重上。我们希望找到对于所有的结点对u,v属于V，找出一条从结点u
* 到结点v的最短路径，以及这条路径的权重。
*
* 与单源最短路径不同中使用邻接表来表示图不同，本章的多数算法使用邻接矩阵来表示图。该矩阵代表的是一个有n个结点的有向图G=(V,E)的边的权重W=(w_i_j)，
* 其中 w_i_j =:
*
*   - 0:若i=j
*   - 有向边(i,j)的权重，若i!=j且(i,j)属于E
*   - 正无穷，若 i!=j且(i,j)不属于E
*
* 我们允许存在负权重的边，目前仍然假定图中不存在权重为负值的环路。
*
* 本章讨论的所有结点对最短路径的算法的输出也是一个n*n的矩阵D=(d_i_j)，其中 d_i_j 代表的是结点i到j的一条最短路径的权重。
*
* 有时候为了解决所有结点对最短路径问题，我们不仅要计算出最短路径权重，还需要计算出前驱结点矩阵 II=(pai_i_j)，其中 pai_i_j在i=j或者从i到j
* 不存在路径时为NIL，在其他情况下给出的是从结点i到结点j的某条最短路径上结点j的前驱结点。由矩阵II的第i行所诱导的子图应当是一棵根节点为i
* 的最短路径树。
*
*
* ## matrix_shortest_path 算法
*
* ### 算法原理
*
* matrix_shortest_path采用动态规划算法求解。考虑从结点i到j的一条最短路径p，假定p最多包含m条边，假定没有权重为负值的环路，且m为有限值。
* 如果 i=j，则p的权重为0且不包含任何边；如果i和j不同，则将路径p分解为 i-->k(经过路径p')-->j，其中路径p'最多包含m-1条边。
*
* 定义 l_i_j<m>为从结点i到j的最多包含m条边的任意路径中的最小权重，则有：l_i_j<m>=
*
* - 0：如果i=j
* - 正无穷:如果 i!=j
*
* 对于m>=1，我们有： l_i_j<m>=min(l_i_j<m-1>，min_(1<=k<=n){l_i_k<m-1>+w_k_j})=min_(1<=k<=n){l_i_k<m-1>+w_k_j}。
* 如果图G不包含负值的环路，则对于每一对结点i,j，如果他们delt(i,j)<正无穷，则从i到j之间存在一条最短路径。由于该路径是简单路径，
* 则包含的边最多为n-1条。因此delt(i,j)=l_i_j<n-1>=l_i_j<n>=...
*
* matrix_shortest_path算法根据输入矩阵W=(w_i_j)，计算出矩阵序列 L<1>，L<2>,...L<n-1>。最后的矩阵L<n-1>包含的是最短路径的权重。
* 其中L<1>=W。算法的核心是extend_path函数，它将最近计算出的路径扩展了一条边
*
*
* ### 算法步骤
*
* - 初始化：从图中获取权重矩阵 W
* - 执行循环扩展L，其中 L<0>=W, L<k>=extend_path(L<k-1>,W)
* - 最终返回 L<N-1>
*
* ### 算法性能
*
* 时间复杂度O(V^4)
*/
template<typename GraphType> std::array<std::array<typename GraphType::EWeightType ,GraphType::NUM>,GraphType::NUM>
    matrix_shortest_path(std::shared_ptr<GraphType> graph)
{
    typedef typename GraphType::EWeightType EWeightType;
    typedef std::array<std::array<EWeightType,GraphType::NUM>,GraphType::NUM> MatrixType;

    if(!graph)
        throw std::invalid_argument("matrix_shortest_path error: graph must not be nullptr!");
    MatrixType W;
    //**************  从图中创建权重矩阵  ***************
    //****  这里不能直接从图的矩阵描述中提取，因为这里要求 w(i,i)=0，而图的矩阵描述中，结点可能有指向自己的边
    for(int i=0;i<GraphType::NUM;i++)
        for(int j=0;j<GraphType::NUM;j++)
        {
            if(i==j) W[i][j]=0;
            else
            {
                if(!graph->has_edge(i,j))
                    W[i][j]=unlimit<EWeightType>();
                else
                    W[i][j]=graph->weight(i,j);
            }
         }
    //*********  计算 L <n-1> ***********
    MatrixType L=W; //L<0>
    for(int i=0;i<GraphType::NUM-2;i++) //扩展 N-2次
    {
        L=extend_path(L,W);
    }
    return L;
}

//!matrix_shortest_path：返回所有节点对的最短路径的矩阵乘法复平方算法。算法导论25章25.1节
/*!
*
* \param graph:指定的有向图。它必须非空，否则抛出异常
* \return: 一个n*n的矩阵(d_i_j)，其中 d_i_j 代表的是结点i到j的一条最短路径的权重
*
* ## 所有结点对的最短路径
*
* 给定一个带权重的有向图G=(V,E)，其权重函数为w:E->R，该函数将边映射到实值权重上。我们希望找到对于所有的结点对u,v属于V，找出一条从结点u
* 到结点v的最短路径，以及这条路径的权重。
*
* 与单源最短路径不同中使用邻接表来表示图不同，本章的多数算法使用邻接矩阵来表示图。该矩阵代表的是一个有n个结点的有向图G=(V,E)的边的权重W=(w_i_j)，
* 其中 w_i_j =:
*
*   - 0:若i=j
*   - 有向边(i,j)的权重，若i!=j且(i,j)属于E
*   - 正无穷，若 i!=j且(i,j)不属于E
*
* 我们允许存在负权重的边，目前仍然假定图中不存在权重为负值的环路。
*
* 本章讨论的所有结点对最短路径的算法的输出也是一个n*n的矩阵D=(d_i_j)，其中 d_i_j 代表的是结点i到j的一条最短路径的权重。
*
* 有时候为了解决所有结点对最短路径问题，我们不仅要计算出最短路径权重，还需要计算出前驱结点矩阵 II=(pai_i_j)，其中 pai_i_j在i=j或者从i到j
* 不存在路径时为NIL，在其他情况下给出的是从结点i到结点j的某条最短路径上结点j的前驱结点。由矩阵II的第i行所诱导的子图应当是一棵根节点为i
* 的最短路径树。
*
* ##  matrix_shortest_path_fast 算法
*
* ### 算法原理
*
* matrix_shortest_path_fast改进了算法matrix_shortest_path。因为我们的目标并不是计算所有的L<m>矩阵，我们感兴趣的仅仅是矩阵L<n-1>。
* 由matrix_shortest_path过程定义的矩阵乘法是相关的，因此可以仅用t个矩阵乘积来计算矩阵L<n-1>，其中 n为大于lg(n-1)的最小整数。
* 因此可以用复平方技术来计算上述矩阵序列：
*
* - L<1>=W
* - L<2>=W^2=W.W
* - L<4>=W^4=W^2.W^2
* - L<8>=W^8=W^4.W^4
* ....
*
*
* ### 算法步骤
*
* - 初始化：从图中获取权重矩阵 W
* - 执行循环扩展L，其中 L<0>=W, L<2*k>=extend_path(L<k>,L<k>)
* - 最终返回 L<log(N-1)的上界整数>
*
* ### 算法性能
* 时间复杂度O(V^3lgV)
*/
template<typename GraphType> std::array<std::array<typename GraphType::EWeightType ,GraphType::NUM>,GraphType::NUM>
    matrix_shortest_path_fast(std::shared_ptr<GraphType> graph)
{
    typedef typename GraphType::EWeightType EWeightType;
    typedef std::array<std::array<EWeightType,GraphType::NUM>,GraphType::NUM> MatrixType;

    if(!graph)
        throw std::invalid_argument("matrix_shortest_path_fast error: graph must not be nullptr!");

    MatrixType W;
    //**************  从图中创建权重矩阵  ***************
    //****  这里不能直接从图的矩阵描述中提取，因为这里要求 w(i,i)=0，而图的矩阵描述中，结点可能有指向自己的边
    for(int i=0;i<GraphType::NUM;i++)
        for(int j=0;j<GraphType::NUM;j++)
        {
            if(i==j) W[i][j]=0;
            else
            {
                if(!graph->has_edge(i,j))
                    W[i][j]=unlimit<EWeightType>();
                else
                    W[i][j]=graph->weight(i,j);
            }
         }
    //*********  计算 L <n-1> ***********
    MatrixType L=W;
    int m=1;
    while(m<GraphType::NUM-1)
    {
        L=extend_path(L,L);
        m=m<<1;
    }
    return L;
}

//!Vertex：图的顶点，算法导论22章22.1节
/*!
*
* 顶点是一个结构体的模板，模板类型为它存储的数据的类型。它主要包含两个数据：
*
*   - `key`:顶点存放的数据
*   - `id`:顶点的编号（从0开始，负编号的节点为无效节点）。它是一个`const int`，一旦顶点初始化完毕就不可更改。
*
* 默认情况下：`id`为-1,`key`为`T()`。
*
*/
template<typename KType> struct Vertex
{
    typedef KType KeyType;    /*!< 顶点存储数据的类型*/
    typedef int VIDType;      /*!< 顶点编号的数据类型*/

    //!默认构造函数，将`key`设为`KType()`，编号设为-1
    Vertex():key(KeyType()),id(-1){}
    //!显式构造函数，指定`key`
    /*!
    * \param  k:顶点存放的数据
    */
    explicit Vertex(const KeyType&k):key(k),id(-1){}
    //!显式构造函数，指定`key`和编号
    /*!
    * \param  k:顶点存放的数据
    * \param  d:顶点的编号
    */
    Vertex(const KeyType&k,VIDType d):key(k),id(d){}

    //!to_string：返回顶点的字符串描述
    /*!
    * \return :顶点的字符串描述
    *
    * 依次输出顶点的`id`、`key`信息。
    */
    virtual std::string to_string() const
    {
        std::ostringstream os;
        os<<"vertex id:"<<id<<"\t key:"<<key;
        return os.str();
    }
    KeyType key;  /*!< 顶点存储数据*/
    const VIDType id;   /*!< 顶点id，一旦顶点生成则id不可变*/
};

//!Graph：图，算法导论22章22.1节
/*!
* 图的矩阵主要包含四个数据：
*
* - `matrix`：图的矩阵表示，类型为`MatrixGraph<N>`
* - `adjList`：图的邻接表表示，类型为`ADJListGraph<N>`
* - `vertexes`：顶点集合，类型为`std::array<std::shared_ptr<VertexType>,N>`。它是一个`std::array`，其元素类型为指向顶点的强引用
* - `next_empty_vertex`：顶点集合中，下一个为空的位置，类型为`std::size_t`。它用于添加顶点。

* 图支持插入、修改顶点操作，插入、修改边操作（由图的矩阵以及图的邻接表来代理），以及返回边、返回权重（由图的矩阵以及图的邻接表来代理）。
*
*/
template<unsigned N,typename VType> struct Graph
{
    typedef int VIDType;     /*!< 顶点编号的类型*/
    typedef int EWeightType; /*!< 权重的类型*/
    typedef std::tuple<VIDType,VIDType,EWeightType> EdgeTupleType; /*!< 边的三元素（顶点1编号，顶点2编号，权重)组成的元组*/
    typedef VType VertexType; /*!< 顶点的类型*/
    static const unsigned NUM=N; /*!< 顶点的最大容量*/

    //!显式构造函数，为图的矩阵指定`invalid_weight`
    /*!
    * \param  val:无效权重值
    */
    explicit Graph(EWeightType val):matrix(val),next_empty_vertex(0)
    {}
    //!默认构造函数
    Graph():next_empty_vertex(0){}

    //!add_vertex:添加一个顶点
    /*!
    * \param  key:顶点存放的数据
    * \return: 顶点的id
    *
    * 如果已经有了N个顶点，则图的顶点已满，则抛出`std::invalid_argument`异常.
    *
    * 在每一次添加顶点之前会从`next_empty_vertex·指定的位置处开始寻找可以添加顶点的地方。如果找不到可以添加顶点的地方，则抛出`std::invalid_argument`异常
    */
    VIDType add_vertex(const typename VertexType::KeyType& key)
    {
        while(next_empty_vertex<N&&vertexes.at(next_empty_vertex))
            next_empty_vertex++;
        if(next_empty_vertex>=N)
            throw std::invalid_argument("add_vertex error:Graph Vertex is full, can not add vertex.");
        VIDType v_id=next_empty_vertex;
        vertexes.at(next_empty_vertex)=std::make_shared<VertexType>(key,v_id);
        next_empty_vertex++;
        return v_id;
    }
    //!add_vertex:添加一个顶点
    /*!
    * \param  key:顶点存放的数据
    * \param id:指定该顶点的`id`
    * \return: 顶点的id
    *
    * - 如果`id<0`或者`id>=N`，则抛出异常。因为正常的顶点`id`在`[0,N)`之间
    * - 如果已经存在某个顶点的`id`为指定的`id`，则抛出异常
    */
    VIDType add_vertex(const typename VertexType::KeyType &key,VIDType id)
    {
        if(id<0||id>=N)
            throw std::invalid_argument("add_vertex error:id must >=0 and <N.");
        if(vertexes.at(id))
            throw std::invalid_argument("add_vertex error: vertex of id has existed.");
        vertexes.at(id)=std::make_shared<VertexType>(key,id);
        return id;
    }
    //!modify_vertex:修改一个顶点的数据
    /*!
    * \param  newkey:新的数据
    * \param id:指定该顶点的`id`
    *
    * - 如果`id<0`或者`id>=N`，则抛出异常。因为正常的顶点`id`在`[0,N)`之间
    * - 如果不存在某个顶点的`id`为指定的`id`，则抛出异常
    */
    void modify_vertex(const typename VertexType::KeyType &newkey,VIDType id)
    {
        if(id<0||id>=N)
            throw std::invalid_argument("modify_vertex error:id must >=0 and <N.");
        if(!vertexes.at(id))
            throw std::invalid_argument("modify_vertex error: vertex of id does not exist.");
        vertexes.at(id)->key=newkey;
    }

    //!add_edge:添加一条边
    /*!
    * \param  edge_tuple:一条边的三元素元组
    *
    * 为了便于计算，添加边时并不是添加`Edge`类型，而是`std::tuple<VIDType,VIDType,EWeightType>`类型的值。
    *
    * 添加边之前如果边指定的任何一个顶点无效，则抛出异常：
    *
    * - 如果指定的顶点`id`不在`[0,N)`之间，则无效
    * - 如果不存在某个顶点与指定的顶点`id`相同，则无效
    *
    * 在添加边时，同时向图的矩阵、图的邻接表中添加边
    *
    * 如果添加的边是无效权重，则直接返回而不添加
    */
    void add_edge(const EdgeTupleType& edge_tuple)
    {
        auto id1=std::get<0>(edge_tuple);
        auto id2=std::get<1>(edge_tuple);

        if(id1<0||id1>=N||id2<0||id2>=N)
            throw std::invalid_argument("add edge error:id must >=0 and <N.");
        if(!vertexes.at(id1) || !vertexes.at(id2))
            throw std::invalid_argument("add edge error: vertex of id does not exist.");
        if(std::get<2>(edge_tuple)==matrix.invalid_weight) return;
        matrix.add_edge(edge_tuple);
        adjList.add_edge(edge_tuple);
    }
    //!add_edges:添加一组边
    /*!
    * \param  begin:边容器的起始迭代器
    * \param  end:边容器的终止迭代器
    *
    * 为了便于计算，添加边时并不是添加`Edge`类型，而是`std::tuple<VIDType,VIDType,EWeightType>`类型的值
    *
    * 在添加边时，同时向图的矩阵、图的邻接表中添加边
    */
    template<typename Iterator> void add_edges(const Iterator&begin ,const Iterator&end)
    {

        if(std::distance(begin,end)<=0)
            return;
        Iterator iter=begin;
        while(iter!=end)
        {
            add_edge(*iter);
            iter++;
        }
    }
    //!adjust_edge:修改一条边的权重
    /*!
    * \param  id1:待修改边的第一个顶点
    * \param  id2:待修改边的第二个顶点
    * \param  wt:新的权重
    *
    * 修改边权重之前如果边指定的任何一个顶点无效，则抛出异常：
    *
    * - 如果指定的顶点`id`不在`[0,N)`之间，则无效
    * - 如果不存在某个顶点与指定的顶点`id`相同，则无效
    */
    void adjust_edge(VIDType id1,VIDType id2,EWeightType wt)
    {
        if(id1<0||id1>=N||id2<0||id2>=N)
            throw std::invalid_argument("adjust edge error:id must >=0 and <N.");
        if(!vertexes.at(id1) || !vertexes.at(id2))
            throw std::invalid_argument("adjust edge error: vertex of id does not exist.");
        matrix.adjust_edge(id1,id2,wt);
        adjList.adjust_edge(id1,id2,wt);
    }
    //!edge_tuples:返回图中所有边的三元素元组集合，这里集合采用`std::vector<std::tuple<VIDType,VIDType,EWeightType>>`
    /*!
    * \return  :图中所有边的三元素元组集合
    *
    * 要求图的矩阵和图的邻接表都返回同样的结果
    */
    const std::vector<EdgeTupleType> edge_tuples() const
    {
        auto edges1=matrix.edge_tuples();
        auto edges2=adjList.edge_tuples();
        auto compare=[](const EdgeTupleType& e1,const EdgeTupleType&e2)->bool {
            if(
                    (std::get<0>(e1) < std::get<0>(e2))||
                    ( (std::get<0>(e1) == std::get<0>(e2))&&(std::get<1>(e1) < std::get<1>(e2)) ) ||
                    ( (std::get<0>(e1) == std::get<0>(e2))&&(std::get<1>(e1) == std::get<1>(e2))&&(std::get<2>(e1) < std::get<2>(e2)) )
                ) return true;
           return false;
        };
        std::sort(edges1.begin(),edges1.end(),compare);
        std::sort(edges2.begin(),edges2.end(),compare);
        assert(edges1==edges2);
        return edges1;
    }

    //!vertex_edge_tuples:返回图中从指定顶点出发的边的三元素元组集合，这里集合采用`std::vector<std::tuple<VIDType,VIDType,EWeightType>>`
    /*!
    * \param id: 指定顶点`id`
    * \return  :图中指定顶点出发的边的三元素元组集合
    *
    * 要求图的矩阵和图的邻接表都返回同样的结果
    *
    * - 如果指定的顶点`id`不在`[0,N)`之间，则无效
    * - 如果不存在某个顶点与指定的顶点`id`相同，则无效
    */
    const std::vector<EdgeTupleType> vertex_edge_tuples(VIDType id) const
    {
        if(id<0||id>=N)
            throw std::invalid_argument("vertex_edge_tuples error:id must >=0 and <N.");
        if(!vertexes.at(id))
            throw std::invalid_argument("vertex_edge_tuples error: vertex of id does not exist.");

        auto edges1=matrix.vertex_edge_tuples(id);
        auto edges2=adjList.vertex_edge_tuples(id);
        auto compare=[](const EdgeTupleType& e1,const EdgeTupleType&e2)->bool {
            if(
                    (std::get<0>(e1) < std::get<0>(e2))||
                    ( (std::get<0>(e1) == std::get<0>(e2))&&(std::get<1>(e1) < std::get<1>(e2)) ) ||
                    ( (std::get<0>(e1) == std::get<0>(e2))&&(std::get<1>(e1) == std::get<1>(e2))&&(std::get<2>(e1) < std::get<2>(e2)) )
                ) return true;
           return false;
        };
        std::sort(edges1.begin(),edges1.end(),compare);
        std::sort(edges2.begin(),edges2.end(),compare);
        assert(edges1==edges2);
        return edges1;
    }

    //!has_edge:返回图中指定顶点之间是否存在边
    /*!
    * \param id_from: 第一个顶点的`id`
    * \param id_to: 第二个顶点的`id`
    * \return  :第一个顶点和第二个顶点之间是否存在边
    *
    * 要求图的矩阵和图的邻接表都返回同样的结果
    *
    *  如果边指定的任何一个顶点无效，则抛出异常：
    *
    * - 如果指定的顶点`id`不在`[0,N)`之间，则无效
    * - 如果不存在某个顶点与指定的顶点`id`相同，则无效
    */
    bool has_edge(VIDType id_from,VIDType id_to) const
    {
        if(id_from<0||id_from>=N||id_to<0||id_to>=N)
            throw std::invalid_argument("has edge error:id must >=0 and <N.");
        if(!vertexes.at(id_from) || !vertexes.at(id_to))
            throw std::invalid_argument("has edge error: vertex of id does not exist.");
       auto m=matrix.has_edge(id_from,id_to);
       auto a=adjList.has_edge(id_from,id_to);
       assert(matrix.has_edge(id_from,id_to)==adjList.has_edge(id_from,id_to));
       return matrix.has_edge(id_from,id_to);
    }

    //!weight:返回图中指定顶点之间的边的权重
    /*!
    * \param id_from: 第一个顶点的`id`
    * \param id_to: 第二个顶点的`id`
    * \return  :第一个顶点和第二个顶点之间的边的权重
    *
    * 要求图的矩阵和图的邻接表都返回同样的结果
    *
    *  如果边指定的任何一个顶点无效，则抛出异常：
    *
    * - 如果指定的顶点`id`不在`[0,N)`之间，则无效
    * - 如果不存在某个顶点与指定的顶点`id`相同，则无效
    */
    EWeightType weight(VIDType id_from,VIDType id_to) const
    {
        if(id_from<0||id_from>=N||id_to<0||id_to>=N)
            throw std::invalid_argument("edge weight error:id must >=0 and <N.");
        if(!vertexes.at(id_from) || !vertexes.at(id_to))
            throw std::invalid_argument("edge weight error: vertex of id does not exist.");
        assert(matrix.weight(id_from,id_to)==adjList.weight(id_from,id_to));
        return matrix.weight(id_from,id_to);
    }

    //!inverse:返回图的一个镜像
    /*!
    * \return  :图的一个镜像的一个强引用
    *
    * 图的一个镜像也是一个图，它与原图有以下关系：
    *
    * - 图的镜像的顶点与原图的顶点相同
    * - 图的镜像的边是原图的边的反向
    *
    * `inverse`方法首先新建一个图，再根据原图的顶点来执行顶点的深拷贝。然后再获取原图的边的反向边，将该反向边作为镜像图的边
    */
    std::shared_ptr<Graph> inverse()
    {
        std::shared_ptr<Graph> graph=std::make_shared<Graph>(matrix.invalid_weight);
        for(int i=0;i<vertexes.size();i++)
        {
            if(vertexes.at(i))
            {
                graph->vertexes.at(i)=std::make_shared<VertexType>(vertexes.at(i)->key,vertexes.at(i)->id);
            }
        }
        auto edges=edge_tuples();
        for(auto&edge :edges)
            std::swap(std::get<0>(edge),std::get<1>(edge));
        graph->add_edges(edges.begin(),edges.end());
        return graph;
    }

    std::array<std::shared_ptr<VertexType>,N> vertexes;
    std::size_t next_empty_vertex;
    MatrixGraph<N> matrix;
    ADJListGraph<N> adjList;
};

typedef Graph<MT_N,Vertex<int>> GType; /*!< 模板实例化的图类型，该图的顶点类型为`Vertex<int>`*/
int main(int argc, char const *argv[])
{
    //****  算法导论图25-1   ****
    _graph=std::make_shared<GType>(unlimit<int>()); //边的无效权重为正无穷

    return 0;
}


