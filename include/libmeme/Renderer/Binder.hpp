#ifndef _ML_BINDER_HPP_
#define _ML_BINDER_HPP_

#include <libmeme/Common.hpp>

// bind scope
#define ML_bind_scope(v, ...) \
	auto ML_anon = _ML impl::scope_binder{ v, ##__VA_ARGS__ }

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ML_NODISCARD scope_binder final
	{
		template <class ... Args
		> scope_binder(T * value, Args && ... args) noexcept
			: m_ptr{ value }
		{
			m_ptr->bind(ML_forward(args)...);
		}

		template <class ... Args
		> scope_binder(T & value, Args && ... args) noexcept
			: scope_binder<T>{ &value, ML_forward(args)... }
		{
		}

		~scope_binder() noexcept
		{
			m_ptr->unbind();
		}

	private: T * const m_ptr;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BINDER_HPP_