#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Core/Timer.hpp>
#include <libmeme/Engine/Export.hpp>
#include <libmeme/Engine/FPS_Tracker.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using filenames_t = ds::flat_set<fs::path>;
		using libraries_t = ds::flat_map<struct shared_library, struct plugin *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup
		struct config final : trackable, non_copyable
		{
			filenames_t			command_line	{}		; // command line arguments
			fs::path			library_path	{}		; // script library path
			fs::path			content_path	{}		; // content path
			filenames_t			script_list		{}		; // scripts to run on start
			filenames_t			plugin_list		{}		; // plugins to load on start
			cstring				window_title	{}		; // window title
			video_mode			window_video	{}		; // window video mode
			context_settings	window_context	{}		; // window context settings
			int32_t				window_flags	{}		; // window flags
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime
		struct io final : trackable, non_copyable
		{
			render_window		window			{}		; // 
			filenames_t			plugin_files	{}		; // 
			libraries_t			plugin_libs		{}		; // 
			timer				main_timer		{ 1 }	; //
			timer				loop_timer		{}		; //
			frame_tracker<120>	fps_tracker		{}		; //
			float_t				delta_time		{}		; // 
			size_t				frame_count		{}		; // 
			float_t				frame_rate		{}		; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// context
		class context final : trackable, non_copyable
		{
			friend class		engine					; // 
			engine::config		config			{}		; // 
			engine::io			io				{}		; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool create_context();

		static bool startup(bool install_callbacks);

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool running();

		static void begin_loop();

		static void begin_draw();

		static void end_draw();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static engine::context const * const get_context() noexcept;

		ML_NODISCARD static engine::config & get_config() noexcept;

		ML_NODISCARD static engine::io & get_io() noexcept;

		ML_NODISCARD static duration const & get_time() noexcept;

		ML_NODISCARD static render_window & get_window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_