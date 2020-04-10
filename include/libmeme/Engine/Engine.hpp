#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/GameTime.hpp>
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

		class engine_context;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using arguments_t = pmr::vector<cstring>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// public startup variables
		struct engine_config final : trackable
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

		// public runtime variables
		struct engine_io final : trackable
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

		ML_NODISCARD static asset_manager & assets() noexcept;

		ML_NODISCARD static engine_config & config() noexcept;

		ML_NODISCARD static engine_io & io() noexcept;

		ML_NODISCARD static plugin_manager & plugins() noexcept;

		ML_NODISCARD static script_manager & scripts() noexcept;

		ML_NODISCARD static game_time & time() noexcept;

		ML_NODISCARD static render_window & window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static fs::path path_to(fs::path const & value = {})
		{
			if (value.empty())
			{
				return config().content_home;
			}
			else
			{
				return config().content_home.native() + value.native();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_