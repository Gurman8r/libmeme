#include <libmeme/Graphics/RenderTarget.hpp>
#include <libmeme/Graphics/Binder.hpp>
#include <libmeme/Renderer/Renderer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_target::draw(VAO const & vao, VBO const & vbo)
	{
		if (!vao || !vbo) { return; }
		ML_bind_scope(vao);
		ML_bind_scope(vbo);
		render_command::draw_arrays(vao.m_mode, 0, vbo.m_size)();
	}

	void render_target::draw(VAO const & vao, VBO const & vbo, IBO const & ibo)
	{
		if (!vao || !vbo || !ibo) { return; }
		ML_bind_scope(vao);
		ML_bind_scope(vbo);
		ML_bind_scope(ibo);
		render_command::draw_indexed(vao.m_mode, ibo.m_count, ibo.m_type, nullptr)();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}