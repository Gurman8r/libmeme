#ifndef _ML_PERFORMANCE_TRACKER_HPP_
#define _ML_PERFORMANCE_TRACKER_HPP_

#include <libmeme/Core/Timer.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// performance tracker singleton
#define ML_performance_tracker \
	_ML performance_tracker::get_instance()

// benchmark scope
#define ML_bench_scope(id) \
	auto ML_anon = _ML performance_tracker::scope_benchmark{ id }

// benchmark lambda
#define ML_bench_lambda(id) \
	auto ML_anon = _ML performance_tracker::lambda_benchmark{ id } + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// global performance tracker
	struct ML_CORE_API performance_tracker final : singleton<performance_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using frame_data = typename pmr::vector<std::pair<cstring, duration>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// get previous frame
		ML_NODISCARD static frame_data const & prev() noexcept
		{
			static auto & inst{ get_instance() };
			return inst.m_previous;
		}

		// add to current frame
		template <class ... Args
		> static void push(Args && ... args) noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_current.emplace_back(ML_forward(args)...);
		}

		// swap frames and clear current
		static void refresh() noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_previous.swap(inst.m_current);
			inst.m_current.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD scope_benchmark final
		{
			explicit scope_benchmark(cstring id) noexcept : id{ id } {}

			~scope_benchmark() noexcept { push(id, t.elapsed()); }

		private: cstring const id; timer t{};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD lambda_benchmark final
		{
			explicit lambda_benchmark(cstring id) noexcept : id{ id } {}

			template <class Fn> inline auto const & operator+(Fn && fn) const & noexcept
			{
				timer t{};
				std::invoke(ML_forward(fn));
				push(id, t.elapsed());
				return (*this);
			}

		private: cstring const id;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<performance_tracker>;

		performance_tracker() noexcept = default;

		~performance_tracker() noexcept;

		frame_data m_current{}, m_previous{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PERFORMANCE_TRACKER_HPP_