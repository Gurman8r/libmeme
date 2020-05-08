#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

// performance_tracker singleton
#define ML_performance_tracker _ML performance_tracker::get_instance()

// benchmark
#define ML_benchmark(name) \
	auto ML_anon = _ML performance_tracker::benchmark{ name }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API performance_tracker final : singleton<performance_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using frame_type = typename pmr::vector<std::pair<cstring, duration>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD benchmark final
		{
			explicit benchmark(cstring name) noexcept : m_name{ name } {}

			~benchmark() noexcept { push(m_name, m_timer.elapsed()); }

		private: cstring m_name{}; timer m_timer{};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto const & previous() noexcept
		{
			return get_instance().m_prev;
		}

		template <class ... Args
		> static void push(Args && ... args) noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_curr.emplace_back(ML_forward(args)...);
		}

		static void swap() noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_prev.swap(inst.m_curr);
			inst.m_curr.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<performance_tracker>;

		performance_tracker() noexcept = default;

		~performance_tracker() noexcept;

		frame_type m_curr; // current frame data
		frame_type m_prev; // previous frame data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_