#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <libmeme/Core/Duration.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Clock = chrono::high_resolution_clock
	> struct basic_timer final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using clock_type = typename _Clock;
		using time_point = typename clock_type::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_timer(bool start_me = {}) noexcept
		{
			if (start_me)
			{
				this->start();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD duration const & elapsed() const & noexcept
		{
			return m_running ? (m_elapsed = (clock_type::now() - m_previous)) : m_elapsed;
		}

		ML_NODISCARD bool running() const noexcept
		{
			return m_running;
		}

		basic_timer & start() & noexcept
		{
			if (!m_running)
			{
				m_current = m_previous = clock_type::now();
				m_elapsed = 0.0;
				m_running = true;
			}
			return (*this);
		}

		basic_timer & stop() & noexcept
		{
			if (m_running)
			{
				m_current = clock_type::now();
				m_elapsed = (m_current - m_previous);
				m_running = false;
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool				m_running	{};
		time_point			m_previous	{};
		time_point			m_current	{};
		mutable duration	m_elapsed	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias timer = typename basic_timer<>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_