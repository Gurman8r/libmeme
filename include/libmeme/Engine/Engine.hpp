#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	struct ML_ENGINE_API Engine final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Engine() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct CreateWindowSettings
		{
			std::string		title;
			DisplayMode		display;
			WindowSettings	style;
			ContextSettings context;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(path_t const & name, path_t const & home);

		static bool create_window(CreateWindowSettings const & settings);

		static bool running();

		static void begin_loop();

		static void end_loop();

		static void shutdown();
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline auto & window() { return s_window; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static RenderWindow s_window;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_