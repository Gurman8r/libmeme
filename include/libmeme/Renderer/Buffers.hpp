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

		enum : size_t { Mode };

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

		void bind() const;

		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		VertexArrayObject & create(uint32_t mode);

		VertexArrayObject & destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) mode() const noexcept { return std::get<Mode>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Vertex Buffer Object
	struct ML_RENDERER_API VertexBufferObject final : public GraphicsObject<VertexBufferObject>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Usage, Data, Size, Count, Offset };

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

		void bind() const;

		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		VertexBufferObject & create(uint32_t usage);

		VertexBufferObject & destroy();

		VertexBufferObject & update(buffer_t data, uint32_t size);

		VertexBufferObject & update(buffer_t data, uint32_t size, uint32_t offset);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) usage() const noexcept { return std::get<Usage>(m_storage); }
		
		constexpr decltype(auto) data() const noexcept { return std::get<Data>(m_storage); }
		
		constexpr decltype(auto) size() const noexcept { return std::get<Size>(m_storage); }

		constexpr decltype(auto) count() const noexcept { return std::get<Count>(m_storage); }

		constexpr decltype(auto) offset() const noexcept { return std::get<Offset>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Index Buffer Object
	struct ML_RENDERER_API IndexBufferObject final : public GraphicsObject<IndexBufferObject>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Usage, Type, Data, Count };

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

		void bind() const;

		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject & create(uint32_t usage, uint32_t type);

		IndexBufferObject & destroy();

		IndexBufferObject & update(buffer_t data, uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) usage() const noexcept { return std::get<Usage>(m_storage); }

		constexpr decltype(auto) type() const noexcept { return std::get<Type>(m_storage); }
		
		constexpr decltype(auto) data() const noexcept { return std::get<Data>(m_storage); }
		
		constexpr decltype(auto) count() const noexcept { return std::get<Count>(m_storage); }

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

		enum : size_t { Size, BufferID, TextureID };
		
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
		
		void bind() const;

		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBufferObject & create(vec2 const & size);

		FrameBufferObject & destroy();

		FrameBufferObject & attachRenderbuffer(uint32_t attachment, uint32_t renderbuffer);

		FrameBufferObject & attachTexture2D(uint32_t attachment, uint32_t texture, uint32_t level);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) size() const noexcept { return std::get<Size>(m_storage); }

		constexpr decltype(auto) buffer() const noexcept { return std::get<BufferID>(m_storage); }

		constexpr decltype(auto) bufferAttachment() const noexcept { return buffer().first; }

		constexpr decltype(auto) bufferHandle() const noexcept { return buffer().second; }

		constexpr decltype(auto) texture() const noexcept { return std::get<TextureID>(m_storage); }

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

		enum : size_t { Size, Format };

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

		void bind() const;

		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject & create(vec2i const & size);

		RenderBufferObject & destroy();

		RenderBufferObject & update(uint32_t format);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) size() const noexcept { return std::get<Size>(m_storage); }

		constexpr decltype(auto) format() const noexcept { return std::get<Format>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static constexpr auto make_vao(Args && ... args)
	{
		return VertexArrayObject{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_vbo(Args && ... args)
	{
		return VertexBufferObject{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_ibo(Args && ... args)
	{
		return IndexBufferObject{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_fbo(Args && ... args)
	{
		return FrameBufferObject{ std::forward<Args>(args)... };
	}

	template <class ... Args> static constexpr auto make_rbo(Args && ... args)
	{
		return RenderBufferObject{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFERS_HPP_