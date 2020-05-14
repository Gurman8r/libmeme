#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

// WIP

#include <libmeme/Renderer/RendererAPI.hpp>

// renderer singleton
#define ML_renderer \
	_ML renderer::get_instance()

namespace ml
{
	// global renderer
	class ML_RENDERER_API renderer final : public singleton<renderer>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_scene() {}

		static void end_scene() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void submit() {}

		static void upload() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<renderer>;

		renderer() noexcept;

		~renderer() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDERER_HPP_