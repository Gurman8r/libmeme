#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <libmeme/Core/Ratio.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Nanoseconds	= typename std::chrono::duration<float64_t, Nano>;
	using Microseconds	= typename std::chrono::duration<float64_t, Micro>;
	using Milliseconds	= typename std::chrono::duration<float64_t, Milli>;
	using Seconds		= typename std::chrono::duration<float64_t, Ratio<1>>;
	using Minutes		= typename std::chrono::duration<float64_t, Ratio<60>>;
	using Hours			= typename std::chrono::duration<float64_t, Ratio<60 * 60>>;
	using Days			= typename std::chrono::duration<float64_t, Ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * */

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

		constexpr Duration(float64_t value) noexcept
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

		constexpr base_type base() const noexcept { return m_base; }

		constexpr float64_t count() const noexcept { return m_base.count(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) nanoseconds() const noexcept
		{
			return std::chrono::duration_cast<Nanoseconds>(m_base);
		}

		constexpr decltype(auto) microseconds() const noexcept
		{
			return std::chrono::duration_cast<Microseconds>(m_base);
		}

		constexpr decltype(auto) milliseconds() const noexcept
		{
			return std::chrono::duration_cast<Milliseconds>(m_base);
		}

		constexpr decltype(auto) seconds() const noexcept
		{
			return std::chrono::duration_cast<Seconds>(m_base);
		}

		constexpr decltype(auto) minutes() const noexcept
		{
			return std::chrono::duration_cast<Minutes>(m_base);
		}

		constexpr decltype(auto) hours() const noexcept
		{
			return std::chrono::duration_cast<Hours>(m_base);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { base_type m_base; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, Duration const & dur)
	{
		const auto hr { (intmax_t)dur.hours().count() };
		const auto m { (intmax_t)dur.minutes().count() };
		const auto s { (intmax_t)dur.seconds().count() };
		const auto ms { (intmax_t)dur.milliseconds().count() };
		return out
			<< (((hr % 24) / 10) % 10) << ((hr % 24) % 10) << ':'
			<< (((m % 60) / 10) % 10) << ((m % 60) % 10) << ':'
			<< (((s % 60) / 10) % 10) << ((s % 60) % 10) << ':'
			<< ((ms % 1000) / 100) << ((ms % 100) / 10);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_