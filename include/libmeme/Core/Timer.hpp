#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <libmeme/Core/Duration.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Timer final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clock = typename std::chrono::high_resolution_clock;

		using TimePoint = typename Clock::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Timer(bool startMe) noexcept
			: m_running{ startMe }
			, m_previous{ Clock::now() }
			, m_current{ m_previous }
			, m_elapsed{ 0.0 }
		{
		}

		Timer(Timer const & other) noexcept
			: m_running{ other.m_running }
			, m_previous{ other.m_previous }
			, m_current{ other.m_current }
			, m_elapsed{ other.m_elapsed }
		{
		}

		~Timer() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Timer & start() noexcept
		{
			if (!m_running)
			{
				m_previous = m_current = Clock::now();

				m_elapsed = 0.0;

				m_running = true;
			}
			return (*this);
		}

		inline Timer & stop() noexcept
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

		ML_NODISCARD inline bool running() const noexcept
		{
			return m_running;
		}

		ML_NODISCARD inline Duration const & elapsed() const noexcept
		{
			return m_running ? (m_elapsed = (Clock::now() - m_previous)) : m_elapsed;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool m_running;

		TimePoint m_previous;

		TimePoint m_current;

		mutable Duration m_elapsed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_