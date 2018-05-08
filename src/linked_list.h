#pragma once

#include <memory>


namespace allocator {


//Basic forward only linked list
template <typename T, typename Alloc = std::allocator<T>>
class linked_list
{
  //Class types
  struct node
  {
    T value;
    node *  next;
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
            return node_ == value.node_;
        }
        bool operator!=(const iterator &value) const {return !operator==(value);}

        iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        reference operator*() {return node_->value;}        
        iterator(iterator&& value) {std::swap(node_, value.node_); }
    private:
        iterator(node * ptr): node_(ptr){}
        node * node_;
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
    node *              head_;
    allocator_type      alloc_;
    size_type           size_;
private:   
    template<typename... Args>
    auto make_node(Args&&... args)
    {
        node * ptr = node_traits::allocate(alloc_, 1);
        ptr->next = nullptr;
        node_traits::construct(alloc_, &(ptr->value), std::forward<Args>(args)...);
        return ptr;
    }
public:
    linked_list():
        head_(nullptr),
        alloc_(),
        size_(0)
    {}
    ~linked_list()
    {
        node * ptr = nullptr;
        auto i = 0;
        while(head_)
        {
            i++;
            ptr = head_->next;
            node_traits::destroy(alloc_, &(head_->value));
            node_traits::deallocate(alloc_, head_, 1);
            head_ = ptr;
        }
    }

    iterator insert_after(iterator pos, const T& value )
    {

        auto ptr = make_node(value);
        if(empty())
            head_ = ptr;
        else
        {
            auto next = pos.node_->next;
            pos.node_->next = ptr;
            ptr->next = next;
        }

        return iterator(ptr);
    }
    void push_front( const T& value )
    {
        auto ptr = make_node(value);
        ptr->next = head_;
        head_ = ptr;
    }
    void push_front(T&& value )
    {
        auto ptr = make_node(std::move(value));
        ptr->next = head_;
        head_ = ptr;
    }

    reference front() { return head_->value;}

    iterator begin() const { return iterator(head_);}
    iterator end() const { return iterator(nullptr);}
    bool empty() const { return nullptr == head_;}


};

} //namespace allocator
