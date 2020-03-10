#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T
	> struct singleton : non_copyable
	{
		ML_NODISCARD static inline T & get_instance() noexcept
		{
			static T inst{};
			return inst;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SINGLETON_HPP_