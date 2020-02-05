#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>
#include <libmeme/Core/ScopeGuard.hpp>

//#ifndef ML_DISABLE_BENCHMARKS
//#	define ML_BENCHMARK(name) auto ML_ANONYMOUS(scope_timer) { scope_timer{ name } }
//#else
//#	define ML_BENCHMARK(name)
//#endif

#define ML_BENCHMARK_IMPL(var, name) \
	auto var = timer{ true }; \
	ML_DEFER{ ml::performance_tracker::emplace_back( name, var.stop().elapsed() ); };

#define ML_BENCHMARK(name) ML_BENCHMARK_IMPL(ML_ANONYMOUS(timer), name)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API performance_tracker final
	{
		using buffer_type = typename pmr::vector<std::pair<cstring, duration>>;

		static buffer_type m_curr;

		static buffer_type m_prev;

	public:
		ML_NODISCARD static inline buffer_type const & previous() noexcept
		{
			return m_prev;
		}

		template <class ... Args
		> static inline decltype(auto) emplace_back(Args && ... args)
		{
			return m_curr.emplace_back(ML_FWD(args)...);
		}

		static inline void refresh() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();

			m_curr.reserve(m_prev.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_