#ifndef _ML_BUFFERS_HPP_
#define _ML_BUFFERS_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	// Base Graphics Object
	template <class Derived, class ... Args
	> struct graphics_buffer
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type	= typename Derived;
		using storage_type = typename std::tuple<Args...>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit graphics_buffer(uint32_t handle) noexcept
			: m_handle{ handle }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator bool() const noexcept { return m_handle; }

		ML_NODISCARD inline auto address() const noexcept -> void * { return ML_ADDRESSOF(m_handle); }

		ML_NODISCARD constexpr auto handle() const noexcept -> uint32_t const & { return m_handle; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		uint32_t m_handle;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Vertex Array Object
	struct ML_RENDERER_API vertex_array final : graphics_buffer<vertex_array,
		uint32_t
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Mode };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit vertex_array(uint32_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ ML_FWD(args)... }
		{
		}

		constexpr vertex_array(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr vertex_array(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr vertex_array()
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
		
		static void bind(vertex_array const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		vertex_array & generate(uint32_t mode);

		vertex_array & destroy();

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
	struct ML_RENDERER_API vertex_buffer final : graphics_buffer<vertex_buffer,
		uint32_t, void const *, uint32_t, uint32_t, uint32_t
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Usage, ID_Data, ID_Size, ID_Count, ID_Offset };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit vertex_buffer(uint32_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ ML_FWD(args)... }
		{
		}

		constexpr vertex_buffer(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr vertex_buffer(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr vertex_buffer()
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
		
		static void bind(vertex_buffer const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		vertex_buffer & generate(uint32_t usage);

		vertex_buffer & destroy();

		vertex_buffer & update(void const * data, uint32_t size);

		vertex_buffer & update(void const * data, uint32_t size, uint32_t offset);

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
	struct ML_RENDERER_API index_buffer final : graphics_buffer<index_buffer,
		uint32_t, uint32_t, void const *, uint32_t
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Usage, ID_Type, ID_Data, ID_Count };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit index_buffer(uint32_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ ML_FWD(args)... }
		{
		}

		constexpr index_buffer(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr index_buffer(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr index_buffer()
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
		
		static void bind(index_buffer const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		index_buffer & generate(uint32_t usage, uint32_t type);

		index_buffer & destroy();

		index_buffer & update(void const * data, uint32_t count);

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
	struct ML_RENDERER_API frame_buffer final : graphics_buffer<frame_buffer,
		vec2,		// size
		uint32_t,	// buffer attachment
		uint32_t,	// buffer handle
		uint32_t,	// texture attachment
		uint32_t,	// texture handle
		uint32_t	// texture level
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t {
			ID_Size,
			ID_BufferAttachment,
			ID_BufferHandle,
			ID_TextureAttachment,
			ID_TextureHandle,
			ID_TextureLevel
		};
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit frame_buffer(uint32_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ ML_FWD(args)... }
		{
		}

		constexpr frame_buffer(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr frame_buffer(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr frame_buffer()
			: self_type{ NULL, vec2{ 0 }, 0, 0, 0, 0, 0 }
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
		
		static void bind(frame_buffer const * value);
		
		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		frame_buffer & generate(vec2 const & size);

		frame_buffer & destroy();

		frame_buffer & attach_buffer(uint32_t attachment, uint32_t buf);

		frame_buffer & attach_texture(uint32_t attachment, uint32_t tex, uint32_t level);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto size() const noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		ML_NODISCARD constexpr auto buffer_attachment() const noexcept
		{
			return std::get<ID_BufferAttachment>(m_storage);
		}

		ML_NODISCARD constexpr auto buffer_handle() const noexcept
		{
			return std::get<ID_BufferHandle>(m_storage);
		}

		ML_NODISCARD constexpr auto texture_attachment() const noexcept
		{
			return std::get<ID_TextureAttachment>(m_storage);
		}

		ML_NODISCARD constexpr auto texture_handle() const noexcept
		{
			return std::get<ID_TextureHandle>(m_storage);
		}

		ML_NODISCARD constexpr auto texture_level() const noexcept
		{
			return std::get<ID_TextureLevel>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Render Buffer Object
	struct ML_RENDERER_API render_buffer final : graphics_buffer<render_buffer,
		vec2i, uint32_t, uint32_t
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Size, ID_Format };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr explicit render_buffer(uint32_t handle, Args && ... args)
			: graphics_buffer{ handle }
			, m_storage{ ML_FWD(args)... }
		{
		}

		constexpr render_buffer(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr render_buffer(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		constexpr render_buffer()
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

		static void bind(render_buffer const * value);

		ML_NODISCARD inline void bind() const { bind(this); }

		ML_NODISCARD inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_buffer & generate(vec2i const & size);

		render_buffer & destroy();

		render_buffer & update(uint32_t format);

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

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vao(Args && ... args)
	{
		return vertex_array{ ML_FWD(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vbo(Args && ... args)
	{
		return vertex_buffer{ ML_FWD(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_ibo(Args && ... args)
	{
		return index_buffer{ ML_FWD(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_fbo(Args && ... args)
	{
		return frame_buffer{ ML_FWD(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_rbo(Args && ... args)
	{
		return render_buffer{ ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFERS_HPP_