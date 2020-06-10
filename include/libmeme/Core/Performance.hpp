#ifndef _ML_PERFORMANCE_HPP_
#define _ML_PERFORMANCE_HPP_

#include <libmeme/Core/Timer.hpp>

// scope benchmark
#define ML_benchmark_S(id) \
	auto ML_anon = _ML performance::scope_benchmark{ id }

// lambda benchmark
#define ML_benchmark_L(id) \
	auto ML_anon = _ML performance::lambda_benchmark{ id } + [&]() noexcept

namespace ml
{
	// performance tracker singleton
	class ML_CORE_API performance final : public singleton<performance>
	{
	public:
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

		// scope benchmark
		struct ML_NODISCARD scope_benchmark final
		{
			explicit scope_benchmark(cstring id) noexcept : id{ id } {}

			~scope_benchmark() noexcept { push(id, t.elapsed()); }

		private: cstring const id; timer t{};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// lambda benchmark
		struct ML_NODISCARD lambda_benchmark final
		{
			explicit lambda_benchmark(cstring id) noexcept : id{ id } {}

			template <class Fn> auto const & operator+(Fn && fn) const & noexcept
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
		friend singleton<performance>;

		performance() noexcept = default;

		~performance() noexcept;

		frame_data m_current{}, m_previous{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PERFORMANCE_HPP_