#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Events.hpp>

namespace ml
{
	struct system_context;
	struct application;
	struct editor_context;
	struct editor_dockspace;
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LOAD
	ML_decl_event(load_event)
	{
		system_context * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr load_event(system_context * ptr) noexcept : ptr{ ptr } {}
	};

	// UNLOAD
	ML_decl_event(unload_event)
	{
		system_context * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr unload_event(system_context * ptr) noexcept : ptr{ ptr } {}
	};

	// UPDATE
	ML_decl_event(update_event)
	{
		system_context * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr update_event(system_context * ptr) noexcept : ptr{ ptr } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// GUI
	ML_decl_event(gui_event)
	{
		editor_context * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr gui_event(editor_context * const ptr) noexcept : ptr{ ptr } {}
	};

	// DOCKSPACE
	ML_decl_event(dockspace_event)
	{
		editor_dockspace * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr dockspace_event(editor_dockspace * const ptr) noexcept : ptr{ ptr } {}
	};

	// MAIN MENU BAR
	ML_decl_event(main_menu_bar_event)
	{
		editor_dockspace * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr main_menu_bar_event(editor_dockspace * const ptr) noexcept : ptr{ ptr } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_