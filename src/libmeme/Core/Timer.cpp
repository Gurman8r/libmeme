#include <libmeme/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Timer::Timer(bool startMe) noexcept
		: m_running{ startMe }
		, m_previous{ Clock::now() }
		, m_current{ m_previous }
		, m_elapsed{ 0.0 }
	{
	}

	Timer::Timer(Timer const & other) noexcept
		: m_running{ other.m_running }
		, m_previous{ other.m_previous }
		, m_current{ other.m_current }
		, m_elapsed{ other.m_elapsed }
	{
	}

	Timer::~Timer() noexcept {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Duration const & Timer::elapsed() const noexcept
	{
		return m_running ? (m_elapsed = (Clock::now() - m_previous)) : m_elapsed;
	}

	Timer & Timer::start() noexcept
	{
		if (!m_running)
		{
			m_previous = m_current = Clock::now();

			m_elapsed = 0.0;

			m_running = true;
		}
		return (*this);
	}

	Timer & Timer::stop() noexcept
	{
		if (m_running)
		{
			m_current = Clock::now();

			m_elapsed = (m_current - m_previous);

			m_running = false;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}