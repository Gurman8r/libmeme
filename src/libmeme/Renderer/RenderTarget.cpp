#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderTarget::clear_color(Color const & color) const
	{
		this->clear_color(color, GL::DepthBufferBit);
	}

	void RenderTarget::clear_color(Color const & color, uint32_t flags) const
	{
		GL::clearColor(color[0], color[1], color[2], color[3]);

		this->clear_flags(GL::ColorBufferBit | flags);
	}

	void RenderTarget::clear_flags(uint32_t flags) const
	{
		GL::clear(flags);
	}

	void RenderTarget::draw(VAO const & vao, VBO const & vbo) const
	{
		if (!vao || !vao) return;
		ML_BIND_SCOPE_M(vao);
		ML_BIND_SCOPE_M(vbo);
		GL::drawArrays(vao.mode(), 0, vbo.size());
		GL::flush();
	}

	void RenderTarget::draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const
	{
		if (!vao || !vao || !ibo) return;
		ML_BIND_SCOPE_M(vao);
		ML_BIND_SCOPE_M(vbo);
		ML_BIND_SCOPE_M(ibo);
		GL::drawElements(vao.mode(), ibo.count(), ibo.type(), nullptr);
		GL::flush();
	}

	void RenderTarget::viewport(int_rect const & bounds) const
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