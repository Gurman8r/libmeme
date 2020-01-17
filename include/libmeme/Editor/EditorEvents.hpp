#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class EditorEventType
	{
		MIN_EDITOR_EVENT = Event::EV_EDITOR,

		EV_BeginGui,
		EV_Gui,
		EV_EndGui,

		EV_Dockspace,
		EV_MainMenuBar,

		MAX_EDITOR_EVENT
	};

	static_assert(
		(int32_t)EditorEventType::MAX_EDITOR_EVENT < 
		(int32_t)EditorEventType::MIN_EDITOR_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <EditorEventType ID> struct EditorEvent : public T_Event<EditorEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginGuiEvent final : public EditorEvent<EditorEventType::EV_BeginGui>
	{
		constexpr BeginGuiEvent() {}
	};

	struct GuiEvent final : public EditorEvent<EditorEventType::EV_Gui>
	{
		float64_t const total_time;
		float64_t const delta_time;

		constexpr GuiEvent(float64_t total_time, float64_t delta_time)
			: total_time{ total_time }
			, delta_time{ delta_time }
		{
		}
	};

	struct EndGuiEvent final : public EditorEvent<EditorEventType::EV_EndGui>
	{
		constexpr EndGuiEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct DockspaceEvent final : public EditorEvent<EditorEventType::EV_Dockspace>
	{
		struct Dockspace & dockspace;

		constexpr DockspaceEvent(struct Dockspace & dockspace)
			: dockspace{ dockspace }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_