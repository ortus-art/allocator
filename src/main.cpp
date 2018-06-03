#include <algorithm>
#include <iostream>
#include <map>

#include "chunk_allocator.h"
#include "app_lib.h"



int main()
{

    try
        {


            app::fill_and_print(std::cout, app::int_map<>{});
            std::cout <<std::endl;

            using map_custom_alloc = app::int_map<int, allocator::chunk_allocator<std::pair<const int, int>>>;
            app::fill_and_print(std::cout, map_custom_alloc{});
            std::cout <<std::endl;



            app::fill_and_print(std::cout, app::int_list<>{});
            std::cout <<std::endl;


            using custom_link_custom_alloc = app::int_list<int, allocator::chunk_allocator<int>>;
            app::fill_and_print(std::cout, custom_link_custom_alloc{}) ;
            std::cout <<std::endl;


        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }

}
