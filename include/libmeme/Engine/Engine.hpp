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
		struct config;
		struct runtime;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using arguments_t	= pmr::vector<cstring>;
		using filenames_t	= ds::flat_set<filesystem::path>;
		using libraries_t	= ds::flat_map<struct shared_library, struct plugin *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup variables
		struct config final : trackable
		{
			arguments_t			command_line	{}			; // command line
			filesystem::path	program_path	{}			; // program path
			filesystem::path	program_name	{}			; // program name
			filesystem::path	content_home	{}			; // content path
			filesystem::path	library_home	{}			; // script library path
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime variables
		struct runtime final : trackable
		{
			float_t				delta_time		{}			; // frame time
			size_t				frame_count		{}			; // frame count
			float_t				frame_rate		{}			; // frame rate
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// engine context
		class context final : trackable, non_copyable
		{
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static engine::config & get_config() noexcept;

		ML_NODISCARD static engine::runtime & get_runtime() noexcept;

		ML_NODISCARD static duration const & get_time() noexcept;

		ML_NODISCARD static render_window & get_window() noexcept;

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

		static int32_t do_script(int32_t lang, pmr::string const & text);

		static int32_t do_script(filesystem::path const & path);

		template <int32_t Lang
		> inline static int32_t do_script(pmr::string const & text)
		{
			return do_script(Lang, text);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline bool is_running() noexcept
		{
			return is_initialized() && get_window().is_open();
		}

		static inline void close() noexcept
		{
			if (is_initialized()) { get_window().close(); }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline filesystem::path path_to(filesystem::path const & value = {})
		{
			auto const & home{ get_config().content_home };
			if (home.empty()) return value;
			if (value.empty()) return home;
			return filesystem::path{ home.native() + value.native() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_