#ifndef _ML_PERFORMANCE_HPP_
#define _ML_PERFORMANCE_HPP_

#include <libmeme/Core/Timer.hpp>

namespace ml
{
	// performance tracker singleton
	class ML_CORE_API performance final : public singleton<performance>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD sample final
		{
			cstring name; duration time;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// get previous frame
		ML_NODISCARD static auto const & last_frame() noexcept
		{
			return get_singleton().m_previous;
		}

		// add to current frame
		template <class ... Args
		> static void add_sample(Args && ... args) noexcept
		{
			static auto & self{ get_singleton() };
			self.m_current.emplace_back(sample{ ML_forward(args)... });
		}

		// swap frames and clear current
		static void refresh_samples() noexcept
		{
			static auto & self{ get_singleton() };
			self.m_previous.swap(self.m_current);
			self.m_current.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// scope benchmark
		struct ML_NODISCARD scope_benchmark final
		{
			explicit scope_benchmark(cstring id) noexcept : id{ id } {}

			~scope_benchmark() noexcept { add_sample(id, t.elapsed()); }

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
				add_sample(id, t.elapsed());
				return (*this);
			}

		private: cstring const id;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton;

		~performance() noexcept;

		pmr::vector<sample> m_current{}, m_previous{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// scope benchmark
#define ML_benchmark_S(id) \
	auto ML_anon = _ML performance::scope_benchmark{ id }

// lambda benchmark
#define ML_benchmark_L(id, ...) \
	auto ML_anon = _ML performance::lambda_benchmark{ id } + [##__VA_ARGS__]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PERFORMANCE_HPP_