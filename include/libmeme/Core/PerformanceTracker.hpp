#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

#define ML_impl_benchmark(var, id)										\
	auto var{ _ML timer{ true } };										\
	ML_defer{ _ML performance_tracker::push(id, var.elapsed()); };

#define ML_benchmark(id) ML_impl_benchmark(ML_anon(benchmark), id)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API performance_tracker final
	{
		using frame = pmr::vector<std::pair<cstring, duration>>;
		
		static frame m_curr;
		static frame m_prev;

	public:
		ML_NODISCARD static frame const & prev() noexcept
		{
			return m_prev;
		}

		template <class ... Args
		> static void push(Args && ... args) noexcept
		{
			m_curr.emplace_back(ML_forward(args)...);
		}

		static void refresh() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_