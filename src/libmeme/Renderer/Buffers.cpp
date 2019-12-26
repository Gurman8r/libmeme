#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	// VAO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void VAO::bind(self_type const * value)
	{
		if (value && (*value))
		{
			GL::bindVertexArray(value->handle());
		}
		else
		{
			GL::bindVertexArray(NULL);
		}
	}

	VAO VAO::create(uint32_t mode)
	{
		return self_type{ GL::genVertexArray(), mode };
	}

	bool VAO::destroy(self_type const * value)
	{
		return (value && (*value)) ? GL::deleteVertexArray(&value->handle()) : false;
	}


	// VBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void VBO::bind(self_type const * value)
	{
		if (value && (*value))
		{
			GL::bindBuffer(GL::ArrayBuffer, value->handle());
		}
		else
		{
			GL::bindBuffer(GL::ArrayBuffer, NULL);
		}
	}

	VBO VBO::create(uint32_t usage, buffer_t data, uint32_t size, uint32_t count)
	{
		return self_type{ GL::genBuffer(), usage, data, size, count };
	}

	bool VBO::destroy(self_type const * value)
	{
		return (value && (*value)) ? GL::deleteBuffer(&value->handle()) : false;
	}


	// IBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void IBO::bind(self_type const * value)
	{
		if (value && (*value))
		{
			GL::bindBuffer(GL::ElementArrayBuffer, value->handle());
		}
		else
		{
			GL::bindBuffer(GL::ElementArrayBuffer, NULL);
		}
	}

	IBO IBO::create(uint32_t usage, buffer_t data, uint32_t type, uint32_t count)
	{
		return self_type{ GL::genBuffer(), usage, data, type, count };
	}

	bool IBO::destroy(self_type const * value)
	{
		return (value && (*value)) ? GL::deleteBuffer(&value->handle()) : false;
	}


	// FBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FBO::bind(self_type const * value)
	{
		if (value && (*value))
		{
			GL::bindFramebuffer(GL::Renderbuffer, value->handle());
		}
		else
		{
			GL::bindRenderbuffer(GL::Renderbuffer, NULL);
		}
	}

	FBO FBO::create(vec2 const & size)
	{
		return self_type{ GL::genFramebuffer(), size };
	}

	bool FBO::destroy(self_type const * value)
	{
		return (value && (*value)) ? GL::deleteFramebuffer(&value->handle()) : false;
	}


	// RBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RBO::bind(self_type const * value)
	{
		if (value && (*value))
		{
			GL::bindRenderbuffer(GL::Renderbuffer, value->handle());
		}
		else
		{
			GL::bindRenderbuffer(GL::Renderbuffer, NULL);
		}
	}

	RBO RBO::create(vec2 const & size, uint32_t storage)
	{
		return self_type{ GL::genRenderbuffer(), size, storage };
	}

	bool RBO::destroy(self_type const * value)
	{
		return (value && (*value)) ? GL::deleteRenderbuffer(&value->handle()) : false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}