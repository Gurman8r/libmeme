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
		gl::render_command::draw_arrays(gl::primitive_triangles, 0, vbo.m_size)();
	}

	void render_target::draw(VAO const & vao, VBO const & vbo, IBO const & ibo)
	{
		if (!vao || !vbo || !ibo) { return; }
		ML_bind_scope(vao);
		ML_bind_scope(vbo);
		ML_bind_scope(ibo);
		gl::render_command::draw_indexed(gl::primitive_triangles, ibo.m_count, gl::type_unsigned_int, nullptr)();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}