#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct begin_gui_event final : I_event<begin_gui_event>
	{
		constexpr begin_gui_event() noexcept = default;
	};

	struct gui_event final : I_event<gui_event>
	{
		constexpr gui_event() noexcept = default;
	};

	struct end_gui_event final : I_event<end_gui_event>
	{
		constexpr end_gui_event() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct dockspace_event final : I_event<dockspace_event>
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