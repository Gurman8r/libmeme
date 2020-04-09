#ifndef _ML_WINDOW_SETTINGS_HPP_
#define _ML_WINDOW_SETTINGS_HPP_

#include <libmeme/Platform/ContextSettings.hpp>
#include <libmeme/Platform/DisplaySettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_hints_ : int32_t
	{
		window_hints_none				= (0 << 0), // none
		window_hints_resizable			= (1 << 0), // resizable
		window_hints_visible			= (1 << 1), // visible
		window_hints_decorated			= (1 << 2), // decorated
		window_hints_focused			= (1 << 3), // focused
		window_hints_auto_iconify		= (1 << 4), // auto_iconify
		window_hints_floating			= (1 << 5), // floating
		window_hints_maximized			= (1 << 6), // maximized
		window_hints_fullscreen			= (1 << 7), // fullscreen
		window_hints_double_buffered	= (1 << 8), // double buffered

		// resizable / visible / decorated / focused / auto iconify
		window_hints_default
			= window_hints_resizable
			| window_hints_visible
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify,

		// resizable / decorated / focused / auto iconify / maximized
		window_hints_default_max 
			= window_hints_resizable
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify
			| window_hints_maximized,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD window_settings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::string			title	{}; // 
		display_settings	display	{}; // 
		context_settings	context	{}; // 
		int32_t				hints	{}; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void from_json(json const & j, window_settings & value)
	{
		j.at("title").get_to(value.title);
		j.at("display").get_to(value.display);
		j.at("context").get_to(value.context);
		j.at("hints").get_to(value.hints);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_SETTINGS_HPP_