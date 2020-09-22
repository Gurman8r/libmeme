#ifndef _ML_GUI_EVENTS_HPP_
#define _ML_GUI_EVENTS_HPP_

#include <libmeme/Core/Events.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct gui_manager;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// DOCKSPACE
	ML_decl_event(gui_dockspace_event)
	{
		gui_manager * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr gui_dockspace_event(gui_manager * const ptr) noexcept : ptr{ ptr } {}
	};

	// MAIN MENU BAR
	ML_decl_event(gui_main_menu_bar_event)
	{
		gui_manager * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr gui_main_menu_bar_event(gui_manager * const ptr) noexcept : ptr{ ptr } {}
	};

	// GUI
	ML_decl_event(gui_event)
	{
		gui_manager * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr gui_event(gui_manager * const ptr) noexcept : ptr{ ptr } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GUI_EVENTS_HPP_