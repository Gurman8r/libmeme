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
			fs::path			program_name	{}		; // program name
			filenames_t			command_line	{}		; // command line arguments
			fs::path			library_path	{}		; // script library path
			fs::path			content_path	{}		; // content path
			filenames_t			script_list		{}		; // scripts to run on start
			filenames_t			plugin_list		{}		; // plugins to load on start
			pmr::string			window_title	{}		; // window title
			video_mode			window_video	{}		; // window video mode
			context_settings	window_context	{}		; // window context settings
			int32_t				window_flags	{}		; // window flags
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime
		struct io final : trackable, non_copyable
		{
			float_t				delta_time		{}		; // frame time
			size_t				frame_count		{}		; // frame count
			float_t				frame_rate		{}		; // frame rate
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// context
		struct context final : trackable, non_copyable
		{
			engine::config		config			{}		; // startup settings
			engine::io			io				{}		; // runtime variables

		private:
			friend class		engine					; // private data
			render_window		window			{}		; // main window
			filenames_t			plugin_files	{}		; // plugin filenames
			libraries_t			plugin_libs		{}		; // plugin instances
			timer				main_timer		{}		; // master timer
			timer				loop_timer		{}		; // frame timer
			frame_tracker<120>	fps_tracker		{}		; // frame rate tracker
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool create_context();

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static engine::context * const get_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup(bool install_callbacks);

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_draw();

		static void end_draw();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline engine::context & ref() noexcept
		{
			ML_BREAK_IF(!initialized());
			ML_ASSERT("engine is not initialized" && initialized());
			return (*get_context());
		}

		ML_NODISCARD static inline engine::context const & cref() noexcept
		{
			return static_cast<engine::context const &>(ref());
		}

		ML_NODISCARD static inline engine::config & get_config() noexcept
		{
			return ref().config;
		}

		ML_NODISCARD static inline engine::io & get_io() noexcept
		{
			return ref().io;
		}

		ML_NODISCARD static inline duration const & get_time() noexcept
		{
			return ref().main_timer.elapsed();
		}

		ML_NODISCARD static inline render_window & get_window() noexcept
		{
			return ref().window;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_