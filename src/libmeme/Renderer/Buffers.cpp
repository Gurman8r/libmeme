#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Vertex Array Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void vertex_array::bind(vertex_array const * value)
	{
		GL::bindVertexArray(value ? value->m_handle : NULL);
	}

	vertex_array & vertex_array::generate(uint32_t mode)
	{
		if (!m_handle)
		{
			m_handle = GL::genVertexArray();

			std::get<ID_Mode>(m_storage) = mode;
		}
		return (*this);
	}

	vertex_array & vertex_array::destroy()
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

	void vertex_buffer::bind(vertex_buffer const * value)
	{
		GL::bindBuffer(GL::ArrayBuffer, value ? value->m_handle : NULL);
	}

	vertex_buffer & vertex_buffer::generate(uint32_t usage)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();

			std::get<ID_Usage>(m_storage) = usage;
		}
		return (*this);
	}

	vertex_buffer & vertex_buffer::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	vertex_buffer & vertex_buffer::update(void const * data, uint32_t size)
	{
		if (m_handle)
		{
			std::get<ID_Data>(m_storage) = data;
			
			std::get<ID_Size>(m_storage) = size;
			
			std::get<ID_Count>(m_storage) = size / vertex::size;
			
			std::get<ID_Offset>(m_storage) = 0;

			GL::bufferData(
				GL::ArrayBuffer,
				this->size() * sizeof(float_t),
				this->data(),
				this->usage()
			);
		}
		return (*this);
	}

	vertex_buffer & vertex_buffer::update(void const * data, uint32_t size, uint32_t offset)
	{
		if (m_handle)
		{
			std::get<ID_Data>(m_storage) = data;
			
			std::get<ID_Size>(m_storage) = size;
			
			std::get<ID_Count>(m_storage) = size / vertex::size;
			
			std::get<ID_Offset>(m_storage) = offset;

			GL::bufferSubData(
				GL::ArrayBuffer,
				this->offset(),
				this->size() * sizeof(float_t),
				this->data()
			);
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Index Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void index_buffer::bind(index_buffer const * value)
	{
		GL::bindBuffer(GL::ElementArrayBuffer, value ? value->m_handle : NULL);
	}

	index_buffer & index_buffer::generate(uint32_t usage, uint32_t type)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();

			std::get<ID_Usage>(m_storage) = usage;

			std::get<ID_Type>(m_storage) = type;
		}
		return (*this);
	}

	index_buffer & index_buffer::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	index_buffer & index_buffer::update(void const * data, uint32_t count)
	{
		if (m_handle)
		{
			std::get<ID_Data>(m_storage) = data;
			
			std::get<ID_Count>(m_storage) = count;

			GL::bufferData(
				GL::ElementArrayBuffer,
				this->count() * sizeof(uint32_t),
				this->data(),
				this->usage()
			);
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Frame Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void frame_buffer::bind(frame_buffer const * value)
	{
		GL::bindFramebuffer(GL::Framebuffer, value ? value->m_handle : NULL);
	}

	frame_buffer & frame_buffer::generate(vec2 const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genFramebuffer();

			std::get<ID_Size>(m_storage) = size;
		}
		return (*this);
	}

	frame_buffer & frame_buffer::destroy()
	{
		if (m_handle)
		{
			GL::deleteFramebuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	frame_buffer & frame_buffer::attach_buffer(uint32_t attachment, uint32_t buf)
	{
		if (m_handle)
		{
			std::get<ID_BufferAttachment>(m_storage) = attachment;
			
			std::get<ID_BufferHandle>(m_storage) = buf;

			GL::framebufferRenderbuffer(
				GL::Framebuffer, this->buffer_attachment(),
				GL::Renderbuffer, this->buffer_handle()
			);
		}
		return (*this);
	}

	frame_buffer & frame_buffer::attach_texture(uint32_t attachment, uint32_t tex, uint32_t level)
	{
		if (m_handle)
		{
			std::get<ID_TextureAttachment>(m_storage) = attachment;
			
			std::get<ID_TextureHandle>(m_storage) = tex;
			
			std::get<ID_TextureLevel>(m_storage) = level;

			GL::framebufferTexture2D(
				GL::Framebuffer, this->texture_attachment(),
				GL::Texture2D, this->texture_handle(), this->texture_level()
			);
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Render Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_buffer::bind(render_buffer const * value)
	{
		GL::bindRenderbuffer(GL::Renderbuffer, value ? value->m_handle : NULL);
	}

	render_buffer & render_buffer::generate(vec2i const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genRenderbuffer();

			std::get<ID_Size>(m_storage) = size;
		}
		return (*this);
	}

	render_buffer & render_buffer::destroy()
	{
		if (m_handle)
		{
			GL::deleteRenderbuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	render_buffer & render_buffer::update(uint32_t format)
	{
		if (m_handle)
		{
			std::get<ID_Format>(m_storage) = format;

			GL::renderbufferStorage(
				GL::Renderbuffer, format, size()[0], size()[1]
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}