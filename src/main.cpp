#include <algorithm>
#include <iostream>
#include <map>

#include "chunk_allocator.h"
#include "linked_list.h"

template<typename T>
constexpr auto factorial(T n) -> decltype(n)
{
    return n ? n*factorial(n-1):1;
}

static_assert(factorial(9) == 9*8*7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(3) ==             3*2*1, "factorial failed!");
static_assert(factorial(2) ==               2*1, "factorial failed!");
static_assert(factorial(1) ==                 1, "factorial failed!");
static_assert(factorial(0) ==                 1, "factorial failed!");


int main()
{
    auto make_factorial = [i = 0]() mutable
    {
        auto f = factorial(i);
        auto value = std::make_pair(i,f);
        ++i;
        return value;
    };

    auto inserter = [](auto & container) { return std::inserter(container, std::begin(container));};
    auto front_inserter = [](auto & container) { return std::front_inserter(container);};

    auto fill_and_print = [&make_factorial](auto&& container, auto &inserter)
    {
        std::generate_n(inserter(container)
                    , 10
                    , make_factorial
        );
        for (const auto& p: container)
            std::cout << p.first << " " << p.second << std::endl;
    };



    try
        {
            std::cout << "std::map, standard allocator ->" <<std::endl;
            fill_and_print( std::map<int, int>{}, inserter);
             std::cout <<std::endl;

            using map_custom_alloc = std::map<int, int, std::less<int>, allocator::chunk_allocator<std::pair<const int, int>>>;

            std::cout << "std::map, custom allocator ->" <<std::endl;
            fill_and_print(map_custom_alloc{}, inserter);
            std::cout <<std::endl;



            using custom_link_std_alloc = allocator::linked_list<std::pair<const int, int>>;
            std::cout << "custom list, standard allocator ->" <<std::endl;
            fill_and_print( custom_link_std_alloc{}, front_inserter);
            std::cout <<std::endl;

            using custom_link_custom_alloc = allocator::linked_list<std::pair<const int, int>, allocator::chunk_allocator<std::pair<const int, int>>>;
            std::cout << "custom list, custom allocator ->" <<std::endl;
            fill_and_print( custom_link_custom_alloc{}, front_inserter);
            std::cout <<std::endl;

        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }

}
