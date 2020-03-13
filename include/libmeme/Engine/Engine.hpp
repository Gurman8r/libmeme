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
		using file_list_t	= pmr::vector<filesystem::path>;
		using file_set_t	= ds::flat_set<filesystem::path>;
		using libraries_t	= ds::flat_map<struct shared_library, struct plugin *>;
		using script_fun_t	= std::function<int32_t()>;
		using script_lib_t	= ds::flat_map<hash_t, pmr::vector<script_fun_t>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// engine config
		struct config final : trackable
		{
			arguments_t			arguments		{}			; // arguments
			filesystem::path	content_home	{}			; // content home
			filesystem::path	library_home	{}			; // library home
			filesystem::path	program_path	{}			; // program path
			filesystem::path	program_name	{}			; // program name
			filesystem::path	setup_script	{}			; // setup script
			window_settings		window_settings	{}			; // window settings
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// engine runtime
		struct runtime final : trackable
		{
			float_t				delta_time		{}			; // frame time
			size_t				frame_count		{}			; // frame count
			float_t				frame_rate		{}			; // frame rate
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool is_initialized() noexcept;

		ML_NODISCARD static bool create_context(json const & j);

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

		ML_NODISCARD static inline bool is_running() noexcept
		{
			return is_initialized() && get_window().is_open();
		}

		static inline void close() noexcept
		{
			if (is_initialized()) { get_window().close(); }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(filesystem::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_callback(pmr::string const & id, script_fun_t const & fn);

		static void run_callback(pmr::string const & id);

		static int32_t do_script(int32_t lang, pmr::string const & text);

		static int32_t do_script(filesystem::path const & path);

		template <int32_t Lang
		> inline static int32_t do_script(pmr::string const & text)
		{
			return do_script(Lang, text);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline filesystem::path path_to(filesystem::path const & value = {})
		{
			if (value.empty())
				return {};
			else if (auto const & home{ get_config().content_home }; home.empty())
				return value;
			else
				return filesystem::path{ home.native() + value.native() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_