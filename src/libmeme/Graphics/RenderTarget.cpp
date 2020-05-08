#include <libmeme/Graphics/RenderTarget.hpp>
#include <libmeme/Graphics/Binder.hpp>
#include <libmeme/Graphics/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_target::clear_color(color const & col)
	{
		this->clear_color(col, GL::DepthBufferBit);
	}

	void render_target::clear_color(color const & col, uint32_t flags)
	{
		GL::clearColor(col[0], col[1], col[2], col[3]);

		this->clear_flags(GL::ColorBufferBit | flags);
	}

	void render_target::clear_flags(uint32_t flags)
	{
		GL::clear(flags);
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

	void render_target::flush()
	{
		GL::flush();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ml::render_target::viewport(vec2i const & size)
	{
		this->viewport({ {}, size });
	}

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
}