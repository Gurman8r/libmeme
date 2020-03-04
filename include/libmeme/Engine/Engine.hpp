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

		class context;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using strings_t		= pmr::vector<pmr::string>;
		using filenames_t	= ds::flat_set<fs::path>;
		using libraries_t	= ds::flat_map<struct shared_library, struct plugin *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup
		class config final : trackable, non_copyable
		{
		public:
			strings_t			command_line	{}			; // command line
			fs::path			program_name	{}			; // program name
			fs::path			program_path	{}			; // program path
			fs::path			content_path	{}			; // content path
			fs::path			library_home	{}			; // script library path
		
		private: friend class context;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime
		class io final : trackable, non_copyable
		{
		public:
			float_t				delta_time		{}			; // frame time
			size_t				frame_count		{}			; // frame count
			float_t				frame_rate		{}			; // frame rate
		
		private: friend class context;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// context
		class context final : trackable, non_copyable
		{
		private:
			friend class		engine						;
			engine::config		m_config		{}			; // startup settings
			engine::io			m_io			{}			; // runtime variables
			timer				m_main_timer	{ true }	; // master timer
			timer				m_loop_timer	{}			; // frame timer
			frame_tracker<120>	m_fps_tracker	{}			; // frame rate tracker
			render_window		m_window		{}			; // main window
			filenames_t			m_plugin_files	{}			; // plugin filenames
			libraries_t			m_plugin_libs	{}			; // plugin instances
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static bool create_context();

		static bool destroy_context();

		ML_NODISCARD static engine::context * const get_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup(bool install_callbacks);

		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_draw();

		static void end_draw();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline engine::config & get_config() noexcept
		{
			return get_context()->m_config;
		}

		ML_NODISCARD static inline engine::io & get_io() noexcept
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

		ML_NODISCARD static inline fs::path path_to(fs::path const & path)
		{
			if (path.empty()) return get_config().content_path;
			return fs::path{ get_config().content_path.string() + path.string() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_