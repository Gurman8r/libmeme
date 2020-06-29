#ifndef _ML_TIME_MANAGER_HPP_
#define _ML_TIME_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Array.hpp>
#include <libmeme/Core/Performance.hpp>

namespace ml
{
	struct ML_ENGINE_API time_manager final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit time_manager(json const & j, allocator_type alloc) noexcept;

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

			performance::refresh_samples();

			m_delta_time = m_loop_timer.elapsed();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto total_time() const noexcept -> duration { return m_main_timer.elapsed(); }

		auto delta_time() const noexcept -> duration { return m_delta_time; }

		auto frame_rate() const noexcept -> float_t { return m_frame_rate; }

		auto frame_count() const noexcept -> uint64_t { return m_frame_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		timer		m_main_timer	{}			; // main timer
		timer		m_loop_timer	{ false }	; // loop timer
		duration	m_delta_time	{}			; // delta time
		float_t		m_frame_rate	{}			; // frame rate
		uint64_t	m_frame_count	{}			; // frame count

		// fps tracker
		struct ML_NODISCARD fps_tracker final : trackable, non_copyable
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