#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>
#include <libmeme/Core/ScopeGuard.hpp>

#define ML_IMPL_BENCHMARK(var, name)							\
	auto var{ ml::timer{ true } };								\
	ML_DEFER{													\
	var.stop();													\
	ml::performance_tracker::push_frame(name, var.elapsed());	\
	};

#define ML_BENCHMARK(name) ML_IMPL_BENCHMARK(ML_ANONYMOUS(timer), name)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API performance_tracker final
	{
		using frame_type = typename pmr::vector<
			std::pair<cstring, duration>
		>;

		static frame_type m_curr;
		static frame_type m_prev;

	public:
		ML_NODISCARD static inline frame_type const & last_frame() noexcept
		{
			return m_prev;
		}

		template <class ... Args
		> static inline decltype(auto) push_frame(Args && ... args)
		{
			return m_curr.emplace_back(ML_FWD(args)...);
		}

		static inline void refresh_frames() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();

			m_curr.reserve(m_prev.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_