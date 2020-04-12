#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_target::clear_color(color const & value)
	{
		this->clear_color(value, GL::DepthBufferBit);
	}

	void render_target::clear_color(color const & value, uint32_t flags)
	{
		GL::clearColor(value[0], value[1], value[2], value[3]);

		this->clear_flags(GL::ColorBufferBit | flags);
	}

	void render_target::clear_flags(uint32_t flags)
	{
		GL::clear(flags);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_target::viewport(int_rect const & bounds)
	{
		this->viewport(bounds.left(), bounds.top(), bounds.width(), bounds.height());
	}

	void render_target::viewport(vec2i const & pos, vec2i const & size)
	{
		this->viewport(pos[0], pos[1], size[0], size[1]);
	}

	void render_target::viewport(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		GL::viewport(x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_target::draw(VAO const & vao, VBO const & vbo)
	{
		if (!vao || !vbo) { return; }
		ML_defer{ GL::flush(); };
		ML_bind_scope(vao);
		ML_bind_scope(vbo);
		GL::drawArrays(vao.m_mode, 0, vbo.m_size);
	}

	void render_target::draw(VAO const & vao, VBO const & vbo, IBO const & ibo)
	{
		if (!vao || !vbo || !ibo) { return; }
		ML_defer{ GL::flush(); };
		ML_bind_scope(vao);
		ML_bind_scope(vbo);
		ML_bind_scope(ibo);
		GL::drawElements(vao.m_mode, ibo.m_count, ibo.m_type, nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}