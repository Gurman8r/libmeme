#include <libmeme/Window/VideoMode.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_os_windows)
#	include <Windows.h>

#else
#	error ""
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	video_mode const & video_mode::get_desktop_mode()
	{
		static video_mode temp{};
		static ML_scope(&) // once
		{
#if defined(ML_os_windows)
			DEVMODE dm; dm.dmSize = sizeof(dm);
			::EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &dm);
			temp =
			{
				{ (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight },
				vec4b::fill((byte_t)dm.dmBitsPerPel),
				(int32_t)dm.dmDisplayFrequency
			};
#else
#	error ""
#endif
		};
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ds::set<video_mode> const & video_mode::get_fullscreen_modes()
	{
		static ds::set<video_mode> temp{};
		static ML_scope(&) // once
		{
#if defined(ML_os_windows)
			DEVMODE dm; dm.dmSize = sizeof(dm);
			for (int32_t i = 0; ::EnumDisplaySettingsW(nullptr, i, &dm); ++i)
			{
				temp.insert
				({
					{ (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight },
					vec4b::fill((byte_t)dm.dmBitsPerPel),
					(int32_t)dm.dmDisplayFrequency
				});
			}
#else
#	error ""
#endif
		};
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}