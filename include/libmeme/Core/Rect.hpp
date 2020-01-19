#ifndef _ML_RECT_HPP_
#define _ML_RECT_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct rectangle final : public tvec4<T>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type		= typename T;
		using self_type			= typename rectangle<value_type>;
		using base_type			= typename tvec4<value_type>;
		using coord_type		= typename tvec2<value_type>;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr rectangle() noexcept
			: base_type{}
		{
		}

		constexpr rectangle(T value) noexcept
			: base_type{ value }
		{
		}
		
		constexpr rectangle(T w, T h) noexcept
			: base_type{ 0, 0, w, h }
		{
		}
		
		constexpr rectangle(T x, T y, T w, T h) noexcept
			: base_type{ x, y, w, h }
		{
		}

		template <class X, class Y, class W, class H
		> constexpr rectangle(X x, Y y, W w, H h) noexcept : base_type{
			static_cast<T>(x),
			static_cast<T>(y),
			static_cast<T>(w),
			static_cast<T>(h)
		}
		{
		}

		constexpr rectangle(self_type const & other) noexcept
			: base_type{ other }
		{
		}

		template <class U
		> constexpr rectangle(tvec4<U> const & other) noexcept
			: base_type{ other }
		{
		}
		
		constexpr rectangle(coord_type const & pos, coord_type const & size) noexcept
			: base_type{ pos[0], pos[1], size[0], size[1] }
		{
		}
		
		constexpr rectangle(coord_type const & size) noexcept
			: base_type{ 0, 0, size[0], size[1] }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ML_NODISCARD constexpr auto left() const -> value_type { return at(0); }
		
		ML_NODISCARD constexpr auto top() const -> value_type { return at(1); }
		
		ML_NODISCARD constexpr auto width() const -> value_type { return at(2); }
		
		ML_NODISCARD constexpr auto height() const -> value_type { return at(3); }
		
		ML_NODISCARD constexpr auto bot() const -> value_type { return (top() + height()); }
		
		ML_NODISCARD constexpr auto right() const -> value_type { return (left() + width()); }
		
		ML_NODISCARD constexpr auto position() const -> coord_type { return { left(), top() }; }
		
		ML_NODISCARD constexpr auto size() const -> coord_type { return { width(), height() }; }
		
		ML_NODISCARD constexpr auto center() const -> coord_type { return (position() + (size() / (T)2)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto left(value_type value) -> self_type & { return set(0, value); }
		
		constexpr auto top(value_type value) -> self_type & { return set(1, value); }
		
		constexpr auto width(value_type value) -> self_type & { return set(2, value); }
		
		constexpr auto height(value_type value) -> self_type & { return set(3, value); }
		
		constexpr auto bot(value_type value) -> self_type & { return height(value - top()); }
		
		constexpr auto right(value_type value) -> self_type & { return width(value - left()); }
		
		constexpr auto position(coord_type const & value) -> self_type & { return left(value[0]).top(value[1]); }
		
		constexpr auto size(coord_type const & value) -> self_type & { return width(value[0]).height(value[1]); }
		
		constexpr auto center(coord_type const & value) -> self_type & { return position(value - (size() / (T)2)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING float_rect = rectangle<float_t>;
	ML_USING int_rect	= rectangle<int32_t>;
	ML_USING uint_rect	= rectangle<uint32_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RECT_HPP_