#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	template <class Derived, bool HasGetter = false
	> struct singleton : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename Derived;

		static constexpr bool has_getter
		{
			HasGetter
		};

		using base_type = typename singleton
		<
			self_type, has_getter
		>;

		ML_NODISCARD static self_type & get_instance() noexcept
		{
			if constexpr (!has_getter)
			{
				static self_type inst{}; return inst;
			}
			else
			{
				return *self_type::do_get_instance();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SINGLETON_HPP_