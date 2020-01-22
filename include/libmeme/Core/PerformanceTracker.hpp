#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

#ifndef ML_DISABLE_BENCHMARKS
#	define ML_BENCHMARK(name) auto ML_ANONYMOUS(scope_timer) { scope_timer{ name } }
#else
#	define ML_BENCHMARK(name)
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API performance_tracker final
	{
		using buffer_type = typename pmr::vector<std::pair<C_string, duration>>;

		static buffer_type m_curr;

		static buffer_type m_prev;

	public:
		ML_NODISCARD static inline auto const & previous() noexcept
		{
			return m_prev;
		}

		template <class ... Args
		> static inline auto emplace_back(Args && ... args)
		{
			return m_curr.emplace_back(std::forward<Args>(args)...);
		}

		static inline void refresh() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();

			m_curr.reserve(m_prev.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct scope_timer final : non_copyable
	{
		C_string name; timer timer;

		scope_timer(C_string name) noexcept : name{ name }, timer { true }
		{
		}

		~scope_timer() noexcept
		{
			performance_tracker::emplace_back(name, timer.stop().elapsed());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_