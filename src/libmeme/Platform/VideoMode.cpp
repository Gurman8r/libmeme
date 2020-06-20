#include <libmeme/Platform/VideoMode.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// platform specific
#if defined(ML_os_windows)
#	include <Windows.h>
#elif defined(ML_os_apple)
#elif defined(ML_os_unix)
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	video_mode const & video_mode::get_desktop_mode()
	{
		static video_mode temp{};
		static ML_scope // once
		{
#if defined(ML_os_windows)
			DEVMODE dm; dm.dmSize = sizeof(dm);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
			temp.size = vec2i{ (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight };
			temp.depth = dm.dmBitsPerPel;

#elif defined(ML_os_apple)
#elif defined(ML_os_unix)
#else
#endif
		};
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ds::set<video_mode> const & video_mode::get_fullscreen_modes()
	{
		static ds::set<video_mode> temp{};
		static ML_scope // once
		{
#if defined(ML_os_windows)
			DEVMODE dm; dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				temp.insert({ { (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight }, dm.dmBitsPerPel });
			}

#elif defined(ML_os_apple)
#elif defined(ML_os_unix)
#else
#endif
		};
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}