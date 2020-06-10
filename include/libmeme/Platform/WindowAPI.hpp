#ifndef _ML_WINDOW_API_HPP_
#define _ML_WINDOW_API_HPP_

#include <libmeme/Core/Memory.hpp>
#include <libmeme/Platform/WindowInput.hpp>
#include <libmeme/Platform/WindowSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_declhandle(cursor_handle);
	ML_declhandle(monitor_handle);
	ML_declhandle(window_handle);
	ML_declhandle(window_proc);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias window_char_fn				= typename void(*)(window_handle, uint32_t);
	ML_alias window_char_mods_fn		= typename void(*)(window_handle, uint32_t, int32_t);
	ML_alias window_close_fn			= typename void(*)(window_handle);
	ML_alias window_content_scale_fn	= typename void(*)(window_handle, float_t, float_t);
	ML_alias window_cursor_enter_fn		= typename void(*)(window_handle, int32_t);
	ML_alias window_cursor_position_fn	= typename void(*)(window_handle, float64_t, float64_t);
	ML_alias window_drop_fn				= typename void(*)(window_handle, int32_t, cstring[]);
	ML_alias window_error_fn			= typename void(*)(int32_t, cstring);
	ML_alias window_focus_fn			= typename void(*)(window_handle, int32_t);
	ML_alias window_framebuffer_size_fn	= typename void(*)(window_handle, int32_t, int32_t);
	ML_alias window_iconify_fn			= typename void(*)(window_handle, int32_t);
	ML_alias window_key_fn				= typename void(*)(window_handle, int32_t, int32_t, int32_t, int32_t);
	ML_alias window_maximize_fn			= typename void(*)(window_handle, int32_t);
	ML_alias window_mouse_fn			= typename void(*)(window_handle, int32_t, int32_t, int32_t);
	ML_alias window_position_fn			= typename void(*)(window_handle, int32_t, int32_t);
	ML_alias window_refresh_fn			= typename void(*)(window_handle);
	ML_alias window_scroll_fn			= typename void(*)(window_handle, float64_t, float64_t);
	ML_alias window_size_fn				= typename void(*)(window_handle, int32_t, int32_t);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_attr_ : int32_t
	{
		window_attr_focused,
		window_attr_iconified,
		window_attr_resizable,
		window_attr_visible,
		window_attr_decorated,
		window_attr_auto_iconify,
		window_attr_floating,
		window_attr_maximized,
		window_attr_center_cursor,
		window_attr_transparent_framebuffer,
		window_attr_hovered,
		window_attr_focus_on_show,

		window_attr_red_bits,
		window_attr_green_bits,
		window_attr_blue_bits,
		window_attr_alpha_bits,
		window_attr_depth_bits,
		window_attr_stencil_bits,
		window_attr_accum_red_bits,
		window_attr_accum_green_bits,
		window_attr_accum_blue_bits,
		window_attr_accum_alpha_bits,
		window_attr_aux_buffers,
		window_attr_stereo,
		window_attr_samples,
		window_attr_srgb_capable,
		window_attr_refresh_rate,
		window_attr_doublebuffer,

		window_attr_client_api,
		window_attr_context_version_major,
		window_attr_context_version_minor,
		window_attr_context_revision,
		window_attr_context_robustness,
		window_attr_backend_forward_compat,
		window_attr_backend_debug_context,
		window_attr_backend_profile,
		window_attr_context_release_behavior,
		window_attr_context_no_error,
		window_attr_context_creation_api,
		window_attr_scale_to_monitor,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_client_ : int32_t
	{
		window_client_unknown,
		window_client_opengl,
		window_client_vulkan,
		window_client_directx,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_profile_ : int32_t
	{
		window_profile_any,
		window_profile_core,
		window_profile_compat,
		window_profile_debug,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_hints_ : int32_t
	{
		window_hints_none				= 0,
		window_hints_resizable			= (1 << 0),
		window_hints_visible			= (1 << 1),
		window_hints_decorated			= (1 << 2),
		window_hints_focused			= (1 << 3),
		window_hints_auto_iconify		= (1 << 4),
		window_hints_floating			= (1 << 5),
		window_hints_maximized			= (1 << 6),
		window_hints_doublebuffer		= (1 << 7),

		// resizable / visible / decorated / focused / auto iconify
		window_hints_default
			= window_hints_resizable
			| window_hints_visible
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify,

		// resizable / decorated / focused / auto iconify / maximized
		window_hints_default_maximized
			= window_hints_resizable
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify
			| window_hints_maximized,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_API_HPP_