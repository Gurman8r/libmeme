#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

#define ML_impl_benchmark(var, id)									\
	auto var{ _ML timer{ true } };									\
	ML_defer{ _ML performance_tracker::add_trace(id, var.elapsed()); };

#define ML_benchmark(id) ML_impl_benchmark(ML_anon(benchmark), id)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API performance_tracker final
	{
		using frame_data = pmr::vector<std::pair<cstring, duration>>;
		
		static frame_data m_curr; // current frame_data
		static frame_data m_prev; // previous frame_data

	public:
		ML_NODISCARD static frame_data const & previous() noexcept
		{
			return m_prev;
		}

		template <class ... Args
		> static void add_trace(Args && ... args) noexcept
		{
			m_curr.emplace_back(ML_forward(args)...);
		}

		static void swap_frames() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_