#include <linked_list.h>
#include <chunk_allocator.h>
#include <gtest/gtest.h>

#include <sstream>


using namespace testing;


TEST(main_case, list_test)
{
    allocator::linked_list<int> list;
    for(auto i = 0; i < 10; i++)
        list.push_front(i);

    std::ostringstream log;
    for (auto &value : list) {
        log << value << ',';
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
    for (auto &value : list) {
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
    for (auto &value : list) {
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
    for (auto &value : list) {
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
    for (auto &value : list) {
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
