#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

// benchmark
#define ML_benchmark(name) \
	auto ML_anon = _ML performance_tracker{ name }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API performance_tracker final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using frame_data = typename pmr::vector<std::pair<cstring, duration>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit performance_tracker(cstring name) noexcept : m_name{ name } {}

		~performance_tracker() noexcept { push(m_name, m_timer.elapsed()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static frame_data const & previous() noexcept
		{
			return m_prev;
		}

		template <class ... Args
		> static void push(Args && ... args) noexcept
		{
			m_curr.emplace_back(ML_forward(args)...);
		}

		static void swap() noexcept
		{
			m_prev.swap(m_curr);
			m_curr.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static frame_data m_curr; // current frame
		static frame_data m_prev; // previous frame

		cstring	m_name	{};
		timer	m_timer	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_