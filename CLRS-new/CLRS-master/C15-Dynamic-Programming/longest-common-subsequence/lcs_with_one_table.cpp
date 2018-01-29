#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#include <functional>
#include <memory>
#include "matrix.hpp"
#include "color.hpp"

namespace ch15 {

/**
 * @brief The LcsWithOneTable class
 *
 * for ex15.4-2
 */
template<typename Range>
class LcsWithOneTable
{
public:
    using SizeType  =   typename Range::size_type;
    using Pointer   =   const Range*;
    using sPointer  =   std::shared_ptr<Range>;
    using MatrixType=   ch15::Matrix<SizeType>;

    //! Ctor
    LcsWithOneTable(const Range& l, const Range& r):
        lhs(&l),
        rhs(&r),
        maze(l.size() + 1, r.size() + 1, 0)
    {
        build_maze();
    }

    /**
     * @brief print_maze
     */
    void print_maze()const
    {
        ch15::print(maze);
    }

    /**
     * @brief generate
     */
    sPointer generate() const       //通过maze矩阵找出最优解
    {
        assert(lhs && rhs);
        sPointer lcs = std::make_shared<Range>();

        //! lambda to do the real work
        using Lambda = std::function<void(SizeType, SizeType)>;
        Lambda build_lcs = [&lcs, &build_lcs, this](SizeType l, SizeType r)
        {
            //! stop condition
            if(l == 0 || r == 0)    return;     //通过行列的左值和上边的值比较

            //! recur
            if((*lhs)[l - 1]    ==  (*rhs)[r - 1])     
            {
                build_lcs(l - 1, r - 1);
                lcs->push_back((*lhs)[l - 1]);
            }
            else if (maze(l - 1, r) >= maze(l, r - 1))
                build_lcs(l - 1, r);
            else
                build_lcs(l, r - 1);
        };

        //! call the lambda
        build_lcs(lhs->size(),rhs->size());
        return lcs;
    }

private:
    Pointer lhs;
    Pointer rhs;
    MatrixType maze;         //maze是迷宫, maze就是矩阵C                   

    /**
     * @brief build_maze
     *
     * @complx  O(m + n)
     * for ex15.4-2
     */
    void build_maze()
    {
        for(SizeType l = 1; l != maze.size1(); ++l)
            for(SizeType r = 1; r != maze.size2(); ++r)
            {
                if((*lhs)[l - 1]    ==  (*rhs)[r - 1])
                    maze(l,r)   =   maze(l - 1, r - 1) + 1;
                else if(maze(l - 1, r) >= maze(l, r - 1))
                    maze(l,r)   =   maze(l - 1, r);
                else
                    maze(l,r)   =   maze(l, r - 1);
            }
    }
};
}

int main()
{
   std::string lhs = "ABCBDAB";
   std::string rhs = "BDCABA";

   using LCS   =   ch15::LcsWithOneTable<std::string>;
   LCS lcs(lhs, rhs);
   lcs.print_maze();

   auto sequence = lcs.generate();
   std::cout << "The longest common sequence = ";
   std::cout << color::yellow(*sequence) << std::endl;

   std::cout << color::red("\nend\n");
   return 0;
}

