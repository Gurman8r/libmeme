#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>

#define ML_Engine ::ml::Engine::getInstance()

namespace ml
{
	struct ML_ENGINE_API Engine final : public Singleton<Engine>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<Engine>;

		Engine();

		~Engine();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_