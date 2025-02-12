#pragma once
#include "BeaverEngine/Core/std.h"

namespace bv
{
	template<typename T>
	class Array2D
	{
	public:
		Array2D() = default;
		void setup(size_t width, size_t height)
		{
			width_ = width;
			height_ = height;
			data_.resize(width * height);
		}
		~Array2D() = default;
		Array2D(const Array2D&) = default;
		Array2D& operator=(const Array2D&) = default;
		Array2D(Array2D&&) = default;
		Array2D& operator=(Array2D&&) = default;

		const T& at(size_t x, size_t y) const
		{
			return data_[width_ * y + x];
		}
		T& get(size_t x, size_t y)
		{
			return data_[width_ * y + x];
		}
		void set(size_t x, size_t y, const T& value)
		{
			data_[width_ * y + x] = value;
		}

		size_t getHeight() const { return height_; }
		size_t getWidth() const { return width_; }

		bool exist(size_t x, size_t y) const
		{
			return x >= 0 && x < width_ && y >= 0 && y < height_;
		}

	private:
		size_t width_{}, height_{};

		std::vector<T> data_{};
		T* data() { return data_.data(); }
	};
}