#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <libmeme/Core/Duration.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Clock
	> struct basic_timer final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using clock		= typename _Clock;
		using point		= typename clock::time_point;
		using self_type	= typename basic_timer<clock>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_timer(bool start_me = true) noexcept : m_running{ start_me }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool running() const noexcept
		{
			return m_running;
		}

		ML_NODISCARD duration const & elapsed() const & noexcept
		{
			return m_running ? (m_elapsed = (clock::now() - m_start_time)) : m_elapsed;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & start() & noexcept
		{
			return m_running ? (*this) : this->restart();
		}

		self_type & restart() & noexcept
		{
			m_running = true;
			m_stop_time = m_start_time = clock::now();
			m_elapsed = {};
			return (*this);
		}

		self_type & stop() & noexcept
		{
			if (m_running)
			{
				m_running = false;
				m_elapsed = ((m_stop_time = clock::now()) - m_start_time);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool	m_running			{};
		point	m_start_time		{ clock::now() };
		point	m_stop_time			{ m_start_time };
		
		mutable duration m_elapsed{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias timer = typename basic_timer<chrono::high_resolution_clock>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_