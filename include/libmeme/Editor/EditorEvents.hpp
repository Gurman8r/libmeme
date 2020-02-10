#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct gui_begin_event final : T_event<gui_begin_event>
	{
	};

	struct gui_draw_event final : T_event<gui_draw_event>
	{
	};

	struct gui_end_event final : T_event<gui_end_event>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct gui_dock_event final : T_event<gui_dock_event>
	{
		struct editor_dockspace & d;

		constexpr gui_dock_event(struct editor_dockspace & d) noexcept
			: d{ d }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_