#ifndef _ML_TIME_MANAGER_HPP_
#define _ML_TIME_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>

namespace ml
{
	struct ML_ENGINE_API time_manager final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit time_manager(json const & j, allocator_type alloc = {}) noexcept;

		~time_manager() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto total() const & noexcept -> duration const & { return m_main_timer.elapsed(); }

		auto delta() const & noexcept -> duration const & { return m_delta_time; }

		auto frame_rate() const & noexcept -> float64_t const & { return m_frame_rate; }

		auto frame_count() const & noexcept -> uint64_t const & { return m_frame_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void begin_loop() noexcept
		{
			m_delta_time = m_loop_timer.elapsed();

			m_loop_timer.restart();
		}

		void end_loop() noexcept
		{
			++m_frame_count;

			m_frame_rate = ([&, dt = m_delta_time.count()]() noexcept
			{
				m_fps_accum += dt - m_fps_frames[m_fps_index];

				m_fps_frames[m_fps_index] = dt;

				m_fps_index = (m_fps_index + 1) % ML_arraysize(m_fps_frames);

				return (m_fps_accum > 0.0)
					? 1.0 / (m_fps_accum / (float64_t)ML_arraysize(m_fps_frames))
					: FLT_MAX;
			})();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		timer		m_main_timer{};
		timer		m_loop_timer{ false };
		
		duration	m_delta_time{};
		float64_t	m_frame_rate{};
		uint64_t	m_frame_count{};

		int32_t		m_fps_index{};
		float64_t	m_fps_accum{};
		float64_t	m_fps_frames[128]{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_TIME_MANAGER_HPP_