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

	void render_target::draw(vertex_array const & vao, vertex_buffer const & vbo) const
	{
		if (!vao || !vbo) return;
		ML_BIND_SCOPE_M(vao);
		ML_BIND_SCOPE_M(vbo);
		GL::drawArrays(vao.mode(), 0, vbo.size());
		GL::flush();
	}

	void render_target::draw(vertex_array const & vao, vertex_buffer const & vbo, index_buffer const & ibo) const
	{
		if (!vao || !vbo || !ibo) return;
		ML_BIND_SCOPE_M(vao);
		ML_BIND_SCOPE_M(vbo);
		ML_BIND_SCOPE_M(ibo);
		GL::drawElements(vao.mode(), ibo.usage(), ibo.type(), nullptr);
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