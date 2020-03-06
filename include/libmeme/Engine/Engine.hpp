#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Core/Timer.hpp>
#include <libmeme/Engine/Export.hpp>
#include <libmeme/Engine/FrameTracker.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class context;
		class config;
		class runtime;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using arguments_t	= pmr::vector<pmr::string>;
		using warguments_t	= pmr::vector<pmr::wstring>;
		using filenames_t	= ds::flat_set<filesystem::path>;
		using libraries_t	= ds::flat_map<struct shared_library, struct plugin *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup variables
		class config final : trackable, non_copyable
		{
			friend class context;
		public:
			arguments_t			command_line	{}			; // command line
			filesystem::path	program_name	{}			; // programe name
			filesystem::path	program_path	{}			; // program path
			filesystem::path	content_path	{}			; // content path
			filesystem::path	library_home	{}			; // script library path

			cstring				window_title	{}			; // 
			video_mode			window_display	{}			; // 
			context_settings	window_context	{}			; // 
			int32_t				window_hints	{}			; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime variables
		class runtime final : trackable, non_copyable
		{
			friend class context;
		public:
			float_t				delta_time		{}			; // frame time
			size_t				frame_count		{}			; // frame count
			float_t				frame_rate		{}			; // frame rate
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// engine context
		class context final : trackable, non_copyable
		{
		private:
			friend class		engine						;
			engine::config		m_config		{}			; // startup variables
			engine::runtime		m_io			{}			; // runtime variables
			timer				m_main_timer	{ true }	; // master timer
			timer				m_loop_timer	{}			; // frame timer
			frame_tracker<120>	m_fps_tracker	{}			; // frame rate tracker
			render_window		m_window		{}			; // main window
			filenames_t			m_plugin_files	{}			; // plugin filenames
			libraries_t			m_plugin_libs	{}			; // plugin instances
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool is_initialized() noexcept;

		ML_NODISCARD static bool create_context();

		ML_NODISCARD static bool destroy_context();

		ML_NODISCARD static engine::context * const get_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup();

		ML_NODISCARD static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_draw();

		static void end_draw();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(filesystem::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline engine::config & get_config() noexcept
		{
			return get_context()->m_config;
		}

		ML_NODISCARD static inline engine::runtime & get_io() noexcept
		{
			return get_context()->m_io;
		}

		ML_NODISCARD static inline duration const & get_time() noexcept
		{
			return get_context()->m_main_timer.elapsed();
		}

		ML_NODISCARD static inline render_window * const get_window() noexcept
		{
			return &get_context()->m_window;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline filesystem::path path_to(filesystem::path const & value = {})
		{
			auto const & cpath{ get_config().content_path };
			if (cpath.empty()) return value;
			if (value.empty()) return cpath;
			return filesystem::path{ cpath.native() + value.native() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_