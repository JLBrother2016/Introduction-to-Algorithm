#include <assert.h>
#include <iostream>
#include <vector>
using namespace std;

template<typename Range>
inline void
activity_select_recur(const Range& starts,
                      const Range& finishes,
                      typename Range::size_type pos,
                      Range& output)
{
    assert(starts.size() == finishes.size());

    //! search for next activity
    auto curr = pos + 1;
    while(curr < starts.size()  &&  starts[curr] < finishes[pos])   ++curr;

    //! if within the boundary, push back and recur
    if( curr < starts.size())
    {
        output.push_back(curr);
        activity_select_recur(starts, finishes, curr, output);
    }
}

template<typename Range>
inline void
activity_select_itera(const Range& starts, const Range& finishes, Range& output)
{
    output.push_back(0);

    using SizeType  =   typename Range::size_type;
    SizeType pos    =   0;
    for(SizeType curr = 1; curr != starts.size(); ++curr)
    {
        if(starts[curr] >=  finishes[pos])
        {
            output.push_back(curr);
            pos = curr;
        }
    }
}

namespace alan {

/**
 * @brief print_container
 * @param rng
 */
template<typename Range>
inline void
print_container(const Range& rng)
{
    for(const auto& elem : rng)
        std::cout << elem << " ";
}

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

}//namespaces

int main()
{
   std::vector<int> starts     =   {1,3,0,5,3,5,6,8,8,2,12};
   std::vector<int> finishes   =   {4,5,6,7,9,9,10,11,12,14,16};
   std::vector<int> activites;

   activity_select_itera(starts, finishes, activites);
   // activity_select_recur(starts, finishes, 0, activites);
   alan::print_container(activites);

   std::cout << alan::green("\nexit normally.\n");
   return 0;
}
