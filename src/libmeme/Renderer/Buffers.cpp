#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/Vertex.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	// VAO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void VAO::bind(VAO const * value)
	{
		GL::bindVertexArray(value ? value->m_handle : NULL);
	}

	VAO & VAO::generate(uint32_t mode)
	{
		if (!m_handle)
		{
			m_handle = GL::genVertexArray();
			m_mode = mode;
		}
		return (*this);
	}

	VAO & VAO::destroy()
	{
		if (m_handle)
		{
			GL::deleteVertexArray(&m_handle);
			m_handle = NULL;
			GL::flush();
		}
		return (*this);
	}

	// VBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void VBO::bind(VBO const * value)
	{
		GL::bindBuffer(GL::ArrayBuffer, value ? value->m_handle : NULL);
	}

	VBO & VBO::generate(uint32_t usage)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();
			m_usage = usage;
		}
		return (*this);
	}

	VBO & VBO::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);
			m_handle = NULL;
			GL::flush();
		}
		return (*this);
	}

	VBO & VBO::update(void * data, uint32_t size)
	{
		if (m_handle)
		{
			m_data = data;
			m_size = size;
			m_count = size / vertex::size;

			GL::bufferData(
				GL::ArrayBuffer,
				sizeof(float_t) * m_size,
				m_data,
				m_usage
			);
		}
		return (*this);
	}

	VBO & VBO::update(void * data, uint32_t size, uint32_t offset)
	{
		if (m_handle)
		{
			m_data = data;
			m_size = size;
			m_count = size / vertex::size;

			GL::bufferSubData(
				GL::ArrayBuffer,
				offset,
				m_size * sizeof(float_t),
				m_data
			);
		}
		return (*this);
	}

	// IBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void IBO::bind(IBO const * value)
	{
		GL::bindBuffer(GL::ElementArrayBuffer, value ? value->m_handle : NULL);
	}

	IBO & IBO::generate(uint32_t usage, uint32_t type)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();
			m_usage = usage;
			m_type = type;
		}
		return (*this);
	}

	IBO & IBO::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);
			m_handle = NULL;
			GL::flush();
		}
		return (*this);
	}

	IBO & IBO::update(void * data, uint32_t count)
	{
		if (m_handle)
		{
			m_data = data;
			m_count = count;

			GL::bufferData(
				GL::ElementArrayBuffer,
				m_count * sizeof(uint32_t),
				m_data,
				m_usage
			);
		}
		return (*this);
	}

	// FBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FBO::bind(FBO const * value)
	{
		GL::bindFramebuffer(GL::Framebuffer, value ? value->m_handle : NULL);
	}

	FBO & FBO::generate(vec2i const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genFramebuffer();
			m_size = size;
		}
		return (*this);
	}

	FBO & FBO::destroy()
	{
		if (m_handle)
		{
			GL::deleteFramebuffer(&m_handle);
			m_handle = NULL;
			GL::flush();
		}
		return (*this);
	}

	FBO & FBO::attach_buffer(uint32_t attachment, uint32_t handle)
	{
		if (m_handle)
		{
			m_buffer_attachment = attachment;
			m_buffer_handle = handle;

			GL::framebufferRenderbuffer(
				GL::Framebuffer, attachment,
				GL::Renderbuffer, handle
			);
		}
		return (*this);
	}

	FBO & FBO::attach_texture(uint32_t attachment, uint32_t handle, uint32_t level)
	{
		if (m_handle)
		{
			m_texture_attachment = attachment;
			m_texture_handle = handle;
			m_texture_level = level;

			GL::framebufferTexture2D(
				GL::Framebuffer, attachment,
				GL::Texture2D, handle, level
			);
		}
		return (*this);
	}

	// RBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RBO::bind(RBO const * value)
	{
		GL::bindRenderbuffer(GL::Renderbuffer, value ? value->m_handle : NULL);
	}

	RBO & RBO::generate(vec2i const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genRenderbuffer();
			m_size = size;
		}
		return (*this);
	}

	RBO & RBO::destroy()
	{
		if (m_handle)
		{
			GL::deleteRenderbuffer(&m_handle);
			m_handle = NULL;
			GL::flush();
		}
		return (*this);
	}

	RBO & RBO::update(uint32_t format)
	{
		if (m_handle)
		{
			m_format = format;

			GL::renderbufferStorage(
				GL::Renderbuffer, format, m_size[0], m_size[1]
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}