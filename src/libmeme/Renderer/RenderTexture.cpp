#include <libmeme/Renderer/RenderTexture.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTexture::RenderTexture()
		: RenderTexture{ vec2i::zero() }
	{
	}

	RenderTexture::RenderTexture(vec2i const & size)
		: RenderTexture{ size, GL::ColorAttachment0, GL::DepthStencil, GL::Depth24_Stencil8 }
	{
	}

	RenderTexture::RenderTexture(vec2i const & size, uint32_t colorID, uint32_t frameID, uint32_t format)
		: m_fbo		{ make_fbo() }
		, m_rbo		{ make_rbo() }
		, m_size	{ size }
		, m_colorID	{ colorID }
		, m_frameID	{ frameID }
		, m_format	{ format }
		, m_texture	{ make_texture() }
	{
	}

	RenderTexture::RenderTexture(RenderTexture const & other)
		: m_fbo		{ other.m_fbo }
		, m_rbo		{ other.m_rbo }
		, m_size	{ other.m_size }
		, m_colorID	{ other.m_colorID }
		, m_frameID	{ other.m_frameID }
		, m_format	{ other.m_format }
		, m_texture	{ other.m_texture }
	{
	}

	RenderTexture::RenderTexture(RenderTexture && other) noexcept
		: RenderTexture{}
	{
		swap(std::move(other));
	}

	RenderTexture::~RenderTexture()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTexture & RenderTexture::operator=(RenderTexture const & other)
	{
		RenderTexture temp{ other };
		swap(temp);
		return (*this);
	}

	RenderTexture & RenderTexture::operator=(RenderTexture && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void RenderTexture::swap(RenderTexture & other) noexcept
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

	bool RenderTexture::create()
	{
		if (!m_fbo && !m_rbo && ((m_size[0] != 0) && (m_size[1] != 0)))
		{
			if (ML_BIND(FrameBufferObject, m_fbo.create(m_size)))
			{
				if (ML_BIND(RenderBufferObject, m_rbo.create(m_size)))
				{
					m_rbo.update(m_format);

					m_fbo.attachRenderbuffer(m_frameID, m_rbo.handle());
				}

				if (GL::checkFramebufferStatus(GL::Framebuffer))
				{
					m_texture.destroy();

					m_texture.create(m_size);

					m_fbo.attachTexture2D(m_colorID, m_texture.handle(), m_texture.level());
				}
			}
		}
		return (m_fbo && m_rbo);
	}

	bool RenderTexture::destroy()
	{
		m_fbo.destroy();
		m_rbo.destroy();
		m_texture.destroy();
		return true;
	}

	bool RenderTexture::resize(vec2i const & value)
	{
		if ((value[0] != 0) && (value[1] != 0) && (m_size != value))
		{
			m_size = value;

			return destroy() && create();
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderTexture::bind(RenderTexture const * value)
	{
		if (value)
		{
			value->fbo().bind();
		}
		else
		{
			FrameBufferObject::bind(nullptr);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}