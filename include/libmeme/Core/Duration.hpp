#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <libmeme/Core/Utility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias nanoseconds_t	= typename chrono::duration<float64_t, std::nano>;
	ML_alias microseconds_t	= typename chrono::duration<float64_t, std::micro>;
	ML_alias milliseconds_t	= typename chrono::duration<float64_t, std::milli>;
	ML_alias seconds_t		= typename chrono::duration<float64_t, std::ratio<1>>;
	ML_alias minutes_t		= typename chrono::duration<float64_t, std::ratio<60>>;
	ML_alias hours_t		= typename chrono::duration<float64_t, std::ratio<60 * 60>>;
	ML_alias days_t			= typename chrono::duration<float64_t, std::ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct duration final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename duration;

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

		constexpr explicit duration(base_type const & value)
			: m_base{ value }
		{
		}

		constexpr explicit duration(base_type && value) noexcept
			: m_base{ std::move(value) }
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

		constexpr duration(duration const & value)
			: m_base{ value.m_base }
		{
		}

		constexpr duration(duration && value) noexcept
			: m_base{ std::move(value.m_base) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr duration & operator=(duration const & value)
		{
			duration temp{ value };
			swap(temp);
			return (*this);
		}

		constexpr duration & operator=(duration && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		constexpr void swap(duration & value) noexcept
		{
			if (this != std::addressof(value))
			{
				util::swap(m_base, value.m_base);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr base_type const & base() const & noexcept
		{
			return m_base;
		}

		template <class OutType = float64_t
		> ML_NODISCARD constexpr auto count() const noexcept
		{
			return static_cast<OutType>(m_base.count());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr nanoseconds_t nanoseconds() const noexcept
		{
			return chrono::duration_cast<nanoseconds_t>(m_base);
		}

		ML_NODISCARD constexpr microseconds_t microseconds() const noexcept
		{
			return chrono::duration_cast<microseconds_t>(m_base);
		}

		ML_NODISCARD constexpr milliseconds_t milliseconds() const noexcept
		{
			return chrono::duration_cast<milliseconds_t>(m_base);
		}

		ML_NODISCARD constexpr seconds_t seconds() const noexcept
		{
			return chrono::duration_cast<seconds_t>(m_base);
		}

		ML_NODISCARD constexpr minutes_t minutes() const noexcept
		{
			return chrono::duration_cast<minutes_t>(m_base);
		}

		ML_NODISCARD constexpr hours_t hours() const noexcept
		{
			return chrono::duration_cast<hours_t>(m_base);
		}

		ML_NODISCARD constexpr days_t days() const noexcept
		{
			return chrono::duration_cast<days_t>(m_base);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = duration
		> ML_NODISCARD constexpr auto compare(U const & value) const noexcept
		{
			if constexpr (std::is_same_v<U, duration>)
			{
				return compare(value.m_base);
			}
			else
			{
				return util::compare(m_base, value);
			}
		}

		template <class U = self_type
		> ML_NODISCARD bool operator==(U const & value) const noexcept
		{
			return compare(value) == 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator!=(U const & value) const noexcept
		{
			return compare(value) != 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator<(U const & value) const noexcept
		{
			return compare(value) < 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator>(U const & value) const noexcept
		{
			return compare(value) > 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator<=(U const & value) const noexcept
		{
			return compare(value) <= 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator>=(U const & value) const noexcept
		{
			return compare(value) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr duration operator+() const noexcept
		{
			return duration{ m_base.operator+() };
		}

		constexpr duration operator-() const noexcept
		{
			return duration{ m_base.operator-() };
		}

		constexpr duration operator++(int) noexcept
		{
			return duration{ m_base++ };
		}

		constexpr duration operator--(int) noexcept
		{
			return duration{ m_base-- };
		}

		constexpr duration & operator++() noexcept
		{
			++m_base;
			return (*this);
		}

		constexpr duration & operator--() noexcept
		{
			--m_base;
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> constexpr duration & operator+=(T const & value) noexcept
		{
			m_base += value;
			return (*this);
		}

		template <class T
		> constexpr duration & operator-=(T const & value) noexcept
		{
			m_base -= value;
			return (*this);
		}

		template <class T
		> constexpr duration & operator*=(T const & value) noexcept
		{
			m_base *= value;
			return (*this);
		}

		template <class T
		> constexpr duration & operator/=(T const & value) noexcept
		{
			m_base /= value;
			return (*this);
		}

		template <class T
		> constexpr duration & operator%=(T const & value) noexcept
		{
			m_base %= value;
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { base_type m_base; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_