#include <libmeme/Engine/TimeManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	time_manager::time_manager(json const & j, allocator_type const & alloc) noexcept
		: m_main_timer	{ true }
		, m_loop_timer	{}
		, m_fps_accum	{}
		, m_fps_index	{}
		, m_fps_value	{}
		, m_fps_frames	{}
		, m_delta_time	{}
		, m_frame_rate	{}
		, m_frame_count	{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void time_manager::begin_loop() noexcept
	{
		m_delta_time = m_loop_timer.stop().elapsed();

		m_loop_timer.start();
	}

	void time_manager::end_loop() noexcept
	{
		++m_frame_count;

		m_frame_rate = ([&, dt = m_delta_time.count()]() noexcept
		{
			m_fps_accum += dt - m_fps_frames[m_fps_index];
			
			m_fps_frames[m_fps_index] = dt;
			
			m_fps_index = (m_fps_index + 1) % ML_arraysize(m_fps_frames);
			
			return m_fps_value = (m_fps_accum > 0.0)
				? 1.0 / (m_fps_accum / (float64_t)ML_arraysize(m_fps_frames))
				: FLT_MAX;
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}