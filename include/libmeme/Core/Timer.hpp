#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <libmeme/Core/Duration.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Timer final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clock = typename std::chrono::high_resolution_clock;

		using TimePoint = typename Clock::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Timer(bool startMe = false) noexcept
			: m_running	{ startMe }
			, m_previous{ Clock::now() }
			, m_current	{ m_previous }
			, m_elapsed	{ 0.0 }
		{
		}

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