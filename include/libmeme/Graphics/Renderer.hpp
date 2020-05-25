#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

// WIP

#include <libmeme/Graphics/RenderCommand.hpp>

// renderer singleton
#define ML_renderer \
	_ML renderer::get_instance()

namespace ml
{
	struct perspective_camera final {};
	struct orthographic_camera final {};
	
	class renderer3d final : public singleton<renderer3d>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_scene(perspective_camera const & camera) {}

		static void end_scene() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void submit() {}

		static void upload() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<renderer3d>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDERER_HPP_