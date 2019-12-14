#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/Timer.hpp>

#define ML_PerformanceTracker ::ml::PerformanceTracker::getInstance()

#ifndef ML_DISABLE_BENCHMARKS
#	define ML_BENCHMARK(...) ML_ANON_T(ScopeTimer, ##__VA_ARGS__)
#else
#	define ML_BENCHMARK(...) ((void)0)
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API PerformanceTracker final : public Singleton<PerformanceTracker>
	{
		friend Singleton<PerformanceTracker>;

		std::vector<std::pair<C_String, Duration>> m_curr, m_prev;
		
		PerformanceTracker();
		
		~PerformanceTracker();

	public:
		inline void end_frame()
		{
			m_prev.assign(m_curr.begin(), m_curr.end());

			m_curr.clear();
		}

		inline void push_trace(C_String name, Duration const & duration)
		{
			m_curr.push_back(std::make_pair(name, duration));
		}

		inline auto const & previous() const
		{
			return m_prev;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ScopeTimer final
	{
		C_String name; Timer timer;

		ScopeTimer(C_String name) : name{ name }, timer { true }
		{
		}

		~ScopeTimer()
		{
			ML_PerformanceTracker.push_trace(name, timer.stop().elapsed());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_