#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Vertex Array Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void VertexArrayObject::bind(VertexArrayObject const * value)
	{
		return value
			? GL::bindVertexArray(value->m_handle)
			: GL::bindVertexArray(NULL);
	}

	void VertexArrayObject::bind() const
	{
		return bind(this);
	}

	void VertexArrayObject::unbind() const
	{
		return bind(nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexArrayObject & VertexArrayObject::create(uint32_t mode)
	{
		if (!m_handle)
		{
			m_handle = GL::genVertexArray();

			std::get<Mode>(m_storage) = mode;
		}
		return (*this);
	}

	VertexArrayObject & VertexArrayObject::destroy()
	{
		if (m_handle)
		{
			GL::deleteVertexArray(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Vertex Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void VertexBufferObject::bind(VertexBufferObject const * value)
	{
		return value
			? GL::bindBuffer(GL::ArrayBuffer, value->m_handle)
			: GL::bindBuffer(GL::ArrayBuffer, NULL);
	}

	void VertexBufferObject::bind() const
	{
		return bind(this);
	}

	void VertexBufferObject::unbind() const
	{
		return bind(nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexBufferObject & VertexBufferObject::create(uint32_t usage)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();

			std::get<Usage>(m_storage) = usage;
		}
		return (*this);
	}

	VertexBufferObject & VertexBufferObject::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	VertexBufferObject & VertexBufferObject::update(buffer_t data, uint32_t size)
	{
		if (m_handle)
		{
			std::get<Data>(m_storage) = data;
			std::get<Size>(m_storage) = size;
			std::get<Count>(m_storage) = size / Vertex::Size;
			std::get<Offset>(m_storage) = 0;

			GL::bufferData(
				GL::ArrayBuffer,
				this->size() * sizeof(float_t),
				const_cast<void *>(this->data()),
				this->usage()
			);
		}
		return (*this);
	}

	VertexBufferObject & VertexBufferObject::update(buffer_t data, uint32_t size, uint32_t offset)
	{
		if (m_handle)
		{
			std::get<Data>(m_storage) = data;
			std::get<Size>(m_storage) = size;
			std::get<Count>(m_storage) = size / Vertex::Size;
			std::get<Offset>(m_storage) = offset;

			GL::bufferSubData(
				GL::ArrayBuffer,
				this->offset(),
				this->size() * sizeof(float_t),
				const_cast<void *>(this->data())
			);
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Index Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void IndexBufferObject::bind(IndexBufferObject const * value)
	{
		return value
			? GL::bindBuffer(GL::ElementArrayBuffer, value->m_handle)
			: GL::bindBuffer(GL::ElementArrayBuffer, NULL);
	}

	void IndexBufferObject::bind() const
	{
		return bind(this);
	}

	void IndexBufferObject::unbind() const
	{
		return bind(nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	IndexBufferObject & IndexBufferObject::create(uint32_t usage, uint32_t type)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();

			std::get<Usage>(m_storage) = usage;

			std::get<Type>(m_storage) = type;
		}
		return (*this);
	}

	IndexBufferObject & IndexBufferObject::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	IndexBufferObject & IndexBufferObject::update(buffer_t data, uint32_t count)
	{
		if (m_handle)
		{
			std::get<Data>(m_storage) = data;
			std::get<Count>(m_storage) = count;

			GL::bufferData(
				GL::ElementArrayBuffer,
				this->count() * sizeof(uint32_t),
				const_cast<void *>(this->data()),
				this->usage()
			);
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Frame Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FrameBufferObject::bind(FrameBufferObject const * value)
	{
		return value
			? GL::bindFramebuffer(GL::Framebuffer, value->m_handle)
			: GL::bindFramebuffer(GL::Framebuffer, NULL);
	}

	void FrameBufferObject::bind() const
	{
		return bind(this);
	}

	void FrameBufferObject::unbind() const
	{
		return bind(nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FrameBufferObject & FrameBufferObject::create(vec2 const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genFramebuffer();

			std::get<Size>(m_storage) = size;
		}
		return (*this);
	}

	FrameBufferObject & FrameBufferObject::destroy()
	{
		if (m_handle)
		{
			GL::deleteFramebuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	FrameBufferObject & FrameBufferObject::attachRenderbuffer(uint32_t attachment, uint32_t renderbuffer)
	{
		if (m_handle)
		{
			std::get<BufferID>(m_storage) = std::make_pair(attachment, renderbuffer);

			GL::framebufferRenderbuffer(
				GL::Framebuffer, attachment,
				GL::Renderbuffer, renderbuffer
			);
		}
		return (*this);
	}

	FrameBufferObject & FrameBufferObject::attachTexture2D(uint32_t attachment, uint32_t texture, uint32_t level)
	{
		if (m_handle)
		{
			std::get<TextureID>(m_storage) = std::make_tuple(attachment, texture, level);

			GL::framebufferTexture2D(
				GL::Framebuffer, attachment,
				GL::Texture2D, texture, level
			);
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Render Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderBufferObject::bind(RenderBufferObject const * value)
	{
		return value
			? GL::bindRenderbuffer(GL::Renderbuffer, value->m_handle)
			: GL::bindRenderbuffer(GL::Renderbuffer, NULL);
	}

	void RenderBufferObject::bind() const
	{
		return bind(this);
	}

	void RenderBufferObject::unbind() const
	{
		return bind(nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderBufferObject & RenderBufferObject::create(vec2i const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genRenderbuffer();

			std::get<Size>(m_storage) = size;
		}
		return (*this);
	}

	RenderBufferObject & RenderBufferObject::destroy()
	{
		if (m_handle)
		{
			GL::deleteRenderbuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	RenderBufferObject & RenderBufferObject::update(uint32_t format)
	{
		if (m_handle)
		{
			std::get<Format>(m_storage) = format;

			GL::renderbufferStorage(
				GL::Renderbuffer, format, size()[0], size()[1]
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}