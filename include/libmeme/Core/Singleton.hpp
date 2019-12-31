#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Core/Core.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct Singleton : public NonCopyable
	{
		static inline T & getInstance() noexcept
		{
			static T instance;
			return instance;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SINGLETON_HPP_