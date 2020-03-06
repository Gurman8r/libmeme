#ifndef _ML_FPS_TRACKER_HPP_
#define _ML_FPS_TRACKER_HPP_

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	template <size_t N = 120
	> struct frame_tracker final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		float_t accum		{}; // 
		int32_t index		{}; // 
		float_t value		{}; // 
		float_t frames[N]	{}; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline float_t update(float_t const dt) noexcept
		{
			accum += dt - frames[index];
			frames[index] = dt;
			index = (index + 1) % N;
			return (value = (accum > 0.f) ? (1.f / (accum / (float_t)N)) : FLT_MAX);
		}

		inline float_t operator()(float_t const dt) noexcept
		{
			return this->update(dt);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_FPS_TRACKER_HPP_