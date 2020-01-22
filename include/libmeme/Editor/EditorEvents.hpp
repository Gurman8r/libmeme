#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct begin_gui_event final : type_event<begin_gui_event>
	{
	};

	struct gui_event final : type_event<gui_event>
	{
	};

	struct end_gui_event final : type_event<end_gui_event>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct dockspace_event final : type_event<dockspace_event>
	{
		struct editor_dockspace & d;

		constexpr dockspace_event(struct editor_dockspace & d) noexcept
			: d{ d }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_