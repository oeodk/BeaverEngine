#pragma once
#include "BeaverEngine/Core/std.h"

namespace bv
{
	template<typename T, size_t width, size_t height>
	class Array2D
	{
	public:
		Array2D()
		{
			data_.reserve(width * height);
		}
		~Array2D() = default;
		Array2D(const Array2D&) = default;
		Array2D& operator=(const Array2D&) = default;
		Array2D(Array2D&&) = default;
		Array2D& operator=(Array2D&&) = default;

		const T& at(size_t x, size_t y) const
		{
			return data_[width * x + y];
		}
		T& get(size_t x, size_t y)
		{
			return data_[width * x + y];
		}
		void set(size_t x, size_t y, const T& value)
		{
			data_[width * x + y] = value;
		}
	private:
		std::vector<T> data_{};
		T* data() { return data_.data(); }
	};
}