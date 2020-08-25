#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <libmeme/System/Memory.hpp>

// duration
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

	struct duration final : public chrono::duration<float64_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using chrono::duration<float64_t>::duration;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr nanoseconds_t nanoseconds() const noexcept
		{
			return chrono::duration_cast<nanoseconds_t>(*this);
		}

		ML_NODISCARD constexpr microseconds_t microseconds() const noexcept
		{
			return chrono::duration_cast<microseconds_t>(*this);
		}

		ML_NODISCARD constexpr milliseconds_t milliseconds() const noexcept
		{
			return chrono::duration_cast<milliseconds_t>(*this);
		}

		ML_NODISCARD constexpr seconds_t seconds() const noexcept
		{
			return chrono::duration_cast<seconds_t>(*this);
		}

		ML_NODISCARD constexpr minutes_t minutes() const noexcept
		{
			return chrono::duration_cast<minutes_t>(*this);
		}

		ML_NODISCARD constexpr hours_t hours() const noexcept
		{
			return chrono::duration_cast<hours_t>(*this);
		}

		ML_NODISCARD constexpr days_t days() const noexcept
		{
			return chrono::duration_cast<days_t>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	static void to_json(json & j, duration const & v)
	{
		j = v.count();
	}

	static void from_json(json const & j, duration & v)
	{
		j.get_to((float64_t &)v);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// timer
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Clock
	> struct basic_timer final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using clock_type	= typename _Clock;
		using self_type		= typename basic_timer<clock_type>;
		using time_point	= typename clock_type::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_timer(bool running = true) noexcept
			: m_running		{ running }
			, m_start_time	{ clock_type::now() }
			, m_stop_time	{ m_start_time }
			, m_elapsed		{}
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool running() const noexcept
		{
			return m_running;
		}

		ML_NODISCARD duration elapsed() const noexcept
		{
			return m_running ? (clock_type::now() - m_start_time) : m_elapsed;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & start() & noexcept
		{
			return m_running ? (*this) : this->restart();
		}

		self_type & restart() & noexcept
		{
			m_running = true;
			m_start_time = m_stop_time = clock_type::now();
			m_elapsed = {};
			return (*this);
		}

		self_type & stop() & noexcept
		{
			if (m_running)
			{
				m_running = false;
				m_elapsed = ((m_stop_time = clock_type::now()) - m_start_time);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_running	; // 
		time_point	m_start_time; // 
		time_point	m_stop_time	; // 
		duration	m_elapsed	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias timer = typename basic_timer<chrono::high_resolution_clock>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_