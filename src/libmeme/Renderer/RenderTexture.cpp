#include <libmeme/Renderer/RenderTexture.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_texture::render_texture()
		: render_texture{ vec2i::zero() }
	{
	}

	render_texture::render_texture(vec2i const & size)
		: render_texture{ size, GL::ColorAttachment0, GL::DepthStencil, GL::Depth24_Stencil8 }
	{
	}

	render_texture::render_texture(vec2i const & size, uint32_t colorID, uint32_t frameID, uint32_t format)
		: m_fbo		{ make_fbo() }
		, m_rbo		{ make_rbo() }
		, m_size	{ size }
		, m_colorID	{ colorID }
		, m_frameID	{ frameID }
		, m_format	{ format }
		, m_texture	{ make_texture(GL::Texture2D) }
	{
	}

	render_texture::render_texture(render_texture const & other)
		: m_fbo		{ other.m_fbo }
		, m_rbo		{ other.m_rbo }
		, m_size	{ other.m_size }
		, m_colorID	{ other.m_colorID }
		, m_frameID	{ other.m_frameID }
		, m_format	{ other.m_format }
		, m_texture	{ other.m_texture }
	{
	}

	render_texture::render_texture(render_texture && other) noexcept
		: render_texture{}
	{
		swap(std::move(other));
	}

	render_texture::~render_texture()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_texture & render_texture::operator=(render_texture const & other)
	{
		render_texture temp{ other };
		swap(temp);
		return (*this);
	}

	render_texture & render_texture::operator=(render_texture && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void render_texture::swap(render_texture & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_fbo, other.m_fbo);
			std::swap(m_rbo, other.m_rbo);
			std::swap(m_size, other.m_size);
			std::swap(m_colorID, other.m_colorID);
			std::swap(m_frameID, other.m_frameID);
			std::swap(m_format, other.m_format);
			std::swap(m_texture, other.m_texture);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_texture::create()
	{
		if (good())
			return debug::log_error("render texture already created");

		if (width() == 0)
			return debug::log_error("render texture width negative or zero");

		if (height() == 0)
			return debug::log_error("render texture height negative or zero");

		if (!m_fbo.generate(m_size))
		{
			destroy();
			return debug::log_error("render texture failed creating framebuffer");
		}

		if (!m_rbo.generate(m_size))
		{
			destroy();
			return debug::log_error("render texture failed creating renderbuffer");
		}

		// bind framebuffer
		ML_BIND_SCOPE_M(m_fbo);
		{
			// bind renderbuffer
			ML_BIND_SCOPE_M(m_rbo);

			// update renderbuffer
			m_rbo.update(m_format);

			// attach renderbuffer to framebuffer
			m_fbo.attach_buffer(m_frameID, m_rbo.handle());
		}

		// check framebuffer status
		if (!GL::checkFramebufferStatus(GL::Framebuffer))
		{
			destroy();
			return debug::log_error("render texture framebuffer status check failed");
		}

		// recreate texture
		if (!m_texture.destroy() || !m_texture.create(m_size))
		{
			destroy();
			return debug::log_error("render texture failed creating texture");
		}

		// attach texture to framebuffer
		m_fbo.attach_texture(m_colorID, m_texture.handle(), m_texture.level());

		return good();
	}

	bool render_texture::destroy()
	{
		m_fbo.destroy();
		m_rbo.destroy();
		m_texture.destroy();
		return true;
	}

	bool render_texture::resize(vec2i const & value)
	{
		if ((value[0] != 0) && (value[1] != 0) && (m_size != value))
		{
			m_size = value;

			return destroy() && create();
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_texture::bind(render_texture const * value)
	{
		frame_buffer::bind(value ? &value->fbo() : nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}