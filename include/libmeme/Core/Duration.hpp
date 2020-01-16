#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <libmeme/Common.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using nanoseconds_t		= typename std::chrono::duration<float64_t, nano_t>;
	using microseconds_t	= typename std::chrono::duration<float64_t, micro_t>;
	using milliseconds_t	= typename std::chrono::duration<float64_t, milli_t>;
	using seconds_t			= typename std::chrono::duration<float64_t, ratio_t<1>>;
	using minutes_t			= typename std::chrono::duration<float64_t, ratio_t<60>>;
	using hours_t			= typename std::chrono::duration<float64_t, ratio_t<60 * 60>>;
	using days_t			= typename std::chrono::duration<float64_t, ratio_t<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Duration final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename Duration;
		using base_type = typename std::chrono::duration<float64_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration() noexcept
			: m_base{ 0.0 }
		{
		}

		constexpr Duration(float64_t const value) noexcept
			: m_base{ value }
		{
		}

		constexpr explicit Duration(base_type const & other)
			: m_base{ other }
		{
		}

		constexpr explicit Duration(base_type && other) noexcept
			: m_base{ std::move(other) }
		{
		}

		template <class R, class P = typename R::period
		> constexpr Duration(std::chrono::duration<R, P> const & value)
			: m_base{ std::chrono::duration_cast<base_type>(value) }
		{
		}

		template <class R, class P = typename R::period
		> constexpr Duration(std::chrono::duration<R, P> && value) noexcept
			: m_base{ std::chrono::duration_cast<base_type>(std::move(value)) }
		{
		}

		constexpr Duration(self_type const & other)
			: m_base{ other.m_base }
		{
		}

		constexpr Duration(self_type && other) noexcept
			: m_base{ std::move(other.m_base) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			Duration temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				base_type temp = std::move(m_base);
				m_base = std::move(other.m_base);
				other.m_base = std::move(m_base);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr base_type base() const noexcept { return m_base; }

		ML_NODISCARD constexpr float64_t count() const noexcept { return m_base.count(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto nanoseconds() const noexcept
		{
			return std::chrono::duration_cast<nanoseconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto microseconds() const noexcept
		{
			return std::chrono::duration_cast<microseconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto milliseconds() const noexcept
		{
			return std::chrono::duration_cast<milliseconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto seconds() const noexcept
		{
			return std::chrono::duration_cast<seconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto minutes() const noexcept
		{
			return std::chrono::duration_cast<minutes_t>(m_base);
		}

		ML_NODISCARD constexpr auto hours() const noexcept
		{
			return std::chrono::duration_cast<hours_t>(m_base);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { base_type m_base; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_