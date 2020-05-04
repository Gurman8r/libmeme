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
		> scope_binder(T & value, Args && ... args) noexcept : m_value{ &value }
		{
			m_value->bind(ML_forward(args)...);
		}

		~scope_binder() noexcept
		{
			m_value->unbind();
		}

	private:
		T * const m_value;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BINDER_HPP_