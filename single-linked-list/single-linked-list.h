#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <initializer_list>
#include <iostream>
#include <vector>

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
			assert(node_ != nullptr)
			node_ = node_->next_node;
			return *this;
		}

		BasicIterator operator++(int) noexcept {
			auto old_value{ *this };
			++(*this);
			return old_value;
		}

		// Конечно глупость проверять операторы разыменования
		// но вы придумали такую отличную вещь как 
		// before_begin() поэтому естественно проверить надо
		reference operator*() const noexcept {
			assert(node_ != nullptr);
			return node_->value;
		}

		pointer operator->() const noexcept {
			assert(node_ != nullptr);
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

	// Соответствеющий метод вставки PushFront
	// предоставляет публичный контракт для вставки вначало
	// => при переданном (1,2,3,4,5) он вставит (5,4,3,2,1)
	// чтобы использовать его придется делать развороты и т.д.
	// что является оверхедом, что считается моветоном
	SingleLinkedList(std::initializer_list<Type> values)
		: size_{ values.size() } {
		CreateList({ values.begin(), values.end() });
	}
	
	SingleLinkedList(const SingleLinkedList& other)
		: head_{}, size_{ other.size_ } {
		//std::vector<Type> elements{};
		//elements.reserve(other.size_);
		//
		//for (const auto& node : other) {
		//	elements.push_back(node->node_);
		//}
		CreateList({ other.begin(), other.end() });
	}

	~SingleLinkedList() {
		Clear();
	}

	void PushFront(const Type& value) {
		head_.next_node = new Node{ value, head_.next_node };
		++size_;
	}

	void Clear() noexcept {
		while (head_.next_node != nullptr) {
			PopFront();
		}

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

	// Проверять валидность фиктивного нода
	// Пожалуй нет
	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		Iterator iter{ before_begin() };

		while (iter != pos) {
			++iter;
		}

		iter.node_.next_node = new Node{ value, iter->next_node };
		++size_;

		return iter.node_->next_node;
	}

	void PopFront() noexcept {
		EraseAfter(ConstIterator(head.next_node))
		--size_;
	}

	Iterator EraseAfter(ConstIterator pos) noexcept {
		Iterator iter{ before_begin() };

		while (iter != pos) {
			++iter;
		}

		DeleteNode(iter.node_->next_node);
		--size_;

		return iter.node_->next_node;
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
	void DeleteNode(Node* node) {
		assert(node != nullptr);
		auto* to_delete = node;
		node = node->next_node;
		delete to_delete;
	}
	void CreateList(const std::vector<Type>& elements) {
		Iterator iter{ before_begin()};
		for (const auto& value : elements) {
			iter.node_->next_node = new Node{ value, nullptr };
			++iter;
		}
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
