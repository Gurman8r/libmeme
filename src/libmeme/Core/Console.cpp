#include <libmeme/Core/Console.hpp>

#ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#else
#endif

namespace ml::cio
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::ostream & COL::operator()(std::ostream & out) const
	{
#ifdef ML_SYSTEM_WINDOWS
		if (HANDLE handle{ ::GetStdHandle(STD_OUTPUT_HANDLE) })
		{
			::SetConsoleTextAttribute(handle, (*this)());
		}
#else 
		// do the thing
#endif
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}