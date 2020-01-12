#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

#ifndef ML_DISABLE_BENCHMARKS
#	define ML_BENCHMARK(...) ML_ANON_T(ScopeTimer, ##__VA_ARGS__)
#else
#	define ML_BENCHMARK(...) ((void)0)
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API PerformanceTracker final
	{
		friend struct ScopeTimer;
		
		using buffer_type = typename std::vector<std::pair<C_String, Duration>>;

		static buffer_type m_curr;

		static buffer_type m_prev;

	public:
		static inline auto const & previous() noexcept
		{
			return m_prev;
		}

		static inline void swap() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();

			m_curr.reserve(m_prev.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ScopeTimer final : public NonCopyable
	{
		C_String name; Timer timer;

		ScopeTimer(C_String name) noexcept : name{ name }, timer { true }
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