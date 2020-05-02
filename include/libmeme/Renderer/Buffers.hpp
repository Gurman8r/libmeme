#ifndef _ML_BUFFERS_HPP_
#define _ML_BUFFERS_HPP_

// FIXME

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	// VAO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API VAO final
	{
		uint32_t	m_handle{};
		uint32_t	m_mode	{};

		ML_NODISCARD bool nonzero() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

		static void bind(VAO const * value);

		void bind() const { bind(this); }

		void unbind() const { bind(nullptr); }

		VAO & generate(uint32_t mode);

		VAO & destroy();
	};

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vao(Args && ... args)
	{
		return VAO{ ML_forward(args)... };
	}

	// VBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API VBO final
	{
		uint32_t	m_handle{};
		uint32_t	m_usage	{};
		void *		m_data	{};
		uint32_t	m_size	{};
		uint32_t	m_count	{};
		uint32_t	m_offset{};

		ML_NODISCARD bool nonzero() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

		static void bind(VBO const * value);

		void bind() const { bind(this); }

		void unbind() const { bind(nullptr); }

		VBO & generate(uint32_t usage);

		VBO & destroy();

		VBO & update(void * data, uint32_t size);

		VBO & update(void * data, uint32_t size, uint32_t offset);
	};

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vbo(Args && ... args)
	{
		return VBO{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// IBO
	struct ML_RENDERER_API IBO final
	{
		uint32_t	m_handle{};
		uint32_t	m_usage	{};
		uint32_t	m_type	{};
		void *		m_data	{};
		uint32_t	m_count	{};

		ML_NODISCARD bool nonzero() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

		static void bind(IBO const * value);

		void bind() const { bind(this); }

		void unbind() const { bind(nullptr); }

		IBO & generate(uint32_t usage, uint32_t type);

		IBO & destroy();

		IBO & update(void * data, uint32_t count);
	};

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_ibo(Args && ... args)
	{
		return IBO{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FBO
	struct ML_RENDERER_API FBO final
	{
		uint32_t	m_handle			{};
		vec2i		m_size				{};
		uint32_t	m_buffer_attachment	{};
		uint32_t	m_buffer_handle		{};
		uint32_t	m_texture_attachment{};
		uint32_t	m_texture_handle	{};
		uint32_t	m_texture_level		{};

		ML_NODISCARD bool nonzero() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

		static void bind(FBO const * value);

		void bind() const { bind(this); }

		void unbind() const { bind(nullptr); }

		FBO & generate(vec2i const & size);

		FBO & destroy();

		FBO & attach_buffer(uint32_t attachment, uint32_t handle);

		FBO & attach_texture(uint32_t attachment, uint32_t handle, uint32_t level);
	};

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_fbo(Args && ... args)
	{
		return FBO{ ML_forward(args)... };
	}

	// RBO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API RBO final
	{
		uint32_t	m_handle{};
		vec2i		m_size	{};
		uint32_t	m_format{};

		ML_NODISCARD bool nonzero() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

		static void bind(RBO const * value);

		void bind() const { bind(this); }

		void unbind() const { bind(nullptr); }

		RBO & generate(vec2i const & size);

		RBO & destroy();

		RBO & update(uint32_t format);
	};

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_rbo(Args && ... args)
	{
		return RBO{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFERS_HPP_