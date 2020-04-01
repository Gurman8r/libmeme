#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_target::clear_color(color const & col) const
	{
		this->clear_color(col, GL::DepthBufferBit);
	}

	void render_target::clear_color(color const & col, uint32_t flags) const
	{
		GL::clearColor(col[0], col[1], col[2], col[3]);

		this->clear_flags(GL::ColorBufferBit | flags);
	}

	void render_target::clear_flags(uint32_t flags) const
	{
		GL::clear(flags);
	}

	void render_target::draw(VAO const & vao, VBO const & vbo) const
	{
		if (!vao || !vbo) return;
		ML_bind_scope(vao);
		ML_bind_scope(vbo);
		GL::drawArrays(vao.m_mode, 0, vbo.m_size);
		GL::flush();
	}

	void render_target::draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const
	{
		if (!vao || !vbo || !ibo) return;
		ML_bind_scope(vao);
		ML_bind_scope(vbo);
		ML_bind_scope(ibo);
		GL::drawElements(vao.m_mode, ibo.m_usage, ibo.m_type, nullptr);
		GL::flush();
	}

	void render_target::viewport(int_rect const & bounds) const
	{
		GL::viewport(
			bounds.left(),
			bounds.top(),
			bounds.width(),
			bounds.height()
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}