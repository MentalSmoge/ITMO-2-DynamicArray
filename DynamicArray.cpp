template<typename T>
class DynamicArray
{
public:
	DynamicArray()
		:capacity_{ 4 }
		, length_{ 0 }
	{
		buf_ = static_cast<T*>(malloc((capacity_) * sizeof(T)));
	}
	~DynamicArray()
	{
		if (buf_) {
			for (int i = 0; i < length_; ++i)
				buf_[i].~T();
			free(buf_);
		}
	}

	const T& add(const T& value)
	{
		check_capacity()
		new (p + length_) T(value);
		length_ += 1;
	}

	int add(T&& value) {
		check_capacity()
		new (p + length_) T(std::move(value));
		length_ += 1;
	}
private:
	void check_capacity()
	{
		if (capacity_ == length_) {
			capacity_ = capacity_ * 2 + 1;
			T* p = static_cast<T*>(malloc(capacity_ * sizeof(T)));
			for (int i = 0; i < length_; ++i) {
				new (p + i) T(std::move(buf_[i]));
				buf_[i].~T();
			}
			free(buf_);
			buf_ = p;
		}
	}
	T* buf_;
	int length_;
	int capacity_;
};