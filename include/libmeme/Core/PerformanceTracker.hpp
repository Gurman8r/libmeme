#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

// performance_tracker singleton
#define ML_performance_tracker \
	_ML performance_tracker::get_instance()

// benchmark
#define ML_benchmark(id) \
	auto ML_anon = _ML performance_tracker::benchmark_helper{ id } + [&]() noexcept

namespace ml
{
	// global performance tracker
	struct ML_CORE_API performance_tracker final : singleton<performance_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using frame_data = typename pmr::vector<std::pair<cstring, duration>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn> struct ML_NODISCARD benchmark final
		{
			explicit benchmark(cstring id, Fn && fn) noexcept
			{
				std::invoke(ML_forward(fn));

				push(id, t.elapsed());
			}
			
		private: timer t{};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD benchmark_helper final
		{
			cstring const id;

			template <class Fn> inline auto operator+(Fn && fn) const noexcept
			{
				return benchmark<Fn>{ id, ML_forward(fn) };
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static frame_data const & prev() noexcept
		{
			static auto & inst{ get_instance() };
			return inst.m_previous;
		}

		template <class ... Args
		> static void push(Args && ... args) noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_current.emplace_back(ML_forward(args)...);
		}

		static void refresh() noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_previous.swap(inst.m_current);
			inst.m_current.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<performance_tracker>;

		performance_tracker() noexcept = default;

		~performance_tracker() noexcept;

		frame_data m_current, m_previous;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_