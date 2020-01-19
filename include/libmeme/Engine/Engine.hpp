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

		struct engine_time final
		{
			inline float64_t total() const noexcept { return m_main.elapsed().count(); }

			inline float64_t delta() const noexcept { return m_delta; }

		private:
			friend engine;
			timer		m_main, m_loop;
			float64_t	m_delta;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(engine_startup_settings const & s);

		static bool init_window(window_startup_settings const & s);

		static bool running();

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void begin_draw();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t load_plugin(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline auto const & get_time() noexcept { return s_time; }

		ML_NODISCARD static inline auto & get_window() { return s_window; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static engine_time		s_time;
		static render_window	s_window;
		struct engine_plugins;
		static engine_plugins	s_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_