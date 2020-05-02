#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct singleton : non_copyable
	{
		using self_type = typename T;

		ML_NODISCARD static self_type & get_instance() noexcept
		{
			static self_type temp{};
			return temp;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SINGLETON_HPP_