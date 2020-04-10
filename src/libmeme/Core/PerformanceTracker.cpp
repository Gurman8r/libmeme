#include <libmeme/Core/PerformanceTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(performance_tracker::m_curr) performance_tracker::m_curr{};
	decltype(performance_tracker::m_prev) performance_tracker::m_prev{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}