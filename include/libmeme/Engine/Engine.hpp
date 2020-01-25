#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct config final
		{
			// paths
			fs::path				program_name;
			fs::path				library_path;
			pmr::vector<fs::path>	script_list{ pmr::get_default_resource() };
			pmr::vector<fs::path>	plugin_list{ pmr::get_default_resource() };

			// window
			C_string				window_title;
			video_mode				window_video;
			context_settings		window_context;
			int32_t					window_flags;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct io final
		{
			// time
			float64_t delta_time;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class context final : trackable
		{
			friend engine;

			config			g_config	{};
			io				g_io		{};
			render_window	g_window	{};
			timer			g_main_timer{ true };
			timer			g_loop_timer{ false };

			struct plugins
			{
				ds::flat_set<fs::path> files;
				ds::flat_map<struct shared_library, struct plugin *> libs;
			}
			g_plugins;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static engine::context const * create_context();

		static bool startup(bool install_callbacks);

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool running();

		static void begin_loop();

		static void begin_draw();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static engine::context const * get_context() noexcept;

		ML_NODISCARD static engine::config & get_config() noexcept;

		ML_NODISCARD static engine::io & get_io() noexcept;

		ML_NODISCARD static duration const & get_time() noexcept;

		ML_NODISCARD static render_window & get_window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_