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

	pmr::vector<video_mode> const & video_mode::get_fullscreen_modes()
	{
		static pmr::vector<video_mode> temp{};
		auto add_mode = [&](video_mode && vm) noexcept
		{
			if (std::find(temp.begin(), temp.end(), ML_forward(vm)) == temp.end())
			{
				temp.emplace_back(ML_forward(vm));
			}
		};
		static ML_scope // once
		{
#if defined(ML_os_windows)
			DEVMODE dm; dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				add_mode({ { (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight }, dm.dmBitsPerPel });
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