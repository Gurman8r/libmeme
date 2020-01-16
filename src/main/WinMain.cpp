/* * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

#ifdef ML_OS_WINDOWS

#include <Windows.h>

extern INT main();

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main();
}

#endif

/* * * * * * * * * * * * * * * * * * * * */