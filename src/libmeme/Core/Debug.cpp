#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/Array.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	include <shellapi.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::ostream & COL::operator()(std::ostream & out) const
	{
#ifdef ML_SYSTEM_WINDOWS
		if (HANDLE handle { ::GetStdHandle(STD_OUTPUT_HANDLE) })
		{
			::SetConsoleTextAttribute(handle, (*(*this)));
		}
#else 
		// do the thing
#endif
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Debug::exit(int32_t exitCode)
	{
		return std::exit(exitCode);
	}

	int32_t Debug::clear()
	{
#if (ML_DEBUG)
#	ifdef ML_SYSTEM_WINDOWS
		return std::system("cls");
#	else
		return std::system("clear");
#	endif
#else
		return 0;
#endif
	}

	int32_t Debug::pause(int32_t exitCode)
	{
#if (ML_DEBUG)
#	ifdef ML_SYSTEM_WINDOWS
		std::system("pause");
#	else
		std::cin.get();
#	endif
#endif
		return exitCode;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Debug::execute(std::string const & command, std::string const & file, std::string const & args, std::string const & path, int32_t flags)
	{
#ifdef ML_SYSTEM_WINDOWS
		return ::ShellExecuteA(
			::GetDesktopWindow(),
			command.c_str(),
			file.c_str(),
			args.c_str(),
			path.c_str(),
			flags
		);
#else
		// do the thing
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}