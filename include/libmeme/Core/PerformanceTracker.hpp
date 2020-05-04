#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

// benchmark
#define ML_benchmark(name) \
	auto ML_anon = _ML performance_tracker{ name }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API performance_tracker final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		using frame_data = pmr::vector<std::pair<cstring, duration>>;
		
		static frame_data m_curr; // current global data
		static frame_data m_prev; // previous global data

		cstring	m_name	; // local name
		timer	m_timer	; // local timer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		explicit performance_tracker(cstring name) noexcept : m_name{ name }, m_timer{ true }
		{
		}

		~performance_tracker() noexcept
		{
			add_trace(m_name, m_timer.elapsed());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		ML_NODISCARD static frame_data const & previous() noexcept
		{
			return m_prev;
		}

		template <class ... Args
		> static void add_trace(Args && ... args) noexcept
		{
			m_curr.emplace_back(ML_forward(args)...);
		}

		static void swap_frames() noexcept
		{
			m_prev.swap(m_curr);

			m_curr.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_