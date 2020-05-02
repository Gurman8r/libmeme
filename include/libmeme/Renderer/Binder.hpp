#ifndef _ML_BINDER_HPP_
#define _ML_BINDER_HPP_

#include <libmeme/Common.hpp>

#define ML_bind_ref(v, ...) \
	auto ML_anon(ref_binder) = _ML ref_binder{ v, ##__VA_ARGS__ }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ref_binder final
	{
		template <class ... Args
		> ref_binder(T & value, Args && ... args) noexcept : m_ref{ value }
		{
			m_ref.get().bind(ML_forward(args)...);
		}

		~ref_binder() noexcept
		{
			m_ref.get().unbind();
		}

	private:
		std::reference_wrapper<T> m_ref;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BINDER_HPP_