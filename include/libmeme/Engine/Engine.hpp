#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>
#include <libmeme/Platform/FileSystem.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	struct ML_ENGINE_API Engine final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Engine() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct EngineSettings final
		{
			path_t program_name;
			path_t library_path;
		};

		struct WindowSettings final
		{
			C_string		title;
			VideoMode		display;
			ContextSettings context;
			int32_t			flags;
			bool			install_callbacks;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Time final
		{
			Timer		main{ true },	loop{ false };
			float64_t	total{ 0. },	delta{ 0. };
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(EngineSettings const & es);

		static bool init_window(WindowSettings const & ws);

		static bool running();

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void begin_draw();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t load_plugin(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline auto const & time() noexcept { return s_time; }

		ML_NODISCARD static inline auto & window() { return s_window; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static Time s_time;
		static RenderWindow s_window;

		struct PluginLoader;
		static PluginLoader s_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_