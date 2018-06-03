#pragma once

#ifdef APP_DEBUG
#include "mem_debug.h"
#endif

#include "app_traits.h"
#include "linked_list.h"
#include <algorithm>

namespace app {

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


template<typename _Tp = int, typename _Alloc = std::allocator<std::pair<const _Tp, _Tp> > ,
         typename _Compare = std::less<_Tp>, typename = app::enable_if_integral_t<_Tp>>
using int_map = std::map<_Tp, _Tp, _Compare, _Alloc>;

template<typename _Tp = int, typename _Alloc = std::allocator<_Tp> , typename = app::enable_if_integral_t<_Tp> >
using int_list = allocator::linked_list<_Tp, _Alloc>;


template<typename _Tp, typename _Compare = std::less<_Tp>,
         typename _Alloc = std::allocator<std::pair<const _Tp, _Tp> >>
std::ostream& operator<<(std::ostream &stream, const int_map<_Tp, _Alloc, _Compare> & cntr)
{
    for (const auto& p: cntr)
        stream << p.first << " " << p.second << std::endl;
    return stream;
}



template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
std::ostream& operator<<(std::ostream &stream, int_list<_Tp, _Alloc>& cntr)
{
    for (const auto& p: cntr)
         stream << p << std::endl;
    return stream;
}

template<typename _Tp, typename _Compare = std::less<_Tp>,
         typename _Alloc = std::allocator<std::pair<const _Tp, _Tp> > >
void fill_cntr(int_map<_Tp, _Alloc, _Compare> & cntr, int times = 10)
{
    auto inserter = [](auto & container) { return std::inserter(container, std::begin(container));};

    auto make_factorial = [i = 0]() mutable
    {
        auto f = factorial(i);
        auto value = std::make_pair(i,f);
        ++i;
        return value;
    };



    std::generate_n(inserter(cntr)
                , times
                , make_factorial
    );
}


template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
void fill_cntr(int_list<_Tp, _Alloc>& cntr, int times = 10)
{
    auto it = cntr.begin();
    for(_Tp i = times -1; i >= 0; i-- )
        cntr.push_front(i);
}

template<typename _Tp, typename _Compare = std::less<_Tp>,
         typename _Alloc = std::allocator<std::pair<const _Tp, _Tp> > >
void fill_and_print(std::ostream &stream, int_map<_Tp, _Alloc, _Compare> && cntr, int times = 10)
{
    fill_cntr(cntr, times);
    stream << cntr;
}

template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
void fill_and_print(std::ostream &stream, int_list<_Tp, _Alloc>&& cntr, int times = 10)
{
    fill_cntr(cntr, times);
    stream << cntr;
}

} //namespace app
