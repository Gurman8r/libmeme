#include <libmeme/Core/PerformanceTracker.hpp>

namespace ml
{
	decltype(PerformanceTracker::m_curr) PerformanceTracker::m_curr{};
	decltype(PerformanceTracker::m_prev) PerformanceTracker::m_prev{};
}