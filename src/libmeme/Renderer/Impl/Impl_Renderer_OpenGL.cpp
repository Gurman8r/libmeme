#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::clear(uint32_t flags)
	{
		glCheck(glClear(flags));
	}

	void opengl_render_api::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_viewport(int_rect const & value)
	{
		glCheck(glViewport(value[0], value[1], value[2], value[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::draw_arrays(uint32_t mode, uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(mode, first, count));
	}

	void opengl_render_api::draw_indexed(uint32_t mode, int32_t first, uint32_t type, void * indices)
	{
		glCheck(glDrawElements(mode, first, type, indices));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL