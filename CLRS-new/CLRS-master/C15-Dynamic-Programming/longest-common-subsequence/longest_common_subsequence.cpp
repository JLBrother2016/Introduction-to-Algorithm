#include <functional>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#include "matrix.hpp"
#include "color.hpp"

namespace ch15 {
/**
 * @brief  as a part of an entry
 */
enum class Arrow
{
    NA, LEFT, UP, DIAGONAL
};

/**
 * @brief The Entry struct
 *
 * as an entry of Table
 */
template<typename T, typename A>
struct Entry
{
    //! Ctors
    Entry() = default;
    Entry(const T& len, const A& arw = Arrow::NA):
        length(len), arrow(arw)
    {}

    //! update this entry
    Entry operator ()(const T& len, const A& arw = Arrow::NA)
    {
        length  =   len;
        arrow   =   arw;

        return *this;
    }

    T length = 0;
    A arrow = Arrow::NA;
};

/**
 * @brief operator <<
 *
 * called when printing a boost matrix
 */
template<typename T, typename A>
inline std::ostream&
operator <<(std::ostream& os , const Entry<T,A>& etr)
{
    os << color::green("[")
       << etr.length << ",";

    switch (etr.arrow)
    {
    case Arrow::NA      :   os << " ";      break;
    case Arrow::LEFT    :   os << "<";      break;
    case Arrow::DIAGONAL:   os << "\\";     break;
    case Arrow::UP      :   os << "^";      break;
    }

    os << color::green("]");

    return os;
}

/**
 * @brief operator >=
 *
 * overloaded for comparison in build_lcs_table
 */
template<typename T, typename A>
inline bool
operator >=(const Entry<T,A>& lhs, const Entry<T,A>& rhs)
{
    return lhs.length >= rhs.length;
}

//! @alias for Table
template<typename T, typename A>
using Table = ch15::Matrix<Entry<T,A>>;

/**
 * @brief build lcs table
 * @param lhs   X
 * @param rhs   Y
 *
 * @pseudocode  LCS-LENGTH
 * @page    394, CLRS
 * @complx  O(mn)
 *
 * @note    the tables b and c in the peudocode have been merged in to one.
 *          so only one table is returned
 */
template<typename Range, typename A = Arrow>
Table<typename Range::size_type, A>
build_lcs_table(const Range& lhs, const Range& rhs)
{
    //! types def
    using SizeType  =   typename Range::size_type;
    using TableType =   Table<SizeType, A>;

    //! init
    SizeType rows = lhs.size() + 1;
    SizeType cols = rhs.size() + 1;
    TableType lcs(rows, cols);             //构建一个LCS(Longest common subsequence)表
    //! ^^^^^^^^^^^^^^^^^^^^^^@attention  :
    //!     the Entry's default ctor grantee that its member length has
    //!     default value 0.so no need to implementlines 4 - 7 explicitly.

    //! build lcs table
    for(SizeType r = 1; r != rows; ++r)
        for(SizeType c = 1; c != cols; ++c)
        {
            if(lhs[r - 1]   ==  rhs[c - 1])
                lcs(r,c) (lcs(r - 1, c - 1).length + 1   ,  Arrow::DIAGONAL);
            else if(lcs(r - 1, c) >= lcs(r, c - 1))
                lcs(r,c) (lcs(r - 1, c).length           ,  Arrow::UP);
            else
                lcs(r,c) (lcs(r, c - 1).length           ,  Arrow::LEFT);
        }

    return lcs;
}

/**
 * @brief The LongestCommonSubsequence class
 */
template<typename Range, typename A = ch15::Arrow>
class LongestCommonSubsequence
{
public:
    //! types def
    using SizeType  =   typename Range::size_type;
    using TableType =   ch15::Table<SizeType, A>;
    using Pointer   =   const Range*;

    //! Ctor
    LongestCommonSubsequence(const Range& l, const Range& r):
        lhs(&l),
        rhs(&r),
        maze(ch15::build_lcs_table(l,r))
    {}

    /**
     * @brief print_maze
     *
     * print the maze built by build_lcs_table
     */
    void print_maze()const
    {
        ch15::print(maze);
    }

    /**
     * @brief return the generated longest common sequence
     *
     * @pseudocode PRINT-LCS
     * @page    395, CLRS
     * @complx  O(m + n)
     */
    Range generate() const
    {
        assert(lhs && rhs);
        using Lambda = std::function<void(SizeType, SizeType)>;     //lamda函数
        Range lcs;

        //! a recursive lamda that performs the real work
        Lambda build_lcs = [&lcs, &build_lcs, this](SizeType row, SizeType col)    //找出最优解，输出的是字符串
        {
            //! stop condition
            if(row == 0 || col == 0)    return;

            //! build the longest common sequence
            if(maze(row, col).arrow ==  Arrow::DIAGONAL)                           //对角线
            {
                build_lcs(row - 1, col - 1);
                lcs.push_back((*lhs)[row - 1]);
            }
            else if(maze(row, col).arrow ==  Arrow::UP)                            //向上
                build_lcs(row - 1, col);
            else                                                                   //向左
                build_lcs(row, col - 1);
        };

        //! call the lambda
        build_lcs(lhs->size(), rhs->size());
        return lcs;
    }

private:
    Pointer lhs;
    Pointer rhs;
    TableType maze;
};
}

int main()
{
   //! strings used on page 394, CLRS.
   std::string lhs = "ABCBDAB";
   std::string rhs = "BDCABA";

   using LCS   =   ch15::LongestCommonSubsequence<std::string>;
   LCS lcs(lhs, rhs);
   lcs.print_maze();

   auto sequence = lcs.generate();
   std::cout << "The longest common sequence = ";
   std::cout << color::yellow(sequence) << std::endl;

   std::cout << color::red("\nend\n");
   return 0;
}