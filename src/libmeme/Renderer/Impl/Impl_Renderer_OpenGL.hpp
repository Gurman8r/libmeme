#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Impl_Renderer.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (!ML_is_debug)
#	define glCheck(expr) (expr)
#else
#	define glCheck(expr) \
	do { expr; _ML gl_check_error(__FILE__, __LINE__, #expr); } while (0)
#endif

namespace ml
{
	static void gl_check_error(cstring file, uint32_t line, cstring expr)
	{
		if (auto const code{ glGetError() }; code != GL_NO_ERROR)
		{
			pmr::string path{ file };
			path = path.substr(path.find_last_of("\\/") + 1);
			pmr::string err_name{ "No Error" };
			pmr::string err_desc{ "No Description" };
			switch (code)
			{
			case GL_INVALID_ENUM:
				err_name = "Invalid Enum";
				err_desc = "An unacceptable value has been specified for an enumerated argument";
				break;
			case GL_INVALID_VALUE:
				err_name = "Invalid Value";
				err_desc = "A numeric argument is out of range";
				break;
			case GL_INVALID_OPERATION:
				err_name = "Invalid Operation";
				err_desc = "The specified operation is not allowed in the current state";
				break;
			case GL_STACK_OVERFLOW:
				err_name = "Stack Overflow";
				err_desc = "This command would cause a stack overflow";
				break;
			case GL_STACK_UNDERFLOW:
				err_name = "Stack Underflow";
				err_desc = "This command would cause a stack underflow";
				break;
			case GL_OUT_OF_MEMORY:
				err_name = "Out of Memory";
				err_desc = "There is not enough memory left to execute the command";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				err_name = "Invalid Framebuffer Operation";
				err_desc = "The object bound to framebuffer binding is not \'framebuffer complete\'";
				break;
			}
			std::cout
				<< "OpenGL Error " << code << ":\n"
				<< " \'" << file << "\' (" << line << ")\n"
				<< "expr:\n"
				<< "\t" << expr << "\n"
				<< "desc:\n"
				<< "\t" << err_name << "\n"
				<< "\t" << err_desc << "\n"
				<< "\n\n";
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl buffers
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex array
	struct ML_RENDERER_API opengl_vertex_array final : vertex_array
	{
		using self_type = opengl_vertex_array;

		opengl_vertex_array() { glCheck(glGenVertexArrays(1, &m_handle)); }

		~opengl_vertex_array() { glCheck(glDeleteVertexArrays(1, &m_handle)); }

		opengl_vertex_array(uint32_t mode) : self_type{}
		{
			m_mode = mode;
		}

		static void bind(opengl_vertex_array const * value)
		{
			glCheck(glBindVertexArray(value ? value->m_handle : NULL));
		}

		void bind() const override { bind(this); }

		void unbind() const override { bind(nullptr); }

		void const * get_handle() const noexcept override { return std::addressof(m_handle); }

		uint32_t get_mode() const noexcept override { return m_mode; }

	private:
		uint32_t m_handle	{}						; // handle
		uint32_t m_mode		{ GL_TRIANGLES }		; // primitive type
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex buffer
	struct ML_RENDERER_API opengl_vertex_buffer final : vertex_buffer
	{
		using self_type = opengl_vertex_buffer;

		opengl_vertex_buffer() { glCheck(glGenBuffers(1, &m_handle)); }

		~opengl_vertex_buffer() { glCheck(glDeleteBuffers(1, &m_handle)); }

		opengl_vertex_buffer(float_t const * vertices, uint32_t size) : self_type{}
		{
			this->set_data(vertices, size);
		}

		opengl_vertex_buffer(float_t const * vertices, uint32_t size, uint32_t offset) : self_type{}
		{
			this->set_sub_data(vertices, size, offset);
		}

		static void bind(opengl_vertex_buffer const * value)
		{
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, value ? value->m_handle : NULL));
		}

		void bind() const override { bind(this); }

		void unbind() const override { bind(nullptr); }

		void set_data(float_t const * vertices, uint32_t size) override
		{
			glCheck(glBufferData(GL_ARRAY_BUFFER, (m_size = size), vertices, GL_STATIC_DRAW));
		}

		void set_sub_data(float_t const * vertices, uint32_t size, uint32_t offset) override
		{
			glCheck(glBufferSubData(GL_ARRAY_BUFFER, offset, (m_size = size), vertices));
		}

		void const * get_handle() const noexcept override { return std::addressof(m_handle); }

		uint32_t get_size() const noexcept override { return m_size; }

	private:
		uint32_t m_handle	{}; // handle
		uint32_t m_size		{}; // size
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl index buffer
	struct ML_RENDERER_API opengl_index_buffer final : index_buffer
	{
		using self_type = opengl_index_buffer;

		opengl_index_buffer() { glCheck(glGenBuffers(1, &m_handle)); }

		~opengl_index_buffer() { glCheck(glDeleteBuffers(1, &m_handle)); }

		opengl_index_buffer(uint32_t const * indices, uint32_t count) : self_type{}
		{
			this->set_data(indices, count);
		}

		static void bind(opengl_index_buffer const * value)
		{
			glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value ? value->m_handle : NULL));
		}

		void bind() const override { bind(this); }

		void unbind() const override { bind(nullptr); }

		void set_data(uint32_t const * indices, uint32_t count) override
		{
			glCheck(glBufferData(
				GL_ELEMENT_ARRAY_BUFFER, (m_count = count) * sizeof(uint32_t), indices, GL_UNSIGNED_INT));
		}

		void const * get_handle() const noexcept override { return std::addressof(m_handle); }

		uint32_t get_count() const noexcept override { return m_count; }

	private:
		uint32_t m_handle	{}; // handle
		uint32_t m_count	{}; // count
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl frame buffer
	struct ML_RENDERER_API opengl_frame_buffer final : frame_buffer
	{
		using self_type = opengl_frame_buffer;

		opengl_frame_buffer() { glCheck(glGenFramebuffers(1, &m_handle)); }

		~opengl_frame_buffer() { glCheck(glDeleteFramebuffers(1, &m_handle)); }

		static void bind(opengl_frame_buffer const * value)
		{
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER, value ? value->m_handle : NULL));
		}

		void bind() const override { bind(this); }

		void unbind() const override { bind(nullptr); }

		void set_render_buffer(void const * value, uint32_t attachment) override
		{
			glCheck(glFramebufferRenderbuffer(
				GL_FRAMEBUFFER, attachment,
				GL_RENDERBUFFER, *reinterpret_cast<uint32_t const *>(value)));
		}

		void set_texture2d(void const * value, uint32_t attachment, uint32_t level) override
		{
			glCheck(glFramebufferTexture2D(
				GL_FRAMEBUFFER, attachment,
				GL_TEXTURE_2D, *reinterpret_cast<uint32_t const *>(value), level));
		}

		void const * get_handle() const noexcept override { return std::addressof(m_handle); }

	private:
		uint32_t m_handle{}; // handle
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl render buffer
	struct ML_RENDERER_API opengl_render_buffer final : render_buffer
	{
		using self_type = opengl_render_buffer;

		opengl_render_buffer() { glCheck(glGenRenderbuffers(1, &m_handle)); }

		~opengl_render_buffer() { glCheck(glDeleteRenderbuffers(1, &m_handle)); }

		opengl_render_buffer(uint32_t format, int32_t width, int32_t height) : self_type{}
		{
			this->set_storage(format, width, height);
		}

		static void bind(opengl_render_buffer const * value)
		{
			glCheck(glBindRenderbuffer(GL_RENDERBUFFER, value ? value->m_handle : NULL));
		}

		void bind() const override { bind(this); }

		void unbind() const override { bind(nullptr); }

		void set_storage(uint32_t format, int32_t width, int32_t height) override
		{
			glCheck(glRenderbufferStorage(
				GL_RENDERBUFFER, format, width, height));
		}

		void const * get_handle() const noexcept override { return std::addressof(m_handle); }

	private:
		uint32_t m_handle{}; // handle
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl render api
namespace ml
{
	struct ML_RENDERER_API opengl_render_api final : render_api
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		opengl_render_api() noexcept = default;
		~opengl_render_api() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void clear(uint32_t flags) override;
		void flush() override;

		void set_clear_color(color const & value) override;
		void set_viewport(int_rect const & value) override;

		void draw_arrays(uint32_t mode, uint32_t first, uint32_t count) override;
		void draw_indexed(uint32_t mode, int32_t first, uint32_t type, void * indices) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL