#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_device		= _ML_GFX opengl_device			;
using impl_vertexarray	= _ML_GFX opengl_vertexarray	;
using impl_vertexbuffer	= _ML_GFX opengl_vertexbuffer	;
using impl_indexbuffer	= _ML_GFX opengl_indexbuffer	;
using impl_texture2d	= _ML_GFX opengl_texture2d		;
using impl_texturecube	= _ML_GFX opengl_texturecube	;
using impl_framebuffer	= _ML_GFX opengl_framebuffer	;
using impl_program		= _ML_GFX opengl_shader			;
using impl_shader		= _ML_GFX opengl_program		;

#elif defined(ML_IMPL_RENDERER_DIRECTX)
#elif defined(ML_IMPL_RENDERER_VULKAN)
// etc...

#else
#error "Unknown or invalid renderer implementation specified."
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gfx
{
	static device * g_devctx{};

	bool device::create_context(context_settings const & cs)
	{
		bool temp{};
		static ML_scope // once
		{
			if (!g_devctx && (g_devctx = new impl_device))
			{
				if (temp = g_devctx->do_initialize(cs))
				{
					get_context()->on_initialize();
				}
				else
				{
					delete g_devctx;
				}
			}
		};
		return temp;
	}

	unique<device> const & device::get_context() noexcept
	{
		static unique<device> temp{ g_devctx };
		return temp;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gfx
{
	shared<vertexarray> vertexarray::allocate(uint32_t mode) noexcept
	{
		return _ML make_shared<impl_vertexarray>(mode);
	}

	void vertexarray::bind(vertexarray const * value) noexcept
	{
		impl_vertexarray::do_bind(static_cast<impl_vertexarray const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gfx
{
	shared<vertexbuffer> vertexbuffer::allocate(uint32_t usage, size_t count, address_t data) noexcept
	{
		return _ML make_shared<impl_vertexbuffer>(usage, count, data);
	}

	void vertexbuffer::bind(vertexbuffer const * value) noexcept
	{
		impl_vertexbuffer::do_bind(static_cast<impl_vertexbuffer const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gfx
{
	shared<indexbuffer> indexbuffer::allocate(uint32_t usage, size_t count, address_t data) noexcept
	{
		return _ML make_shared<impl_indexbuffer>(usage, count, data);
	}
	
	void indexbuffer::bind(indexbuffer const * value) noexcept
	{
		impl_indexbuffer::do_bind(static_cast<impl_indexbuffer const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture
namespace ml::gfx
{
	void texture::bind(texture const * value, uint32_t slot) noexcept
	{
		switch (value ? value->get_texture_type() : texture_type_2d)
		{
		case texture_type_2d:
			return texture2d::bind(static_cast<texture2d const *>(value), slot);

		case texture_type_cubemap:
			return texturecube::bind(static_cast<texturecube const *>(value), slot);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gfx
{
	shared<texture2d> texture2d::allocate(texopts const & opts, address_t data) noexcept
	{
		return _ML make_shared<impl_texture2d>(opts, data);
	}

	void texture2d::bind(texture2d const * value, uint32_t slot) noexcept
	{
		impl_texture2d::do_bind(static_cast<impl_texture2d const *>(value), slot);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube
namespace ml::gfx
{
	shared<texturecube> texturecube::allocate(texopts const & opts) noexcept
	{
		return _ML make_shared<impl_texturecube>(opts);
	}
	
	void texturecube::bind(texturecube const * value, uint32_t slot) noexcept
	{
		impl_texturecube::do_bind(static_cast<impl_texturecube const *>(value), slot);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gfx
{
	shared<framebuffer> framebuffer::allocate(texopts const & opts)
	{
		return _ML make_shared<impl_framebuffer>(opts);
	}
	
	void framebuffer::bind(framebuffer const * value) noexcept
	{
		impl_framebuffer::do_bind(static_cast<impl_framebuffer const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gfx
{
	shared<shader> shader::allocate(uint32_t type) noexcept
	{
		return _ML make_shared<impl_program>(type);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	shared<program> program::allocate() noexcept
	{
		return _ML make_shared<impl_shader>();
	}

	void program::bind(program const * value) noexcept
	{
		impl_shader::do_bind(static_cast<impl_shader const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */