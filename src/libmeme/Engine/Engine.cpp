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
	struct engine::engine_plugins final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		engine_plugins() noexcept {}

		~engine_plugins()
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
					if (auto const plug{ lib.call_function<plugin *>("ml_plugin_main") })
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
		ds::flat_map<shared_library, plugin *> m_libs{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(engine::s_window)	engine::s_window	{};
	decltype(engine::s_time)	engine::s_time		{};
	decltype(engine::s_plugins) engine::s_plugins	{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup(engine_startup_settings const & s)
	{
		s_time.m_main.start();

		if (!Lua::startup())
			return debug::log_error("Failed initializing Lua");

		if (!Python::startup(s.program_name, s.library_path))
			return debug::log_error("Failed initializing Python");
		
		return true;
	}

	bool engine::init_window(window_startup_settings const & s)
	{
		if (!s_window.create(s.title, s.display, s.context, s.flags))
			return false;

		if (s.install_callbacks)
		{
			s_window.set_char_callback([](auto, auto ch)
			{
				event_system::fire_event<char_event>(ch);
			});

			s_window.set_cursor_enter_callback([](auto, auto entered)
			{
				event_system::fire_event<cursor_enter_event>(entered);
			});

			s_window.set_cursor_pos_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<cursor_pos_event>(x, y);
			});

			s_window.set_error_callback([](auto code, auto desc)
			{
				event_system::fire_event<window_error_event>(code, desc);
			});

			s_window.set_frame_size_callback([](auto, auto w, auto h)
			{
				event_system::fire_event<frame_size_event>(w, h);
			});

			s_window.set_key_callback([](auto, auto button, auto scan, auto action, auto mods)
			{
				event_system::fire_event<key_event>(button, scan, action, mods);
			});

			s_window.set_mouse_callback([](auto, auto button, auto action, auto mods)
			{
				event_system::fire_event<mouse_event>(button, action, mods);
			});

			s_window.set_scroll_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<scroll_event>(x, y);
			});

			s_window.set_window_close_callback([](auto)
			{
				event_system::fire_event<window_close_event>();
			});

			s_window.set_window_focus_callback([](auto, auto focused)
			{
				event_system::fire_event<window_focus_event>(focused);
			});

			s_window.set_window_pos_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<window_pos_event>(x, y);
			});

			s_window.set_window_size_callback([](auto, auto w, auto h)
			{
				event_system::fire_event<window_size_event>(w, h);
			});
		}

		return true;
	}

	bool engine::running()
	{
		return s_window.is_open();
	}

	void engine::shutdown()
	{
		s_plugins.clear_plugins();

		s_window.destroy();
		
		window::terminate();

		Python::shutdown();
		
		Lua::shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_loop()
	{
		s_time.m_delta = s_time.m_loop.elapsed().count();
		
		s_time.m_loop.stop().start();

		window::poll_events();
	}

	void engine::begin_draw()
	{
		s_window.clear_color(colors::black);
		
		s_window.viewport({ {}, s_window.get_frame_size() });
		
		constexpr render_states states{
		}; states(); // default states
	}

	void engine::end_loop()
	{
		if (s_window.get_flags() & WindowFlags_DoubleBuffered)
		{
			s_window.swap_buffers();
		}
		GL::flush();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t engine::load_plugin(path_t const & path)
	{
		return s_plugins.load_plugin(path);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}