#ifndef _ML_CLIENT_EVENTS_HPP_
#define _ML_CLIENT_EVENTS_HPP_

#include <libmeme/Core/Events.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct application;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LOAD
	ML_decl_event(app_load_event)
	{
		application * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr app_load_event(application * ptr) noexcept : ptr{ ptr } {}
	};

	// UNLOAD
	ML_decl_event(app_unload_event)
	{
		application * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr app_unload_event(application * ptr) noexcept : ptr{ ptr } {}
	};

	// UPDATE
	ML_decl_event(app_update_event)
	{
		application * const ptr;
		auto operator->() const noexcept { return ptr; }
		auto & operator*() const noexcept { return *ptr; }
		constexpr app_update_event(application * ptr) noexcept : ptr{ ptr } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CLIENT_EVENTS_HPP_