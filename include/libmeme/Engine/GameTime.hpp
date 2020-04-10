#ifndef _ML_GAME_TIME_HPP_
#define _ML_GAME_TIME_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>

namespace ml
{
	struct ML_ENGINE_API game_time final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		game_time(allocator_type const & alloc = {}) noexcept;

		~game_time() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void begin_loop() noexcept;

		void end_loop() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto total() const & noexcept -> duration const & { return m_main_timer.elapsed(); }

		auto delta() const & noexcept -> duration const & { return m_delta_time; }

		auto frame_rate() const & noexcept -> float64_t const & { return m_frame_rate; }

		auto frame_count() const & noexcept -> uint64_t const & { return m_frame_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		timer		m_main_timer;
		timer		m_loop_timer;

		float64_t	m_fps_accum;
		int32_t		m_fps_index;
		float64_t	m_fps_value;
		float64_t	m_fps_frames[128];
		
		duration	m_delta_time;
		float64_t	m_frame_rate;
		uint64_t	m_frame_count;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GAME_TIME_HPP_