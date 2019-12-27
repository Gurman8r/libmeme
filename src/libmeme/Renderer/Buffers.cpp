#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// VAO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool VAO::create(VAO * value, uint32_t mode)
	{
		if (value && !(*value))
		{
			value->m_handle = GL::genVertexArray();

			std::get<0>(value->m_storage) = mode;

			return true;
		}
		return false;
	}

	void VAO::bind(VAO const * value)
	{
		if (value && (*value))
		{
			GL::bindVertexArray(value->m_handle);
		}
		else
		{
			GL::bindVertexArray(NULL);
		}
	}

	bool VAO::destroy(VAO * value)
	{
		if (value)
		{
			if (*value)
			{
				GL::deleteVertexArray(&value->m_handle);
			}
			
			value->m_handle = NULL;
			
			return true;
		}
		return false;
	}

	VAO & VAO::create(uint32_t mode)
	{
		create(this, mode);
		return (*this);
	}

	void VAO::bind() const
	{
		return bind(this);
	}

	bool VAO::destroy()
	{
		return destroy(this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// VBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool VBO::create(VBO * value, uint32_t usage)
	{
		if (value && !(*value))
		{
			value->m_handle = GL::genBuffer();

			std::get<0>(value->m_storage) = usage;

			return true;
		}
		return false;
	}

	void VBO::bind(VBO const * value)
	{
		if (value && (*value))
		{
			GL::bindBuffer(GL::ArrayBuffer, value->m_handle);
		}
		else
		{
			GL::bindBuffer(GL::ArrayBuffer, NULL);
		}
	}

	bool VBO::destroy(VBO * value)
	{
		if (value)
		{
			if (*value)
			{
				GL::deleteBuffer(&value->m_handle);
			}
			
			value->m_handle = NULL;
			
			return true;
		}
		return false;
	}

	void VBO::update(VBO * value, buffer_t data, uint32_t size)
	{
		if (value && (*value))
		{
			std::get<1>(value->m_storage) = data;
			std::get<2>(value->m_storage) = size;
			std::get<3>(value->m_storage) = size / Vertex::Size;
			std::get<4>(value->m_storage) = 0;

			GL::bufferData(
				GL::ArrayBuffer,
				value->size() * sizeof(float_t),
				const_cast<void *>(value->data()),
				value->usage()
			);
		}
	}

	void VBO::update(VBO * value, buffer_t data, uint32_t size, uint32_t offset)
	{
		if (value && (*value))
		{
			std::get<1>(value->m_storage) = data;
			std::get<2>(value->m_storage) = size;
			std::get<3>(value->m_storage) = size / Vertex::Size;
			std::get<4>(value->m_storage) = offset;

			GL::bufferSubData(
				GL::ArrayBuffer,
				value->offset(),
				value->size() * sizeof(float_t),
				const_cast<void *>(value->data())
			);
		}
	}

	VBO & VBO::create(uint32_t usage)
	{
		create(this, usage);
		return (*this);
	}

	void VBO::bind() const
	{
		return bind(this);
	}

	bool VBO::destroy()
	{
		return destroy(this);
	}

	VBO & VBO::update(buffer_t data, uint32_t size)
	{
		VBO::update(this, data, size);
		return (*this);
	}

	VBO & VBO::update(buffer_t data, uint32_t size, uint32_t offset)
	{
		VBO::update(this, data, size, offset);
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// IBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool IBO::create(IBO * value, uint32_t usage, uint32_t type)
	{
		if (value && !(*value))
		{
			value->m_handle = GL::genBuffer();

			std::get<0>(value->m_storage) = usage;

			std::get<1>(value->m_storage) = type;

			return true;
		}
		return false;
	}

	void IBO::bind(IBO const * value)
	{
		if (value && (*value))
		{
			GL::bindBuffer(GL::ElementArrayBuffer, value->m_handle);
		}
		else
		{
			GL::bindBuffer(GL::ElementArrayBuffer, NULL);
		}
	}

	bool IBO::destroy(IBO * value)
	{
		if (value)
		{
			if (*value)
			{
				GL::deleteBuffer(&value->m_handle);
			}
			
			value->m_handle = NULL;
			
			return true;
		}
		return false;
	}

	void IBO::update(IBO * value, buffer_t data, uint32_t count)
	{
		if (value && (*value))
		{
			std::get<2>(value->m_storage) = data;
			std::get<3>(value->m_storage) = count;

			GL::bufferData(
				GL::ElementArrayBuffer,
				value->count() * sizeof(uint32_t),
				const_cast<void *>(value->data()),
				value->usage()
			);
		}
	}

	IBO & IBO::create(uint32_t usage, uint32_t type)
	{
		create(this, usage, type);
		return (*this);
	}

	void IBO::bind() const
	{
		return bind(this);
	}

	bool IBO::destroy()
	{
		return destroy(this);
	}

	IBO & IBO::update(buffer_t data, uint32_t count)
	{
		IBO::update(this, data, count);
		return (*this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// FBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool FBO::create(FBO * value, vec2 const & size)
	{
		if (value && !(*value))
		{
			value->m_handle = GL::genFramebuffer();

			std::get<0>(value->m_storage) = size;

			return true;
		}
		return false;
	}

	void FBO::bind(FBO const * value)
	{
		if (value && (*value))
		{
			GL::bindFramebuffer(GL::Renderbuffer, value->m_handle);
		}
		else
		{
			GL::bindRenderbuffer(GL::Renderbuffer, NULL);
		}
	}

	bool FBO::destroy(FBO * value)
	{
		if (value)
		{
			if (*value)
			{
				GL::deleteFramebuffer(&value->m_handle);
			}
			
			value->m_handle = NULL;
			
			return true;
		}
		return false;
	}

	FBO & FBO::create(vec2 const & size)
	{
		create(this, size);
		return (*this);
	}

	void FBO::bind() const
	{
		return bind(this);
	}

	bool FBO::destroy()
	{
		return destroy(this);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// RBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool RBO::create(RBO * value, vec2 const & size, uint32_t type)
	{
		if (value && !(*value))
		{
			value->m_handle = GL::genRenderbuffer();

			std::get<0>(value->m_storage) = size;

			std::get<1>(value->m_storage) = type;

			return true;
		}
		return false;
	}

	void RBO::bind(RBO const * value)
	{
		if (value && (*value))
		{
			GL::bindRenderbuffer(GL::Renderbuffer, value->m_handle);
		}
		else
		{
			GL::bindRenderbuffer(GL::Renderbuffer, NULL);
		}
	}

	bool RBO::destroy(RBO * value)
	{
		if (value)
		{
			if (*value)
			{
				GL::deleteRenderbuffer(&value->m_handle);
			}
			
			value->m_handle = NULL;
			
			return true;
		}
		return false;
	}

	RBO & RBO::create(vec2 const & size, uint32_t type)
	{
		create(this, size, type);
		return (*this);
	}

	void RBO::bind() const
	{
		return bind(this);
	}

	bool RBO::destroy()
	{
		return destroy(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}