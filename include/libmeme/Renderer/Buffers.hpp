#ifndef _ML_BUFFERS_HPP_
#define _ML_BUFFERS_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	// Base Graphics Object
	template <class T> struct GraphicsObject
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type	= typename T;
		using handle_t	= typename uint32_t;
		using address_t	= typename void *;
		using buffer_t	= typename void const *;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit GraphicsObject(handle_t handle) noexcept
			: m_handle{ handle }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const noexcept { return m_handle; }

		constexpr auto address() const noexcept -> address_t { return ML_ADDRESSOF(m_handle); }

		constexpr auto handle() const noexcept -> handle_t const & { return m_handle; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		handle_t m_handle{ NULL };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Vertex Array Object
	struct ML_RENDERER_API VertexArrayObject final : public GraphicsObject<VertexArrayObject>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Mode };

		using storage_t = typename std::tuple<
			uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit VertexArrayObject(handle_t handle, Args && ... args)
			: GraphicsObject{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr VertexArrayObject(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr VertexArrayObject(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr VertexArrayObject()
			: self_type{ NULL, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				alg::swap(m_handle, other.m_handle);

				alg::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(VertexArrayObject const * value);

		inline void bind() const { bind(this); }

		inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		VertexArrayObject & generate(uint32_t mode);

		VertexArrayObject & destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) mode() const noexcept { return std::get<ID_Mode>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Vertex Buffer Object
	struct ML_RENDERER_API VertexBufferObject final : public GraphicsObject<VertexBufferObject>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Usage, ID_Data, ID_Size, ID_Count, ID_Offset };

		using storage_t = typename std::tuple<
			uint32_t, buffer_t, uint32_t, uint32_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit VertexBufferObject(handle_t handle, Args && ... args)
			: GraphicsObject{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr VertexBufferObject(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr VertexBufferObject(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr VertexBufferObject()
			: self_type{ NULL, 0, nullptr, 0, 0, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				alg::swap(m_handle, other.m_handle);

				alg::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(VertexBufferObject const * value);

		inline void bind() const { bind(this); }

		inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		VertexBufferObject & generate(uint32_t usage);

		VertexBufferObject & destroy();

		VertexBufferObject & update(buffer_t data, uint32_t size);

		VertexBufferObject & update(buffer_t data, uint32_t size, uint32_t offset);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) usage() const noexcept { return std::get<ID_Usage>(m_storage); }
		
		constexpr decltype(auto) data() const noexcept { return std::get<ID_Data>(m_storage); }
		
		constexpr decltype(auto) size() const noexcept { return std::get<ID_Size>(m_storage); }

		constexpr decltype(auto) count() const noexcept { return std::get<ID_Count>(m_storage); }

		constexpr decltype(auto) offset() const noexcept { return std::get<ID_Offset>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Index Buffer Object
	struct ML_RENDERER_API IndexBufferObject final : public GraphicsObject<IndexBufferObject>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Usage, ID_Type, ID_Data, ID_Count };

		using storage_t = typename std::tuple<
			uint32_t, uint32_t, buffer_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit IndexBufferObject(handle_t handle, Args && ... args)
			: GraphicsObject{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr IndexBufferObject(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr IndexBufferObject(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr IndexBufferObject()
			: self_type{ NULL, 0, 0, nullptr, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				alg::swap(m_handle, other.m_handle);

				alg::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(IndexBufferObject const * value);

		inline void bind() const { bind(this); }

		inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject & generate(uint32_t usage, uint32_t type);

		IndexBufferObject & destroy();

		IndexBufferObject & update(buffer_t data, uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) usage() const noexcept { return std::get<ID_Usage>(m_storage); }

		constexpr decltype(auto) type() const noexcept { return std::get<ID_Type>(m_storage); }
		
		constexpr decltype(auto) data() const noexcept { return std::get<ID_Data>(m_storage); }
		
		constexpr decltype(auto) count() const noexcept { return std::get<ID_Count>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Frame Buffer Object
	struct ML_RENDERER_API FrameBufferObject final : public GraphicsObject<FrameBufferObject>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using BufAttachment = typename std::pair<
			uint32_t,	// attachment
			uint32_t	// handle
		>;
		
		using TexAttachment = typename std::tuple<
			uint32_t,	// attachment
			uint32_t,	// handle
			uint32_t	// level
		>;

		enum : size_t { ID_Size, ID_Buffer, ID_Texture };
		
		using storage_t = typename std::tuple<
			vec2, BufAttachment, TexAttachment
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit FrameBufferObject(handle_t handle, Args && ... args)
			: GraphicsObject{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr FrameBufferObject(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr FrameBufferObject(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr FrameBufferObject()
			: self_type{ NULL, vec2{ 0 }, std::make_pair(0, 0), std::make_tuple(0, 0, 0) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				alg::swap(m_handle, other.m_handle);

				alg::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(FrameBufferObject const * value);
		
		inline void bind() const { bind(this); }

		inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBufferObject & generate(vec2 const & size);

		FrameBufferObject & destroy();

		FrameBufferObject & attachRenderbuffer(uint32_t attachment, uint32_t renderbuffer);

		FrameBufferObject & attachTexture2D(uint32_t attachment, uint32_t texture, uint32_t level);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) size() const noexcept { return std::get<ID_Size>(m_storage); }

		constexpr decltype(auto) buffer() const noexcept { return std::get<ID_Buffer>(m_storage); }

		constexpr decltype(auto) bufferAttachment() const noexcept { return buffer().first; }

		constexpr decltype(auto) bufferHandle() const noexcept { return buffer().second; }

		constexpr decltype(auto) texture() const noexcept { return std::get<ID_Texture>(m_storage); }

		constexpr decltype(auto) textureAttachment() const noexcept { return std::get<0>(texture()); }

		constexpr decltype(auto) textureHandle() const noexcept { return std::get<1>(texture()); }

		constexpr decltype(auto) textureLevel() const noexcept { return std::get<2>(texture()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Render Buffer Object
	struct ML_RENDERER_API RenderBufferObject final : public GraphicsObject<RenderBufferObject>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Size, ID_Format };

		using storage_t = typename std::tuple<
			vec2i, uint32_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit RenderBufferObject(handle_t handle, Args && ... args)
			: GraphicsObject{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr RenderBufferObject(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr RenderBufferObject(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr RenderBufferObject()
			: self_type{ NULL, vec2{ 0 }, 0, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				alg::swap(m_handle, other.m_handle);

				alg::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(RenderBufferObject const * value);

		inline void bind() const { bind(this); }

		inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject & generate(vec2i const & size);

		RenderBufferObject & destroy();

		RenderBufferObject & update(uint32_t format);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) size() const noexcept { return std::get<ID_Size>(m_storage); }

		constexpr decltype(auto) format() const noexcept { return std::get<ID_Format>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING VAO = typename VertexArrayObject;
	ML_USING VBO = typename VertexBufferObject;
	ML_USING IBO = typename IndexBufferObject;
	ML_USING FBO = typename FrameBufferObject;
	ML_USING RBO = typename RenderBufferObject;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static constexpr auto make_vao(Args && ... args)
	{
		return VAO{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_vbo(Args && ... args)
	{
		return VBO{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_ibo(Args && ... args)
	{
		return IBO{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_fbo(Args && ... args)
	{
		return FBO{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_rbo(Args && ... args)
	{
		return RBO{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFERS_HPP_