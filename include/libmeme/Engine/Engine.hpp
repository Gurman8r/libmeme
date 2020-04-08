#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Core/Timer.hpp>
#include <libmeme/Engine/FrameTracker.hpp>
#include <libmeme/Engine/AssetManager.hpp>
#include <libmeme/Engine/PluginManager.hpp>
#include <libmeme/Engine/ScriptManager.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class context;
		struct config;
		struct io;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using arguments_t = pmr::vector<cstring>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// engine config
		struct config final : trackable
		{
			arguments_t		command_line		{}				; // command line
			fs::path		content_home		{}				; // content home
			fs::path		library_home		{}				; // library home
			fs::path		program_path		{}				; // program path
			fs::path		program_name		{}				; // program name
			fs::path		setup_script		{}				; // setup script
			window_settings	window_settings		{}				; // window settings
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// engine io
		struct io final : trackable
		{
			float_t			delta_time			{}				; // frame time
			size_t			frame_count			{}				; // frame count
			float_t			frame_rate			{}				; // frame rate
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool is_initialized() noexcept;

		ML_NODISCARD static bool create_context(json const & j);

		ML_NODISCARD static bool destroy_context();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup();

		ML_NODISCARD static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void begin_draw();

		static void end_draw();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static fs::path path_to(fs::path const & value = {})
		{
			if (value.empty())
			{
				return get_config().content_home;
			}
			else
			{
				return get_config().content_home.native() + value.native();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static asset_manager & get_assets() noexcept;

		ML_NODISCARD static config & get_config() noexcept;

		ML_NODISCARD static io & get_io() noexcept;

		ML_NODISCARD static plugin_manager & get_plugins() noexcept;

		ML_NODISCARD static script_manager & get_scripts() noexcept;

		ML_NODISCARD static duration const & get_time() noexcept;

		ML_NODISCARD static render_window & get_window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_