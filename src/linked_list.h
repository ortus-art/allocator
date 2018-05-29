#pragma once

#include <memory>
#include <functional>

namespace allocator {


//Basic forward only linked list
template <typename T, typename Alloc = std::allocator<T>>
class linked_list
{
    //Forward declaration
    struct deleter;
    struct node;
    using unique_ptr = std::unique_ptr<node, deleter>;
  //Class types
  struct node
  {
      node() {}
    T value;
    unique_ptr next;
  };
  public:
    class iterator
    {
        friend class linked_list;
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = typename std::pointer_traits<pointer>::difference_type;
        using iterator_category = std::forward_iterator_tag;

        iterator& operator=(const iterator& value) {
            node_ = value.node_;
            return *this;
        }
        iterator& operator=(iterator&& value) {
            std::swap(node_, value.node_);
            return *this;
        }
        bool operator==(const iterator &value) const {
            if (this == &value)
                return true;
            return   *node_ == *value.node_;
        }
        bool operator!=(const iterator &value) const {return !operator==(value);}

        iterator& operator++() {
            if(nullptr ==node_->get())
                throw std::range_error("operator ++ out of range");
            node_ = &(*node_)->next;
            return *this;
        }

        reference operator*() {return (*node_)->value;}
        iterator(iterator&& value) {std::swap(node_, value.node_); }
    private:
        iterator(unique_ptr * ptr): node_(ptr){}
        unique_ptr* node_;
    };
private:
    using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;

    using node_traits = std::allocator_traits<allocator_type>;
    using value_traits = std::allocator_traits<Alloc>;    
public:
    using value_type = T;
    using reference = T&;
    using difference_type = typename node_traits::difference_type;
    using size_type = typename node_traits::size_type;
private:
    unique_ptr          head_;
    unique_ptr          tail_;
    allocator_type      alloc_;
    size_type           size_;
    deleter             deleter_;
private:
    struct deleter
    {
        deleter(allocator_type  * alloc = nullptr) : alloc_(alloc){}
        allocator_type  * alloc_;

        void operator()(node * ptr)
        {
            if(nullptr != ptr && nullptr != alloc_)
            {
                node_traits::destroy(*alloc_, &(ptr->value));
                node_traits::destroy(*alloc_, &(ptr->next));
                node_traits::deallocate(*alloc_, ptr, 1);
            }

        }
    };
    template<typename... Args>
    auto make_node(Args&&... args)
    {
        unique_ptr ptr {node_traits::allocate(alloc_, 1), deleter(&alloc_)};
        node_traits::construct(alloc_, &(ptr->value), std::forward<Args>(args)...);
        node_traits::construct(alloc_, &(ptr->next), nullptr, deleter(&alloc_));
        return ptr;
    }



public:
    linked_list():
        head_(nullptr),
        tail_(nullptr),
        alloc_(),
        size_(0)
    {
        deleter_.alloc_ = &alloc_;
    }

    iterator insert_after(iterator pos, const T& value )
    {

        auto ptr = make_node(value);
        if(empty())
            head_.reset(ptr.release());
        else
        {
            if(pos.node_->get() != nullptr)
                ptr->next.reset((*pos.node_)->next.release());
            (*pos.node_).reset(ptr.release());
        }
        size_ ++;
        ++pos;
        return pos;
    }
    void push_front( const T& value )
    {

        auto ptr = make_node(value);
        if(!empty())
             ptr->next.reset(head_.release());
        head_.reset(ptr.release());

        ++size_;
    }
    void push_front(T&& value )
    {
        auto ptr = make_node(std::move(value));
        if(!empty())
             ptr->next.reset(head_.release());
        head_.reset(ptr.release());
        ++size_;
    }

    reference front() { return head_->value;}

    iterator begin() { return iterator(&head_);}
    iterator end() { return iterator(&tail_);}
    bool empty() const { return nullptr == head_;}


};

} //namespace allocator
