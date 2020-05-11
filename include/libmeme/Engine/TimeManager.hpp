#ifndef _ML_TIME_MANAGER_HPP_
#define _ML_TIME_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>
#include <libmeme/Core/Array.hpp>

namespace ml
{
	struct ML_ENGINE_API time_manager final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit time_manager(json const & j, allocator_type alloc = {}) noexcept;

		~time_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void begin_loop() noexcept
		{
			m_loop_timer.restart();

			m_frame_rate = m_fps.update(m_delta_time.count<float_t>());
		}

		void end_loop() noexcept
		{
			++m_frame_count;

			m_delta_time = m_loop_timer.elapsed();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto total() const & noexcept -> duration const & { return m_main_timer.elapsed(); }

		auto delta() const & noexcept -> duration const & { return m_delta_time; }

		auto frame_rate() const & noexcept -> float_t const & { return m_frame_rate; }

		auto frame_count() const & noexcept -> uint64_t const & { return m_frame_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		timer		m_main_timer{};
		timer		m_loop_timer{ false };
		
		duration	m_delta_time{};
		float_t		m_frame_rate{};
		uint64_t	m_frame_count{};

		struct fps_tracker final : non_copyable
		{
			using frame_times = ds::array<float_t, 120>;

			float_t		accum{}; // accumulator
			size_t		index{}; // frame index
			frame_times times{}; // frame times

			float_t update(float_t const dt) noexcept
			{
				accum += dt - times[index];
				times[index] = dt;
				index = (index + 1) % times.size();
				return (0.f < accum) ? 1.f / (accum / (float_t)times.size()) : FLT_MAX;
			}

		} m_fps{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_TIME_MANAGER_HPP_