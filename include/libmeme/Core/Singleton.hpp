#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct Singleton : public NonCopyable
	{
		static inline T & getInstance()
		{
			static T instance;
			return instance;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SINGLETON_HPP_