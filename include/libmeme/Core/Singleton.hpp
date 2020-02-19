#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T
	> struct singleton : non_copyable
	{
		ML_NODISCARD static inline T & ref() noexcept
		{
			static T instance{};
			return instance;
		}

		ML_NODISCARD static inline T const & cref() noexcept
		{
			return static_cast<T const &>(ref());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SINGLETON_HPP_