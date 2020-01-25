#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <libmeme/Core/Duration.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API timer final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using clock = typename chrono::high_resolution_clock;

		using time_point = typename clock::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		timer(bool start_me = false) noexcept
			: m_running	{}
			, m_current	{}
			, m_previous{}
			, m_elapsed	{ 0.0 }
		{
			if (start_me) { start(); }
		}

		timer(timer && other) noexcept
			: m_running	{}
			, m_current	{}
			, m_previous{}
			, m_elapsed	{}
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline timer & operator=(timer && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(timer & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_running, other.m_running);
				std::swap(m_current, other.m_current);
				std::swap(m_previous, other.m_previous);
				std::swap(m_elapsed, other.m_elapsed);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool running() const noexcept
		{
			return m_running;
		}

		ML_NODISCARD inline duration const & elapsed() const noexcept
		{
			return m_running ? (m_elapsed = (clock::now() - m_previous)) : m_elapsed;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline timer & start() noexcept
		{
			if (!m_running)
			{
				m_current = m_previous = clock::now();
				m_elapsed = 0.0;
				m_running = true;
			}
			return (*this);
		}

		inline timer & stop() noexcept
		{
			if (m_running)
			{
				m_current = clock::now();
				m_elapsed = (m_current - m_previous);
				m_running = false;
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline timer & restart() noexcept
		{
			return stop().start();
		}

		inline timer & wait(duration const & t) noexcept
		{
			while (elapsed() < t);
			return (*this);
		}

		template <class Fn
		> inline timer & wait_then(duration const & t, Fn fn) noexcept
		{
			wait(t);
			std::invoke(fn);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool m_running;
		time_point m_previous;
		time_point m_current;
		mutable duration m_elapsed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_