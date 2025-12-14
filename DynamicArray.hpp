#pragma once
#include <utility>
#include <cassert>
#include <cstdlib>
template<typename T>
class DynamicArray
{
private:
	void check_capacity()
	{
		if (capacity_ == length_) {
			capacity_ = (capacity_ <= 0) ? default_capacity_ : capacity_ * 2 + 1;
			T* new_buf = static_cast<T*>(malloc(capacity_ * sizeof(T)));
			for (int i = 0; i < length_; ++i) {
				new (new_buf + i) T(std::move(buf_[i]));
				buf_[i].~T();
			}
			free(buf_);
			buf_ = new_buf;
		}
	}
	T* buf_;
	int length_;
	int capacity_;
	const int default_capacity_ = 4;
public:
	#pragma region Iterators
	class Iterator {
	private:
		T* ptr_;
		int index_;
		int size_;

	public:
		Iterator(T* ptr, int index, int size)
			: ptr_(ptr), index_(index), size_(size) {
		}

		const T& get() const {
			return ptr_[index_];
		}

		void set(const T& value) {
			ptr_[index_] = value;
		}

		void next() {
			if (hasNext()) {
				++index_;
			}
		}

		bool hasNext() const {
			return index_ < size_ - 1;
		}
	};
	class ReverseIterator {
	private:
		T* ptr_;
		int index_;
		int size_;

	public:
		ReverseIterator(T* ptr, int index, int size)
			: ptr_(ptr), index_(index), size_(size) {
		}

		const T& get() const {
			return ptr_[index_];
		}

		void set(const T& value) {
			ptr_[index_] = value;
		}

		void next() {
			if (hasNext()) {
				--index_;
			}
		}

		bool hasNext() const {
			return index_ > 0;
		}
	};
	#pragma endregion
	DynamicArray():
		capacity_{ default_capacity_ },
		length_{ 0 }
	{
		buf_ = static_cast<T*>(malloc((capacity_) * sizeof(T)));
	}
	explicit DynamicArray(int capacity):
		capacity_{ capacity },
		length_{ 0 }
	{
		if (capacity_ <= 0) capacity_ = default_capacity_;
		buf_ = static_cast<T*>(malloc((capacity_) * sizeof(T)));
	}
	~DynamicArray()
	{
		if (buf_) {
			clear();
			free(buf_);
		}
	}

	DynamicArray(const DynamicArray& other)
		: capacity_(other.capacity_), length_(other.length_) {
		buf_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));

		for (int i = 0; i < length_; ++i) {
			new (buf_ + i) T(other.buf_[i]);
		}
	}

	DynamicArray(DynamicArray&& other) noexcept
		: buf_(other.buf_), length_(other.length_), capacity_(other.capacity_) {
		other.buf_ = nullptr;
		other.length_ = 0;
		other.capacity_ = 0;
	}

	void swap(DynamicArray& other) noexcept {
		std::swap(buf_, other.buf_);
		std::swap(length_, other.length_);
		std::swap(capacity_, other.capacity_);
	}

	//copy swap
	DynamicArray& operator=(DynamicArray other) noexcept {
		swap(other);
		return *this;
	}

	int insert(const T& value)
	{
		check_capacity();
		new (buf_ + length_) T(value);
		length_ += 1;
		return length_ - 1;
	}
	int insert(int index, const T& value) {
		assert(index >= 0 && index <= length_);
		check_capacity();
		//Теперь проверяем на мув семантику
		if (std::is_move_constructible_v<T>) {
			for (int i = length_; i > index; --i) {
				new (buf_ + i) T(std::move(buf_[i - 1]));
				buf_[i - 1].~T();
			}
		}

		else {
			for (int i = length_; i > index; --i) {
				new (buf_ + i) T(buf_[i - 1]);
				buf_[i - 1].~T();
			}
		}

		new (buf_ + index) T(value);
		length_ += 1;

		return index;
	}

	int insert(T&& value) {
		check_capacity();
		new (buf_ + length_) T(std::move(value));
		length_ += 1;
		return length_ - 1;
	}

	int insert(int index, T&& value) {
		assert(index >= 0 && index <= length_);

		check_capacity();
		//Теперь проверяем на мув семантику
		if (std::is_move_constructible_v<T>) {
			for (int i = length_; i > index; --i) {
				new (buf_ + i) T(std::move(buf_[i - 1]));
				buf_[i - 1].~T();
			}
		}

		else {
			for (int i = length_; i > index; --i) {
				new (buf_ + i) T(buf_[i - 1]);
				buf_[i - 1].~T();
			}
		}

		new (buf_ + index) T(std::move(value));
		length_ += 1;

		return index;
	}

	void remove(int index) {
		assert(index >= 0 && index < length_);

		buf_[index].~T();

		if (std::is_move_constructible_v<T>) {
			for (int i = index; i < length_ - 1; ++i) {
				new (buf_ + i) T(std::move(buf_[i + 1]));
				buf_[i + 1].~T();
			}
		}
		else {
			for (int i = index; i < length_ - 1; ++i) {
				new (buf_ + i) T(buf_[i + 1]);
				buf_[i + 1].~T();
			}
		}

		length_ -= 1;
	}

	T& operator[](int index) {
		assert(index >= 0 && index < length_);
		return buf_[index];
	}

	const T& operator[](int index) const {
		assert(index >= 0 && index < length_);
		return buf_[index];
	}

	int size() const {
		return length_;
	}

	void clear() {
		for (int i = 0; i < length_; ++i) {
			buf_[i].~T();
		}
		length_ = 0;
	}

	T* begin() {
		return buf_;
	}

	T* end() {
		return buf_ + length_;
	}

	Iterator iterator() {
		return Iterator(buf_, 0, length_);
	}

	Iterator iterator() const {
		return Iterator(const_cast<T*>(buf_), 0, length_);
	}

	ReverseIterator reverseIterator() {
		return ReverseIterator(buf_, length_ - 1, length_);
	}

	ReverseIterator reverseIterator() const {
		return ReverseIterator(const_cast<T*>(buf_), length_ - 1, length_);
	}
};