#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

namespace ml
{
	struct Engine::PluginLoader final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		PluginLoader() noexcept {}

		~PluginLoader()
		{
			m_libs.for_each([](auto const &, auto & p) { delete p; });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool load_plugin(path_t const & path)
		{
			if (auto const it{ m_files.insert(path) }; it.second)
			{
				if (auto lib{ make_shared_library(*it.first) })
				{
					if (auto const plug{ lib.call_function<Plugin *>("ml_plugin_main") })
					{
						return (*m_libs.insert(std::move(lib), plug).first.second);
					}
				}
				m_files.erase(it.first);
			}
			return false;
		}

		inline void clear_plugins()
		{
			m_files.clear();
			m_libs.for_each([](auto const &, auto & p) { delete p; });
			m_libs.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::flat_set<path_t> m_files{};
		ds::flat_map<SharedLibrary, Plugin *> m_libs{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(Engine::s_window)	Engine::s_window	{};
	decltype(Engine::s_time)	Engine::s_time		{};
	decltype(Engine::s_plugins) Engine::s_plugins	{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Engine::startup(EngineSettings const & es)
	{
		if (!Lua::startup())
			return debug::log_error("Failed initializing Lua");

		if (!Python::startup(es.program_name, es.library_path))
			return debug::log_error("Failed initializing Python");
		
		return true;
	}

	bool Engine::init_window(WindowSettings const & ws)
	{
		if (!s_window.create(ws.title, ws.display, ws.context, ws.flags))
			return false;

		if (ws.install_callbacks)
		{
			s_window.set_char_callback([](auto, auto ch)
			{
				EventSystem::fire_event<CharEvent>(ch);
			});
			s_window.set_cursor_enter_callback([](auto, auto entered)
			{
				EventSystem::fire_event<CursorEnterEvent>(entered);
			});
			s_window.set_cursor_pos_callback([](auto, auto x, auto y)
			{
				EventSystem::fire_event<CursorPosEvent>(x, y);
			});
			s_window.set_error_callback([](auto code, auto desc)
			{
				EventSystem::fire_event<WindowErrorEvent>(code, desc);
			});
			s_window.set_frame_size_callback([](auto, auto w, auto h)
			{
				EventSystem::fire_event<FrameSizeEvent>(w, h);
			});
			s_window.set_key_callback([](auto, auto button, auto scan, auto action, auto mods)
			{
				EventSystem::fire_event<KeyEvent>(button, scan, action, mask8_t{ {
					(mods & ML_MOD_SHIFT),
					(mods & ML_MOD_CTRL),
					(mods & ML_MOD_ALT),
					(mods & ML_MOD_SUPER),
					(mods & ML_MOD_CAPSLOCK),
					(mods & ML_MOD_NUMLOCK)
				} });
			});
			s_window.set_mouse_callback([](auto, auto button, auto action, auto mods)
			{
				EventSystem::fire_event<MouseEvent>(button, action, mods);
			});
			s_window.set_scroll_callback([](auto, auto x, auto y)
			{
				EventSystem::fire_event<ScrollEvent>(x, y);
			});
			s_window.set_window_close_callback([](auto)
			{
				EventSystem::fire_event<WindowCloseEvent>();
			});
			s_window.set_window_focus_callback([](auto, auto focused)
			{
				EventSystem::fire_event<WindowFocusEvent>(focused);
			});
			s_window.set_window_pos_callback([](auto, auto x, auto y)
			{
				EventSystem::fire_event<WindowPosEvent>(x, y);
			});
			s_window.set_window_size_callback([](auto, auto w, auto h)
			{
				EventSystem::fire_event<WindowSizeEvent>(w, h);
			});
		}

		return true;
	}

	bool Engine::running()
	{
		return s_window.is_open();
	}

	void Engine::shutdown()
	{
		s_plugins.clear_plugins();

		s_window.destroy();
		
		Window::terminate();

		Python::shutdown();
		
		Lua::shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::begin_loop()
	{
		s_time.total = s_time.main.elapsed().count();
		
		s_time.delta = s_time.loop.elapsed().count();
		
		s_time.loop.stop().start();

		Window::poll_events();
	}

	void Engine::begin_draw()
	{
		GL::clearColor(0, 0, 0, 1);

		GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
		
		GL::viewport(0, 0, s_window.get_frame_width(), s_window.get_frame_height());
		
		constexpr RenderStates states{ // default states
			{}, {}, {}, {}
		}; states();
	}

	void Engine::end_loop()
	{
		if (s_window.get_flags() & WindowFlags_DoubleBuffered)
		{
			s_window.swap_buffers();
		}
		GL::flush();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Engine::load_plugin(path_t const & path)
	{
		return s_plugins.load_plugin(path);
	}

	int32_t Engine::load_plugin(path_t && path)
	{
		return s_plugins.load_plugin(std::move(path)); // fixme
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}