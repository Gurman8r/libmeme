#include <libmeme/Core/PerformanceTracker.hpp>

namespace ml
{
	PerformanceTracker::PerformanceTracker()
		: m_curr{}
		, m_prev{}
	{
	}

	PerformanceTracker::~PerformanceTracker()
	{
	}
}