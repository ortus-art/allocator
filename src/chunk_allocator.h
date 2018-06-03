#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <bitset>
#include <array>
#include <list>
#include <climits>
#include <limits>

namespace allocator {


enum class memory_strategy
{
    NONE,
    LIFO,
    FIFO
};

namespace impl {

template<memory_strategy Strategy, typename List, typename NodeManager>
struct remove_block{
    void operator()(List &, const NodeManager * const){}
};

template<typename List, typename NodeManager>
struct remove_block<memory_strategy::LIFO, List, NodeManager> {
    void operator()(List& list, const NodeManager * const ptr){
        if(nullptr == ptr)
            throw std::runtime_error("nullptr");
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
        if(nullptr == ptr)
            throw std::runtime_error("nullptr");
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

template <typename T, size_t Size = 10, memory_strategy Strategy=memory_strategy::NONE >
class chunk_allocator {
   static_assert(Size > 1, "The chunk size should be at least 2 * 8 elements");
public:
   using value_type = T;
   using pointer = T *;
   using size_type = size_t;

private:
   template <typename Manager>
   struct node
   {
       T data;
       bool used = false;
       Manager * manager = nullptr;
   };
   //Define types in specialization
   static constexpr const auto Chunk_size = Size * CHAR_BIT;   
   //Managers allocated memory chunk
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
           for(auto & item : memory)
                if(!item.used)
                   {
                        item.used = true;
                        pointer result = &item.data;
                        usage_counter++;
                        return result;
                   }
           return nullptr;
       }
       bool free_block(node_t * ptr){
           if(nullptr == ptr)
               throw std::runtime_error("nullptr");
           if(ptr >= &(memory[0]) && ptr <= &(memory[memory.size()-1]))
           {
            ptr->used = false;
            usage_counter--;
            return  true;
           }
           return false;
       }
       bool has_free() {
           return Chunk_size > usage_counter;
       }
       bool empty() {
           return 0 == usage_counter;
       }

   private:
       node_array_t memory;
       unsigned short usage_counter = 0; //Number of items is limited by this type
       static_assert(Chunk_size < std::numeric_limits<unsigned short>::max(), "Cannot be larger than unsigned short" );
   };


public:
   template<typename U>
   struct rebind
   {
       using other = chunk_allocator<U, Size, Strategy>;
   };



   chunk_allocator() = default;
   ~chunk_allocator(){
    pool_.clear();
   }

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
            impl::remove_block<Strategy, pool_t, node_manager>{}(pool_, ptr->manager);
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
