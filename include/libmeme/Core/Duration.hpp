#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <libmeme/Common.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using nanoseconds_t		= typename chrono::duration<float64_t, std::nano>;
	using microseconds_t	= typename chrono::duration<float64_t, std::micro>;
	using milliseconds_t	= typename chrono::duration<float64_t, std::milli>;
	using seconds_t			= typename chrono::duration<float64_t, std::ratio<1>>;
	using minutes_t			= typename chrono::duration<float64_t, std::ratio<60>>;
	using hours_t			= typename chrono::duration<float64_t, std::ratio<60 * 60>>;
	using days_t			= typename chrono::duration<float64_t, std::ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct duration final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename chrono::duration<float64_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr duration() noexcept
			: m_base{ 0.0 }
		{
		}

		constexpr duration(float64_t const value) noexcept
			: m_base{ value }
		{
		}

		constexpr explicit duration(base_type const & other)
			: m_base{ other }
		{
		}

		constexpr explicit duration(base_type && other) noexcept
			: m_base{ std::move(other) }
		{
		}

		template <class R, class P = typename R::period
		> constexpr duration(chrono::duration<R, P> const & value)
			: m_base{ chrono::duration_cast<base_type>(value) }
		{
		}

		template <class R, class P = typename R::period
		> constexpr duration(chrono::duration<R, P> && value) noexcept
			: m_base{ chrono::duration_cast<base_type>(std::move(value)) }
		{
		}

		constexpr duration(duration const & other)
			: m_base{ other.m_base }
		{
		}

		constexpr duration(duration && other) noexcept
			: m_base{ std::move(other.m_base) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr duration & operator=(duration const & other)
		{
			duration temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr duration & operator=(duration && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(duration & other) noexcept
		{
			if (this != std::addressof(other))
			{
				base_type temp = std::move(m_base);
				m_base = std::move(other.m_base);
				other.m_base = std::move(m_base);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr base_type base() const noexcept
		{
			return m_base;
		}

		template <class T = float64_t
		> ML_NODISCARD constexpr T count() const noexcept
		{
			return static_cast<T>(m_base.count());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto nanoseconds() const noexcept
		{
			return chrono::duration_cast<nanoseconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto microseconds() const noexcept
		{
			return chrono::duration_cast<microseconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto milliseconds() const noexcept
		{
			return chrono::duration_cast<milliseconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto seconds() const noexcept
		{
			return chrono::duration_cast<seconds_t>(m_base);
		}

		ML_NODISCARD constexpr auto minutes() const noexcept
		{
			return chrono::duration_cast<minutes_t>(m_base);
		}

		ML_NODISCARD constexpr auto hours() const noexcept
		{
			return chrono::duration_cast<hours_t>(m_base);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD constexpr bool operator==(T const & other) const
		{
			return (m_base == other.m_base);
		}

		template <class T
		> ML_NODISCARD constexpr bool operator!=(T const & other) const
		{
			return (m_base != other.m_base);
		}

		template <class T
		> ML_NODISCARD constexpr bool operator<(T const & other) const
		{
			return (m_base < other.m_base);
		}

		template <class T
		> ML_NODISCARD constexpr bool operator>(T const & other) const
		{
			return (m_base > other.m_base);
		}

		template <class T
		> ML_NODISCARD constexpr bool operator<=(T const & other) const
		{
			return (m_base <= other.m_base);
		}

		template <class T
		> ML_NODISCARD constexpr bool operator>=(T const & other) const
		{
			return (m_base >= other.m_base);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { base_type m_base; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_