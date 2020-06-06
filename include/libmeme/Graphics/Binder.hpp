#ifndef _ML_BINDER_HPP_
#define _ML_BINDER_HPP_

#include <libmeme/Common.hpp>

// bind scope
#define ML_bind_scope(v, ...) \
	auto ML_anon = _ML impl::scope_binder{ v, ##__VA_ARGS__ }

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Bindable> struct ML_NODISCARD scope_binder final
	{
		template <class ... Args
		> scope_binder(Bindable * p, Args && ... args) noexcept : m_ptr{ p }
		{
			ML_assert(m_ptr);
			m_ptr->bind(ML_forward(args)...);
		}

		template <class ... Args
		> scope_binder(Bindable & r, Args && ... args) noexcept : m_ptr{ std::addressof(r) }
		{
			m_ptr->bind(ML_forward(args)...);
		}

		~scope_binder() noexcept
		{
			m_ptr->unbind();
		}

	private:
		Bindable * const m_ptr;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BINDER_HPP_