#include <linked_list.h>
#include <chunk_allocator.h>
#include <gtest/gtest.h>

#include <sstream>

#include <app_lib.h>


using namespace testing;


TEST(allocator_case, int_test)
{
    const auto counter = app::alloc_counter;
    {
        allocator::chunk_allocator<int> allocator;
        auto ptr = allocator.allocate(1);
        ASSERT_TRUE(nullptr != ptr);
        allocator.deallocate(ptr, 1);
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}

TEST(allocator_case, catch_errors)
{
    const auto counter = app::alloc_counter;
    {
        allocator::chunk_allocator<int> allocator;
        try {
            auto ptr = allocator.allocate(2);
            FAIL() << "Expected ip_filter::parser_error";
        } catch (std::invalid_argument & e) {
             EXPECT_EQ(e.what(),std::string("Currently allocator supports only single cell allocation"));
        } catch (std::exception & e) {
            FAIL() << "Expected ip_filter::parser_error" << e.what();
        } catch (...) {
            FAIL() << "Expected ip_filter::parser_error";
        }

        try {
            auto ptr = allocator.allocate(1);
            allocator.deallocate(ptr, 2);
            FAIL() << "Expected ip_filter::parser_error";
        } catch (std::invalid_argument & e) {
             EXPECT_EQ(e.what(),std::string("Currently allocator supports only single cell allocation"));
        } catch (std::exception & e) {
            FAIL() << "Expected ip_filter::parser_error" << e.what();
        } catch (...) {
            FAIL() << "Expected ip_filter::parser_error";
        }
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);

}

TEST(list_allocator, allocator_test_insert_after)
{
    const auto counter = app::alloc_counter;
    {
        using namespace allocator;
        linked_list<int, chunk_allocator<int>> list;
        auto it = list.begin();
        for(auto i = 20; i >= 0; i--)
            list.push_front(i);

        std::ostringstream log;
        for (const auto &value : list) {
            log << value << ',';
        }

        ASSERT_EQ(log.str(), "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,");
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}


TEST(list_allocator, allocator_test_iterator_lifo)
{
    const auto counter = app::alloc_counter;
    {
        using namespace allocator;
        linked_list<int, chunk_allocator<int, 2, allocator::memory_strategy::LIFO>> list;
        auto it = list.begin();
        for(auto i = 20; i >= 0; i--)
            list.push_front(i);

        std::ostringstream log;
        for (it = list.begin(); it != list.end(); ++it) {
            log << *it << ',';
        }

        ASSERT_EQ(log.str(), "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,");
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}

TEST(list_allocator, allocator_test_iterator_fifo)
{
    const auto counter = app::alloc_counter;
    {
        using namespace allocator;
        linked_list<int, chunk_allocator<int, 2, allocator::memory_strategy::FIFO>> list;
        auto it = list.begin();
        for(auto i = 20; i >= 0; i--)
            list.push_front(i);


        std::ostringstream log;
        for (it = list.begin(); it != list.end(); ++it) {
            log << *it << ',';
        }

        ASSERT_EQ(log.str(), "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,");
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}

TEST(list_case, list_test)
{
    const auto counter = app::alloc_counter;
    {
        allocator::linked_list<int> list;
        for(auto i = 0; i < 10; i++)
            list.push_front(i);

        std::ostringstream log;
        for (const auto &value : list) {
            log << value << ',';
        }

        ASSERT_EQ(log.str(), "9,8,7,6,5,4,3,2,1,0,");
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}

TEST(list_case, list_pair_test)
{
    const auto counter = app::alloc_counter;
    {
        using namespace allocator;
        using custom_link_std_alloc = linked_list<std::pair<const int, int>>;
        custom_link_std_alloc list;
        for(auto i = 0; i < 10; i++)
        {
            const auto value = std::make_pair(i,i);
            list.push_front(value);
        }

        std::ostringstream log;
        for (const auto &value : list) {
            log << value.first << ',';
        }

        ASSERT_EQ(log.str(), "9,8,7,6,5,4,3,2,1,0,");
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}


TEST(main_case, allocator_test)
{
    const auto counter = app::alloc_counter;
    {
        using namespace allocator;
        linked_list<int, chunk_allocator<int>> list;
        for(auto i = 0; i < 10; i++)
            list.push_front(i);

        std::ostringstream log;
        for (const auto &value : list) {
            log << value << ',';
        }

        ASSERT_EQ(log.str(), "9,8,7,6,5,4,3,2,1,0,");
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}


TEST(app_lib_case, linked_int_test)
{
    std::ostringstream log;

    using custom_link_std_alloc = app::int_list<>;
    app::fill_and_print(log, custom_link_std_alloc{});


    ASSERT_EQ(log.str(),
              "0\n"
              "1\n"
              "2\n"
              "3\n"
              "4\n"
              "5\n"
              "6\n"
              "7\n"
              "8\n"
              "9\n");
}

TEST(app_lib_case, linked_int_custom_allocator_test)
{
    const auto counter = app::alloc_counter;
    {
        std::ostringstream log;

        using custom_link_custom_alloc = app::int_list<int, allocator::chunk_allocator<int>>;
        app::fill_and_print(log, custom_link_custom_alloc{}) ;

        ASSERT_EQ(log.str(),
                  "0\n"
                  "1\n"
                  "2\n"
                  "3\n"
                  "4\n"
                  "5\n"
                  "6\n"
                  "7\n"
                  "8\n"
                  "9\n");
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}


TEST(app_lib_case, map_int_test)
{
    std::ostringstream log;
    app::fill_and_print(log, app::int_map<>{});

    ASSERT_EQ(log.str(),
              "0 1\n"
              "1 1\n"
              "2 2\n"
              "3 6\n"
              "4 24\n"
              "5 120\n"
              "6 720\n"
              "7 5040\n"
              "8 40320\n"
              "9 362880\n"
              );
}



TEST(app_lib_case, map_int_custom_allocator_test)
{

    const auto counter = app::alloc_counter;
    {
        std::ostringstream log;

        using map_custom_alloc = app::int_map<int, allocator::chunk_allocator<std::pair<const int, int>>>;
        app::fill_and_print(log, map_custom_alloc{});

        ASSERT_EQ(log.str(),
                  "0 1\n"
                  "1 1\n"
                  "2 2\n"
                  "3 6\n"
                  "4 24\n"
                  "5 120\n"
                  "6 720\n"
                  "7 5040\n"
                  "8 40320\n"
                  "9 362880\n"
                  );
    }
    const auto after_counter = app::alloc_counter;
    ASSERT_EQ(counter, after_counter);
}


int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
