#ifndef _ML_BUFFERS_HPP_
#define _ML_BUFFERS_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	template <class T> struct GraphicsBuffer
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename T;

		using handle_t = typename uint32_t;

		using buffer_t = typename void *;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit GraphicsBuffer(handle_t handle) noexcept
			: m_handle{ handle }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const noexcept { return m_handle; }

		constexpr auto address() const noexcept -> void * { return ML_ADDRESSOF(m_handle); }

		constexpr auto handle() noexcept -> handle_t & { return m_handle; }

		constexpr auto handle() const noexcept -> handle_t const & { return m_handle; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		handle_t m_handle;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	struct ML_RENDERER_API VAO final : public GraphicsBuffer<VAO>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit VAO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr VAO(self_type const & other) noexcept
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr VAO() noexcept
			: self_type{ NULL, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(self_type const * value);
		
		static self_type create(uint32_t mode);
		
		static bool destroy(self_type const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) mode() const noexcept { return std::get<0>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	struct ML_RENDERER_API VBO final : public GraphicsBuffer<VBO>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			uint32_t, buffer_t, uint32_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit VBO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr VBO(VBO const & other) noexcept
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr VBO() noexcept
			: self_type{ NULL, 0, nullptr, 0, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(self_type const * value);
		
		static self_type create(uint32_t usage, buffer_t data, uint32_t size, uint32_t count);
		
		static bool destroy(self_type const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) usage() const noexcept { return std::get<0>(m_storage); }
		
		constexpr decltype(auto) data() const noexcept { return std::get<1>(m_storage); }
		
		constexpr decltype(auto) size() const noexcept { return std::get<2>(m_storage); }
		
		constexpr decltype(auto) count() const noexcept { return std::get<3>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	struct ML_RENDERER_API IBO final : public GraphicsBuffer<IBO>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			uint32_t, buffer_t, uint32_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit IBO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr IBO(IBO const & other) noexcept
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr IBO() noexcept
			: self_type{ NULL, 0, nullptr, 0, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(self_type const * value);
		
		static self_type create(uint32_t usage, buffer_t data, uint32_t type, uint32_t count);
		
		static bool destroy(self_type const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) usage() const noexcept { return std::get<0>(m_storage); }
		
		constexpr decltype(auto) data() const noexcept { return std::get<1>(m_storage); }
		
		constexpr decltype(auto) type() const noexcept { return std::get<2>(m_storage); }
		
		constexpr decltype(auto) count() const noexcept { return std::get<3>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	struct ML_RENDERER_API FBO final : public GraphicsBuffer<FBO>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			vec2
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit FBO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr FBO(FBO const & other) noexcept
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr FBO() noexcept
			: self_type{ NULL, vec2::zero() }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(FBO::self_type const * value);
		
		static self_type create(vec2 const & size);
		
		static bool destroy(self_type const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) size() const noexcept { return std::get<0>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	struct ML_RENDERER_API RBO final : public GraphicsBuffer<RBO>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			vec2, uint32_t
		>;


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		constexpr explicit RBO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		constexpr RBO(RBO const & other) noexcept
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		constexpr RBO() noexcept
			: self_type{ NULL, vec2::zero(), 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(self_type const * value);
		
		static self_type create(vec2 const & size, uint32_t storage);
		
		static bool destroy(self_type const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) size() const noexcept { return std::get<0>(m_storage); }

		constexpr decltype(auto) storage() const noexcept { return std::get<1>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BUFFERS_HPP_