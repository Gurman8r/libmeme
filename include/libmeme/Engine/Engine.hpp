#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>
#include <libmeme/Platform/FileSystem.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct engine_startup_settings final
		{
			path_t program_name;
			path_t library_path;
			std::initializer_list<path_t> boot_scripts;
		};

		struct window_startup_settings final
		{
			C_string			title;
			video_mode			display;
			context_settings	context;
			int32_t				flags;
			bool				install_callbacks;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class plugins final
		{
			friend engine;

			ds::flat_set<path_t> files;

			ds::flat_map<struct shared_library, struct plugin *> libs;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class time final
		{
			friend engine;
			timer m_main, m_loop;
			float64_t m_delta;

		public:
			inline float64_t total() const noexcept { return m_main.elapsed().count(); }

			inline float64_t delta() const noexcept { return m_delta; }
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(engine_startup_settings const & s);

		static bool create_window(window_startup_settings const & s);

		static bool running();

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void begin_draw();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static engine::plugins const & get_plugins() noexcept;

		ML_NODISCARD static engine::time const & get_time() noexcept;

		ML_NODISCARD static render_window & get_window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_