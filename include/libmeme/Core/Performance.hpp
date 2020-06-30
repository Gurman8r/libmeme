#ifndef _ML_PERFORMANCE_HPP_
#define _ML_PERFORMANCE_HPP_

#include <libmeme/Core/Timer.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		ML_NODISCARD static auto const & get_previous() noexcept
		{
			return get_instance().m_previous;
		}

		// add to current frame
		template <class ... Args
		> static void push_sample(Args && ... args) noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_current.emplace_back(sample{ ML_forward(args)... });
		}

		// swap frames and clear current
		static void refresh_samples() noexcept
		{
			static auto & inst{ get_instance() };
			inst.m_previous.swap(inst.m_current);
			inst.m_current.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<performance>;

#ifdef ML_STATIC
		static self_type g_instance;
#endif

		~performance() noexcept;

		pmr::vector<sample> m_current{}, m_previous{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{

	// scope benchmark
	struct ML_NODISCARD scope_benchmark final
	{
		explicit scope_benchmark(cstring id) noexcept : id{ id } {}

		~scope_benchmark() noexcept { performance::push_sample(id, t.elapsed()); }

	private: cstring const id; timer t{};
	};
}

// scope benchmark
#define ML_benchmark_S(id) \
	auto ML_anon = _ML_IMPL scope_benchmark{ id }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
	// lambda benchmark
	struct ML_NODISCARD lambda_benchmark final
	{
		explicit lambda_benchmark(cstring id) noexcept : id{ id } {}

		template <class Fn> auto const & operator+(Fn && fn) const & noexcept
		{
			timer t{};
			std::invoke(ML_forward(fn));
			performance::push_sample(id, t.elapsed());
			return (*this);
		}

	private: cstring const id;
	};

}

// lambda benchmark
#define ML_benchmark_L(id) \
	auto ML_anon = _ML_IMPL lambda_benchmark{ id } + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PERFORMANCE_HPP_