#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

#ifndef ML_DISABLE_BENCHMARKS
#	define ML_BENCHMARK(name) auto ML_ANONYMOUS(scope_timer) { ScopeTimer{ name } }
#else
#	define ML_BENCHMARK(name)
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API PerformanceTracker final
	{
		ML_NODISCARD static inline auto const & previous() noexcept
		{
			return m_prev;
		}

		static inline void swap() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();

			m_curr.reserve(m_prev.size());
		}

	private:
		friend struct ScopeTimer;

		using buffer_type = typename pmr::vector<std::pair<C_string, Duration>>;

		static buffer_type m_curr;

		static buffer_type m_prev;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ScopeTimer final : public NonCopyable
	{
		C_string name; Timer timer;

		ScopeTimer(C_string name) noexcept : name{ name }, timer { true }
		{
		}

		~ScopeTimer() noexcept
		{
			PerformanceTracker::m_curr.emplace_back(name, timer.stop().elapsed());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_