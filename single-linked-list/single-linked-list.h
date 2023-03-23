#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <initializer_list>
#include <iostream>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList<Type>;

        explicit BasicIterator(Node* node)
            : node_{ node } {}
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_{ other.node_ } {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (node_ == rhs.node_);
        }

        bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (node_ == rhs.node_);
        }

        bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value{ *this };
            ++(*this);
            return old_value;
        }

        reference operator*() const noexcept {
            return node_->value;
        }

        pointer operator->() const noexcept {
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };
public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList()
        : head_{}, size_{} {}

    SingleLinkedList(std::initializer_list<Type> values)
        : size_{ values.size() } {
        auto* temp{ &head_ };
        for (const auto& value : values) {
            temp->next_node = new Node{ value, nullptr };
            temp = temp->next_node;
        }
    }

    SingleLinkedList(const SingleLinkedList& other)
        : head_{}, size_{ other.size_ } {
        auto* temp{ &head_ };
        for (auto iter{ other.begin() }; iter != other.end(); ++iter) {
            temp->next_node = new Node{ *iter, nullptr };
            temp = temp->next_node;
        }
    }

    ~SingleLinkedList() {
        Clear();
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node{ value, head_.next_node };
        ++size_;
    }

    void Clear() noexcept {
        auto* iter{ head_.next_node };
        while (iter != nullptr) {
            auto* to_delete = iter;
            iter = iter->next_node;
            delete to_delete;
        }
        head_.next_node = nullptr;
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return before_begin();
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        auto* iter{ &head_ };

        while (ConstIterator(iter) != pos) {
            iter = iter->next_node;
        }

        iter->next_node = new Node{ value, iter->next_node };
        ++size_;

        return Iterator(iter->next_node);
    }

    void PopFront() noexcept {
        auto* ptr_to_next{ head_.next_node->next_node };
        delete head_.next_node;
        head_.next_node = ptr_to_next;
        --size_;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        auto* iter{ &head_ };

        while (ConstIterator(iter) != pos) {
            iter = iter->next_node;
        }

        auto* ptr_to_next{ iter->next_node->next_node };
        delete iter->next_node;
        iter->next_node = ptr_to_next;
        --size_;

        return Iterator(iter->next_node);
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this == &rhs) {
            return *this;
        }

        SingleLinkedList copy{ rhs };

        swap(copy);

        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }
private:
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
