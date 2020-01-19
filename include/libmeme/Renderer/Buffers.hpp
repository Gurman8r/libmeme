#ifndef _ML_BUFFERS_HPP_
#define _ML_BUFFERS_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	// Base Graphics Object
	template <class T> struct graphics_buffer
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type	= typename T;
		using handle_t	= typename uint32_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit graphics_buffer(handle_t handle) noexcept
			: m_handle{ handle }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator bool() const noexcept { return m_handle; }

		ML_NODISCARD constexpr auto address() const noexcept -> void * { return ML_ADDRESSOF(m_handle); }

		ML_NODISCARD constexpr auto handle() const noexcept -> handle_t const & { return m_handle; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		handle_t m_handle;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Vertex Array Object
	struct ML_RENDERER_API vertex_array_object final : graphics_buffer<vertex_array_object>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Mode };

		using storage_type = typename std::tuple<
			uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit vertex_array_object(handle_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr vertex_array_object(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr vertex_array_object(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr vertex_array_object()
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
				util::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(vertex_array_object const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		vertex_array_object & generate(uint32_t mode);

		vertex_array_object & destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto mode() const noexcept
		{
			return std::get<ID_Mode>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Vertex Buffer Object
	struct ML_RENDERER_API vertex_buffer_object final : graphics_buffer<vertex_buffer_object>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Usage, ID_Data, ID_Size, ID_Count, ID_Offset };

		using storage_type = typename std::tuple<
			uint32_t, void const *, uint32_t, uint32_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit vertex_buffer_object(handle_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr vertex_buffer_object(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr vertex_buffer_object(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr vertex_buffer_object()
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
				util::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(vertex_buffer_object const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		vertex_buffer_object & generate(uint32_t usage);

		vertex_buffer_object & destroy();

		vertex_buffer_object & update(void const * data, uint32_t size);

		vertex_buffer_object & update(void const * data, uint32_t size, uint32_t offset);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto usage() const noexcept
		{
			return std::get<ID_Usage>(m_storage);
		}

		ML_NODISCARD constexpr auto data() const noexcept
		{
			return std::get<ID_Data>(m_storage);
		}

		ML_NODISCARD constexpr auto size() const noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		ML_NODISCARD constexpr auto count() const noexcept
		{
			return std::get<ID_Count>(m_storage);
		}

		ML_NODISCARD constexpr auto offset() const noexcept
		{
			return std::get<ID_Offset>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Index Buffer Object
	struct ML_RENDERER_API index_buffer_object final : graphics_buffer<index_buffer_object>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Usage, ID_Type, ID_Data, ID_Count };

		using storage_type = typename std::tuple<
			uint32_t, uint32_t, void const *, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit index_buffer_object(handle_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr index_buffer_object(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr index_buffer_object(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr index_buffer_object()
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
				util::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(index_buffer_object const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		index_buffer_object & generate(uint32_t usage, uint32_t type);

		index_buffer_object & destroy();

		index_buffer_object & update(void const * data, uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto usage() const noexcept
		{
			return std::get<ID_Usage>(m_storage);
		}

		ML_NODISCARD constexpr auto type() const noexcept
		{
			return std::get<ID_Type>(m_storage);
		}

		ML_NODISCARD constexpr auto data() const noexcept
		{
			return std::get<ID_Data>(m_storage);
		}

		ML_NODISCARD constexpr auto count() const noexcept
		{
			return std::get<ID_Count>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Frame Buffer Object
	struct ML_RENDERER_API frame_buffer_object final : graphics_buffer<frame_buffer_object>
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
		
		using storage_type = typename std::tuple<
			vec2, BufAttachment, TexAttachment
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit frame_buffer_object(handle_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr frame_buffer_object(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr frame_buffer_object(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr frame_buffer_object()
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
				util::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static void bind(frame_buffer_object const * value);
		
		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		frame_buffer_object & generate(vec2 const & size);

		frame_buffer_object & destroy();

		frame_buffer_object & attachRenderbuffer(uint32_t attachment, uint32_t renderbuffer);

		frame_buffer_object & attachTexture2D(uint32_t attachment, uint32_t tex, uint32_t level);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto size() const noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		ML_NODISCARD constexpr auto buffer_data() const noexcept
		{
			return std::get<ID_Buffer>(m_storage);
		}

		ML_NODISCARD constexpr auto bufferAttachment() const noexcept
		{
			return buffer_data().first;
		}

		ML_NODISCARD constexpr auto bufferHandle() const noexcept
		{
			return buffer_data().second;
		}

		ML_NODISCARD constexpr auto texture_data() const noexcept
		{
			return std::get<ID_Texture>(m_storage);
		}

		ML_NODISCARD constexpr auto textureAttachment() const noexcept
		{
			return std::get<0>(texture_data());
		}

		ML_NODISCARD constexpr auto textureHandle() const noexcept
		{
			return std::get<1>(texture_data());
		}

		ML_NODISCARD constexpr auto textureLevel() const noexcept
		{
			return std::get<2>(texture_data());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Render Buffer Object
	struct ML_RENDERER_API render_buffer_object final : graphics_buffer<render_buffer_object>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Size, ID_Format };

		using storage_type = typename std::tuple<
			vec2i, uint32_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit render_buffer_object(handle_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr render_buffer_object(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr render_buffer_object(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr render_buffer_object()
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
				util::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(render_buffer_object const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_buffer_object & generate(vec2i const & size);

		render_buffer_object & destroy();

		render_buffer_object & update(uint32_t format);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto size() const noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		ML_NODISCARD constexpr auto format() const noexcept
		{
			return std::get<ID_Format>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING VAO = typename vertex_array_object;
	ML_USING VBO = typename vertex_buffer_object;
	ML_USING IBO = typename index_buffer_object;
	ML_USING FBO = typename frame_buffer_object;
	ML_USING RBO = typename render_buffer_object;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vao(Args && ... args)
	{
		return VAO{ std::forward<Args>(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vbo(Args && ... args)
	{
		return VBO{ std::forward<Args>(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_ibo(Args && ... args)
	{
		return IBO{ std::forward<Args>(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_fbo(Args && ... args)
	{
		return FBO{ std::forward<Args>(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_rbo(Args && ... args)
	{
		return RBO{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFERS_HPP_