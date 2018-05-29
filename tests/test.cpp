#include <linked_list.h>
#include <chunk_allocator.h>
#include <gtest/gtest.h>

#include <sstream>


using namespace testing;


TEST(allocator_case, int_test)
{
    allocator::chunk_allocator<int> allocator;
    auto ptr = allocator.allocate(1);
    ASSERT_TRUE(nullptr != ptr);
    allocator.deallocate(ptr, 1);
}

TEST(allocator_case, catch_errors)
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



TEST(list_case, list_test)
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

TEST(list_case, list_pair_test)
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


TEST(main_case, allocator_test)
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

TEST(main_case, allocator_test_rval)
{
    using namespace allocator;
    linked_list<int, chunk_allocator<int>> list;
    for(auto i = 0; i < 10; i++)
        list.push_front(1);

    list.front() = 2;
    std::ostringstream log;
    for (const auto &value : list) {
        log << value << ',';
    }

    ASSERT_EQ(log.str(), "2,1,1,1,1,1,1,1,1,1,");
}

TEST(main_case, allocator_test_rval_calloc)
{
    using namespace allocator;
    linked_list<int, chunk_allocator<int, 2, allocator::memory_strategy::LIFO>> list;
    for(auto i = 0; i < 10; i++)
        list.push_front(1);

    list.front() = 2;
    std::ostringstream log;
    for (const auto &value : list) {
        log << value << ',';
    }

    ASSERT_EQ(log.str(), "2,1,1,1,1,1,1,1,1,1,");
}

TEST(main_case, allocator_test_insert_after)
{
    using namespace allocator;
    linked_list<int, chunk_allocator<int>> list;
    auto it = list.begin();
    for(auto i = 0; i < 20; i++)
        it = list.insert_after(std::move(it), i);

    std::ostringstream log;
    for (const auto &value : list) {
        log << value << ',';
    }

    ASSERT_EQ(log.str(), "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,");
}


TEST(main_case, allocator_test_iterator_lifo)
{
    using namespace allocator;
    linked_list<int, chunk_allocator<int, 2, allocator::memory_strategy::LIFO>> list;
    auto it = list.begin();
    for(auto i = 0; i < 20; i++)
        it = list.insert_after(std::move(it), i);

    std::ostringstream log;
    for (it = list.begin(); it != list.end(); ++it) {
        log << *it << ',';
    }

    ASSERT_EQ(log.str(), "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,");
}

TEST(main_case, allocator_test_iterator_fifo)
{
    using namespace allocator;
    linked_list<int, chunk_allocator<int, 2, allocator::memory_strategy::FIFO>> list;
    auto it = list.begin();
    for(auto i = 0; i < 20; i++)
        it = list.insert_after(std::move(it), i);

    std::ostringstream log;
    for (it = list.begin(); it != list.end(); ++it) {
        log << *it << ',';
    }

    ASSERT_EQ(log.str(), "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,");
}


int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
