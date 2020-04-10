#include <libmeme/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	game_time::game_time(allocator_type const & alloc) noexcept
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

	game_time::~game_time() noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void game_time::begin_loop() noexcept
	{
		m_delta_time = m_loop_timer.stop().elapsed();

		m_loop_timer.start();
	}

	void game_time::end_loop() noexcept
	{
		++m_frame_count;

		m_frame_rate = ([&, dt = m_delta_time.count()]()
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