#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <bitset>
#include <array>
#include <list>
#include <climits>

#include <cassert>

namespace allocator {


enum class memory_strategy
{
    NONE,
    LIFO,
    FIFO
};

namespace _implementation {

template<memory_strategy Strategy, typename List, typename NodeManager>
struct remove_block{
    void operator()(List &, const NodeManager * const){}
};

template<typename List, typename NodeManager>
struct remove_block<memory_strategy::LIFO, List, NodeManager> {
    void operator()(List& list, const NodeManager * const ptr){
        assert(ptr != nullptr);
        auto it = list.begin(), end = list.end();
        for( ; it != end; it++)
        {
            if((*it).get() == ptr)
            {
                list.erase(it);
                break;
            }

        }
    }
};

template<typename List, typename NodeManager>
struct remove_block<memory_strategy::FIFO, List, NodeManager> {
    void operator()(List& list, const NodeManager * const  ptr){
        assert(ptr != nullptr);
        auto it = list.rbegin(), end = list.rend();
        for( ; it != end; it++)
        {
            if((*it).get() == ptr)
            {
                list.erase(std::next(it).base());
                break;
            }

        }
    }
};


}

template <typename T, size_t Size = 2, memory_strategy Strategy=memory_strategy::NONE >
class chunk_allocator {
   static_assert(Size > 1, "The chunck size should be at least 2 * 8 elements");
public:
   using value_type = T;
   using pointer = T *;
   using size_type = size_t;

private:
   template <typename Manager>
   struct node
   {
       T data;
       Manager * manager;
   };
   //Define types in specialization
   static constexpr const auto Chunk_size = Size * CHAR_BIT;
   //Managers allocated memory chunck
   class node_manager
   {

   public:
       using node_t = node<node_manager>;
       using node_array_t =  std::array<node_t, Chunk_size> ;

       node_manager()
       {

           for(auto i = 0; i < Chunk_size; i++)
               memory[i].manager = this;
       }
       bool operator==(const node_manager& value) {return *this == value;}
       pointer use_free_block(){
           auto i = 0, j = 0;
           for(; i < Size; i++)
               for(j = 0; j < CHAR_BIT; j++)
                   if ((bitset[i] & (1 << j)) == 0)
                   {
                       auto  index = CHAR_BIT*i;
                       index += j;
                       pointer result = &(memory[index].data);
                       set_used(i, j);
                       return result;
                   }
           return nullptr;
       }
       bool free_block(node_t * ptr){
           assert(nullptr != ptr);
           if(ptr >= &(memory[0]) && ptr <= &(memory[memory.size()-1]))
           {
            auto diff = ptr - memory.data();
            auto  index  = diff/CHAR_BIT;
            u_char unset_bit_mask =~(1 << (diff - index * CHAR_BIT));
            bitset[index] &= unset_bit_mask;
            return  true;
           }
           return false;
       }
       bool has_free() {
           for(auto i =0 ; i < Size; i++)
               if((bitset[i] & 0xFF) != 0xFF)
                   return true;
           return false;
       }
       bool empty() {
           for(auto i =0 ; i < Size; i++)
               if((bitset[i]) != 0)
                   return false;
           return false;
       }
   private:
       bool set_used(int bank, u_char bit){
           assert((Size > bank && 0 <= bank));
           assert(CHAR_BIT > bit);
           bitset[bank] |= (1 <<bit);
           return false;
       }
   private:
       //Map of block usage
       u_char bitset[Size] = {};
       node_array_t memory;

   };


public:
   template<typename U>
   struct rebind
   {
       using other = chunk_allocator<U, Size, Strategy>;
   };



   chunk_allocator() = default;

   template <class U> chunk_allocator (const chunk_allocator<U>&) noexcept {}
   pointer allocate (std::size_t n) {

     if(n > 1)
        throw std::invalid_argument( "Currently allocator supports only single cell allocation" );

    return get_free_block();
  }


  void deallocate (pointer p, std::size_t n) {
      if(n > 1)
           throw std::invalid_argument( "Currently allocator supports only single cell allocation" );

      auto * ptr = reinterpret_cast<typename node_manager::node_t*>(p);
      ptr->manager->free_block(ptr);
      if(ptr->manager->empty())
        _implementation::remove_block<Strategy, pool_t, node_manager>{}(pool_, ptr->manager);

  }

private:
  pointer get_free_block() {
    //Try to find the first available block;
    if(pool_.size())
    {
        auto it = pool_.rbegin(), end = pool_.rend();
        for( ; it != end; it++)
        {
            if((*it)->has_free())
                return (*it)->use_free_block();
        }
    }
    pool_.push_back(std::make_unique<node_manager>());
    return pool_.back()->use_free_block();
  }
private:  
    using pool_t =   std::list<std::unique_ptr<node_manager>>;
    pool_t pool_;


};

} //namespace allocator
