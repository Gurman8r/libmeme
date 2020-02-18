#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <libmeme/Core/Duration.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum TimerFlags_ : int32_t
	{
		TimerFlags_None, TimerFlags_Running
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API timer final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using clock = typename chrono::high_resolution_clock;
		using point = typename clock::time_point;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		timer(bool start_me = false) noexcept
			: m_flags	{}
			, m_current	{}
			, m_previous{}
			, m_elapsed	{}
		{
			if (start_me)
			{
				this->start();
			}
		}

		timer(timer && other) noexcept
			: m_flags	{ std::move(other.m_flags) }
			, m_current	{ std::move(other.m_current) }
			, m_previous{ std::move(other.m_previous) }
			, m_elapsed	{ std::move(other.m_elapsed) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline timer & operator=(timer && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		inline void swap(timer & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_flags,		other.m_flags);
				std::swap(m_current,	other.m_current);
				std::swap(m_previous,	other.m_previous);
				std::swap(m_elapsed,	other.m_elapsed);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline timer & start() noexcept
		{
			if (!this->running())
			{
				m_current = m_previous = clock::now();
				m_elapsed = 0.0;
				m_flags |= TimerFlags_Running;
			}
			return (*this);
		}

		inline timer & stop() noexcept
		{
			if (this->running())
			{
				m_current = clock::now();
				m_elapsed = (m_current - m_previous);
				m_flags &= ~TimerFlags_Running;
			}
			return (*this);
		}

		inline timer & restart() noexcept
		{
			return this->stop().start();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool running() const noexcept
		{
			return (m_flags & TimerFlags_Running);
		}

		ML_NODISCARD inline duration const & elapsed() const & noexcept
		{
			return this->running() ? (m_elapsed = (clock::now() - m_previous)) : m_elapsed;
		}

		ML_NODISCARD inline operator duration const & () const & noexcept
		{
			return this->elapsed();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t m_flags;
		point	m_previous;
		point	m_current;
		mutable duration m_elapsed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_