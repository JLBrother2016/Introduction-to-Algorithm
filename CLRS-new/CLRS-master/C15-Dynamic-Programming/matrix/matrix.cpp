#include <boost/numeric/ublas/matrix.hpp>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#include <assert.h>


namespace color{

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

template<typename T>
using Matrix = boost::numeric::ublas::matrix<T>;

/**
 * @brief print a matrix like a matrix
 */
template<typename T>
void print(const Matrix<T>& mat)
{
    using SizeType  =   typename Matrix<T>::size_type;

    for(SizeType row = 0; row != mat.size1(); ++row)
    {
        for(SizeType col = 0; col != mat.size2(); ++col)
            std::cout << mat(row, col) << " ";
        std::cout << "\n\n";
    }
}

/**
 * @brief copy_row
 * @param mat
 * @param rng
 * @param row
 *
 * @date  13 aug 2014
 *
 * copy a row from a std container into a boost matrix.
 * implemented for longest_mono_increasing.hpp
 */
template<typename Range>
void inline
copy_row(Matrix<typename Range::value_type>& mat,
                const Range& rng,
                typename Matrix<typename Range::value_type>::size_type
                        row)
{
    assert(rng.size()   ==  mat.size2());
    using SizeType  =   typename Range::size_type;

    for(SizeType count = 0; count != rng.size(); ++count)
        mat(row, count) =   rng[count];
}


//! @brief  extending boost matrix
namespace boost { namespace numeric { namespace ublas {

/**
 * @brief forwarding declarations
 */
template<typename Range>
void
operator <<= (matrix<typename Range::value_type>& mat, const Range& rng);
template<typename Range>
void
operator >>= ( const Range& rng, matrix<typename Range::value_type>& mat);
template<typename T>
matrix<T>
operator*(const matrix<T>& lhs, const matrix<T>& rhs);

/**
 * @brief operator <<=
 * @param lhs   matrix
 * @param rhs   a stl container that stores data
 *
 * copy data from rhs to matrix, leaving data in matrix untouched when data from
 * the stl container is not enough.
 */
template<typename Range>
inline void
operator <<= (matrix<typename Range::value_type>& mat, const Range& rng)
{
    using SizeType  =   typename matrix<typename Range::value_type>::size_type;

    //! check if equal
    SizeType mat_size   =   mat.size1() *   mat.size2();
    assert(mat_size == rng.size());

    //! copy
    for(SizeType index = 0; index != mat_size; ++index)
        mat.data()[index]   =   rng[index];
}

/**
 * @brief operator >>=
 * @param lhs   a stl container that stores data
 * @param rhs   matrix
 */
template<typename Range>
inline void
operator >>= ( const Range& rng, matrix<typename Range::value_type>& mat)
{
    mat <<= rng;
}

/**
 * @brief operator *
 * @param lhs
 * @param rhs
 *
 * @page 371
 * @pseudocode MATRIX-MULTIPLY
 */
template<typename T>
inline matrix<T>
operator*(const matrix<T>& lhs, const matrix<T>& rhs)
{
    assert(lhs.size2() == rhs.size1());

    matrix<T> ret(lhs.size1(), rhs.size2());
    for(unsigned row = 0; row != lhs.size1(); ++row)
        for(unsigned col = 0; col != rhs.size2(); ++ col)
        {
            ret(row,col) = 0;
            for(unsigned count = 0 ; count != lhs.size2(); ++count)
                ret(row,col) += lhs(row,count) * rhs(count, col);
        }

    return ret;
}

}}}//namespace boost::numeric::ublas


template<typename T>
using Chain = std::vector<Matrix<T>>;

//! forwad declarations
template<typename Range>
class MatrixChainOrder;

template<typename T>
Matrix<T> matrix_chain_multiply(const Chain<T>& chain);

/**
 * @brief The MatrixChain class
 */

template<typename Range>
class MatrixChainOrder
{
    friend Matrix<typename Range::value_type>
        matrix_chain_multiply<typename Range::value_type>
            (const Chain<typename Range::value_type>& chain);
public:
    using ValueType =   typename    Range::value_type;
    using SizeType  =   typename    Range::size_type;
    using Matrix1    =       Matrix<ValueType>;

    /**
     * @brief Ctor
     */
    MatrixChainOrder(const Range& d):
        data(d),
        m(d.size() - 1, d.size() - 1),
        s(d.size() - 2, d.size() - 2),
        size(d.size() - 1)
    {
        assert(size > 0);
    }

    /**
     * @brief build
     *
     * @pseudocode  MATRIX-CHAIN-ORDER
     * @page 375, CLRS
     * @time complx  theta(n^3)
     * @space required  theta(n^2)
     */
    void build()
    {
        //! (head == tail) == true
        for(SizeType head = 0; head != size; ++head)
            m(head,head)  =   0;

        for(SizeType len = 2; len != size + 1; ++len)
        {
            for(SizeType head = 0; head != size - len + 1; ++head)
            {
                SizeType tail = head + len - 1;
                m(head,tail)  =   std::numeric_limits<ValueType>::max();

                for(SizeType split = head; split != tail; ++split)
                {
                    ValueType result  =
                                m(head,split)
                            +   m(split + 1, tail)
                            +   data[head] * data[split + 1] * data[tail + 1];

                    if(result < m(head,tail))
                    {
                        m(head,tail)        =   result;
                        s(head,tail - 1)    =   split + 1;
                    }
                }
            }
        }
    }

    /**
     * @brief print_optimal
     * @param head  :   i
     * @param tail  :   j
     *
     * @pseudocode PRINT-OPTIMAL-PARENTS
     * @page    377, CLRS
     */
    void print_optimal(SizeType head, SizeType tail)const
    {
        if(head  == tail)
            std ::cout << "A" << head;
        else
        {
            std::cout << color::yellow("(");
            print_optimal(head, s(head - 1,tail - 2));
            print_optimal(s(head - 1,tail - 2) + 1, tail);
            std::cout << color::yellow(")");
        }
    }

private:
    const Range& data;
    Matrix1 m;
    Matrix1 s;
    SizeType size;
};

int main()
{
   Matrix<int> mat(3,3,0);

   copy_row(mat, std::vector<int>({1,2,3}), 0);
   print(mat);

   std::cout << color::red("\nend\n");

   std::vector<int> v = {30,35,15,5,10,20,25};
   MatrixChainOrder<decltype(v)> chain_order(v);

   chain_order.build();
   chain_order.print_optimal(1,6);

   std::cout << color::red("\nend\n");
   return 0;

   return 0;
}

