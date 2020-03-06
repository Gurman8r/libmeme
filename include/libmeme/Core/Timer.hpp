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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline timer & start() noexcept
		{
			if (!this->is_initialized())
			{
				m_current = m_previous = clock::now();
				m_elapsed = 0.0;
				m_flags |= TimerFlags_Running;
			}
			return (*this);
		}

		inline timer & stop() noexcept
		{
			if (this->is_initialized())
			{
				m_current = clock::now();
				m_elapsed = (m_current - m_previous);
				m_flags &= ~TimerFlags_Running;
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool is_initialized() const noexcept
		{
			return (m_flags & TimerFlags_Running);
		}

		ML_NODISCARD inline duration const & elapsed() const & noexcept
		{
			return this->is_initialized() ? (m_elapsed = (clock::now() - m_previous)) : m_elapsed;
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