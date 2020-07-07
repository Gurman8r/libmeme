#ifndef _ML_NATIVE_HPP_
#define _ML_NATIVE_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_os_windows)
#	include <Windows.h>

#elif defined(ML_os_unix)
#	include <unistd.h>
#	if defined(ML_os_linux)
#		include <dlfcn.h>
#	endif

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_NATIVE_HPP_