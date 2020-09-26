/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/StreamSniper.hpp>
#include <libmeme/Client/PluginManager.hpp>
#include <libmeme/Client/ClientEvents.hpp>
#include <libmeme/Client/ImGui.hpp>
#include <libmeme/Client/GuiEvents.hpp>
#include <libmeme/Graphics/Font.hpp>
#include <libmeme/Graphics/Mesh.hpp>
#include <libmeme/Graphics/Shader.hpp>
#include <libmeme/Graphics/Renderer.hpp>
#include <libmeme/Engine/SceneManager.hpp>
#include <libmeme/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_PLUGIN_API studio final : plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		studio(plugin_manager * manager, void * user) noexcept : plugin{ manager, user }
		{
			subscribe<client_enter_event>();
			subscribe<client_exit_event>();
			subscribe<client_update_event>();

			subscribe<gui_dockspace_event>();
			subscribe<gui_menubar_event>();
			subscribe<gui_render_event>();

			subscribe<window_key_event>();
			subscribe<window_mouse_event>();
			subscribe<window_cursor_position_event>();
		}

		~studio() noexcept override {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_event(event const & value) override
		{
			switch (value)
			{
			case client_enter_event				::ID: return on_client_enter((client_enter_event const &)value);
			case client_exit_event				::ID: return on_client_exit((client_exit_event const &)value);
			case client_update_event			::ID: return on_client_update((client_update_event const &)value);
			
			case gui_dockspace_event			::ID: return on_imgui_dockspace((gui_dockspace_event const &)value);
			case gui_menubar_event				::ID: return on_imgui_menubar((gui_menubar_event const &)value);
			case gui_render_event				::ID: return on_imgui_render((gui_render_event const &)value);
			
			case window_key_event				::ID: return on_window_key((window_key_event const &)value);
			case window_mouse_event				::ID: return on_window_mouse((window_mouse_event const &)value);
			case window_cursor_position_event	::ID: return on_window_cursor_position((window_cursor_position_event const &)value);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_client_enter(client_enter_event const & ev)
		{
		}

		void on_client_exit(client_exit_event const & ev)
		{
		}

		void on_client_update(client_update_event const & ev)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_imgui_dockspace(gui_dockspace_event const & ev)
		{
		}

		void on_imgui_menubar(gui_menubar_event const & ev)
		{
		}

		void on_imgui_render(gui_render_event const & ev)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_window_key(window_key_event const & ev)
		{
		}

		void on_window_mouse(window_mouse_event const & ev)
		{
		}

		void on_window_cursor_position(window_cursor_position_event const & ev)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API ml::plugin * ml_plugin_attach(ml::plugin_manager * manager, void * user)
	{
		return manager->get_memory()->new_object<ml::studio>(manager, user);
	}
	
	ML_PLUGIN_API void ml_plugin_detach(ml::plugin_manager * manager, ml::plugin * ptr)
	{
		manager->get_memory()->delete_object((ml::studio *)ptr);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */