#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Vertex Array Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void vertex_array_object::bind(vertex_array_object const * value)
	{
		GL::bindVertexArray(value ? value->m_handle : NULL);
	}

	vertex_array_object & vertex_array_object::generate(uint32_t mode)
	{
		if (!m_handle)
		{
			m_handle = GL::genVertexArray();

			std::get<ID_Mode>(m_storage) = mode;
		}
		return (*this);
	}

	vertex_array_object & vertex_array_object::destroy()
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

	void vertex_buffer_object::bind(vertex_buffer_object const * value)
	{
		GL::bindBuffer(GL::ArrayBuffer, value ? value->m_handle : NULL);
	}

	vertex_buffer_object & vertex_buffer_object::generate(uint32_t usage)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();

			std::get<ID_Usage>(m_storage) = usage;
		}
		return (*this);
	}

	vertex_buffer_object & vertex_buffer_object::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	vertex_buffer_object & vertex_buffer_object::update(void const * data, uint32_t size)
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

	vertex_buffer_object & vertex_buffer_object::update(void const * data, uint32_t size, uint32_t offset)
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

	void index_buffer_object::bind(index_buffer_object const * value)
	{
		GL::bindBuffer(GL::ElementArrayBuffer, value ? value->m_handle : NULL);
	}

	index_buffer_object & index_buffer_object::generate(uint32_t usage, uint32_t type)
	{
		if (!m_handle)
		{
			m_handle = GL::genBuffer();

			std::get<ID_Usage>(m_storage) = usage;

			std::get<ID_Type>(m_storage) = type;
		}
		return (*this);
	}

	index_buffer_object & index_buffer_object::destroy()
	{
		if (m_handle)
		{
			GL::deleteBuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	index_buffer_object & index_buffer_object::update(void const * data, uint32_t count)
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

	void frame_buffer_object::bind(frame_buffer_object const * value)
	{
		GL::bindFramebuffer(GL::Framebuffer, value ? value->m_handle : NULL);
	}

	frame_buffer_object & frame_buffer_object::generate(vec2 const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genFramebuffer();

			std::get<ID_Size>(m_storage) = size;
		}
		return (*this);
	}

	frame_buffer_object & frame_buffer_object::destroy()
	{
		if (m_handle)
		{
			GL::deleteFramebuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	frame_buffer_object & frame_buffer_object::attachRenderbuffer(uint32_t attachment, uint32_t renderbuffer)
	{
		if (m_handle)
		{
			std::get<ID_Buffer>(m_storage) = std::make_pair(attachment, renderbuffer);

			GL::framebufferRenderbuffer(
				GL::Framebuffer, this->bufferAttachment(),
				GL::Renderbuffer, this->bufferHandle()
			);
		}
		return (*this);
	}

	frame_buffer_object & frame_buffer_object::attachTexture2D(uint32_t attachment, uint32_t tex, uint32_t level)
	{
		if (m_handle)
		{
			std::get<ID_Texture>(m_storage) = std::make_tuple(attachment, tex, level);

			GL::framebufferTexture2D(
				GL::Framebuffer, this->textureAttachment(),
				GL::Texture2D, this->textureHandle(), this->textureLevel()
			);
		}
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Render Buffer Object
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_buffer_object::bind(render_buffer_object const * value)
	{
		GL::bindRenderbuffer(GL::Renderbuffer, value ? value->m_handle : NULL);
	}

	render_buffer_object & render_buffer_object::generate(vec2i const & size)
	{
		if (!m_handle)
		{
			m_handle = GL::genRenderbuffer();

			std::get<ID_Size>(m_storage) = size;
		}
		return (*this);
	}

	render_buffer_object & render_buffer_object::destroy()
	{
		if (m_handle)
		{
			GL::deleteRenderbuffer(&m_handle);

			m_handle = NULL;

			GL::flush();
		}
		return (*this);
	}

	render_buffer_object & render_buffer_object::update(uint32_t format)
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