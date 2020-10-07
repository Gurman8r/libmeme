/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Blackboard.hpp>
#include <libmeme/Core/StreamSniper.hpp>
#include <libmeme/Client/PluginManager.hpp>
#include <libmeme/Client/ClientEvents.hpp>
#include <libmeme/Client/ImGuiContext.hpp>
#include <libmeme/Client/ImGuiEvents.hpp>
#include <libmeme/Client/ImGui.hpp>
#include <libmeme/Embed/Python.hpp>
#include <libmeme/Graphics/Font.hpp>
#include <libmeme/Graphics/Mesh.hpp>
#include <libmeme/Graphics/Shader.hpp>
#include <libmeme/Graphics/Renderer.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Scene/SceneManager.hpp>
#include <libmeme/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_PLUGIN_API studio final : plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		blackboard::handle<	ds::map<pmr::string, shared<font>>			> m_fonts		; // 
		blackboard::handle<	ds::map<pmr::string, shared<bitmap>>		> m_images		; // 
		blackboard::handle<	ds::map<pmr::string, shared<mesh>>			> m_meshes		; // 
		blackboard::handle<	ds::map<pmr::string, shared<gfx::shader>>	> m_shaders		; // 
		blackboard::handle<	ds::map<pmr::string, shared<gfx::texture>>	> m_textures	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		studio(plugin_manager * manager, void * user) noexcept : plugin{ manager, user }
			, m_fonts	{ get_vars(), "fonts" }
			, m_images	{ get_vars(), "images" }
			, m_meshes	{ get_vars(), "meshes" }
			, m_shaders	{ get_vars(), "shaders" }
			, m_textures{ get_vars(), "textures" }
		{
			subscribe<client_enter_event>();
			subscribe<client_exit_event>();
			subscribe<client_update_event>();

			subscribe<imgui_dockspace_event>();
			subscribe<imgui_menubar_event>();
			subscribe<imgui_render_event>();

			subscribe<window_key_event>();
			subscribe<window_mouse_event>();
			subscribe<window_cursor_position_event>();
		}

		~studio() noexcept override {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_event(event && value) override
		{
			switch (value)
			{
			case client_enter_event				::ID: return on_client_enter((client_enter_event &&)value);
			case client_exit_event				::ID: return on_client_exit((client_exit_event &&)value);
			case client_update_event			::ID: return on_client_update((client_update_event &&)value);
			
			case imgui_dockspace_event			::ID: return on_imgui_dockspace((imgui_dockspace_event &&)value);
			case imgui_menubar_event			::ID: return on_imgui_menubar((imgui_menubar_event &&)value);
			case imgui_render_event				::ID: return on_imgui_render((imgui_render_event &&)value);
			
			case window_key_event				::ID: return on_window_key((window_key_event &&)value);
			case window_mouse_event				::ID: return on_window_mouse((window_mouse_event &&)value);
			case window_cursor_position_event	::ID: return on_window_cursor_position((window_cursor_position_event &&)value);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_client_enter(client_enter_event && ev)
		{
			// set icon
			if (auto & i = m_images["icon"] = get_memory()->make_ref<bitmap>
			(
				get_io()->path2("assets/textures/icon.png")
			))
			{
				get_window()->set_icon(i->width(), i->height(), i->data());
			}
		}

		void on_client_exit(client_exit_event && ev)
		{
		}

		void on_client_update(client_update_event && ev)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_imgui_dockspace(imgui_dockspace_event && ev)
		{
		}

		void on_imgui_menubar(imgui_menubar_event && ev)
		{
		}

		void on_imgui_render(imgui_render_event && ev)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_window_key(window_key_event && ev)
		{
			get_io()->keyboard[ev.key] = ev.action;
		}

		void on_window_mouse(window_mouse_event && ev)
		{
			get_io()->mouse[ev.button] = ev.action;
		}

		void on_window_cursor_position(window_cursor_position_event && ev)
		{
			get_io()->cursor = { ev.x, ev.y };
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