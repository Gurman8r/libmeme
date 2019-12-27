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

		using address_t = typename void *;

		using buffer_t = typename void const *;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit GraphicsBuffer(handle_t handle) noexcept
			: m_handle{ handle }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const noexcept { return m_handle; }

		inline auto address() const noexcept -> address_t { return ML_ADDRESSOF(m_handle); }

		inline auto handle() noexcept -> handle_t & { return m_handle; }

		inline auto handle() const noexcept -> handle_t const & { return m_handle; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		handle_t m_handle{ NULL };

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
		explicit VAO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		VAO(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		VAO(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		VAO()
			: self_type{ NULL, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool create(VAO * value, uint32_t mode);

		static void bind(VAO const * value);
		
		static bool destroy(VAO * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VAO & create(uint32_t mode);

		void bind() const;

		bool destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline uint32_t mode() const noexcept { return std::get<0>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	struct ML_RENDERER_API VBO final : public GraphicsBuffer<VBO>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			uint32_t, buffer_t, uint32_t, uint32_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		explicit VBO(handle_t handle, Args && ... args)
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		VBO(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		VBO(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		VBO()
			: self_type{ NULL, 0, nullptr, 0, 0, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool create(VBO * value, uint32_t usage);

		static void bind(VBO const * value);
		
		static bool destroy(VBO * value);

		static void update(VBO * value, buffer_t data, uint32_t size);

		static void update(VBO * value, buffer_t data, uint32_t size, uint32_t offset);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		VBO & create(uint32_t usage);

		void bind() const;

		bool destroy();

		VBO & update(buffer_t data, uint32_t size);

		VBO & update(buffer_t data, uint32_t size, uint32_t offset);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline uint32_t usage() const noexcept { return std::get<0>(m_storage); }
		
		inline buffer_t data() const noexcept { return std::get<1>(m_storage); }
		
		inline uint32_t size() const noexcept { return std::get<2>(m_storage); }

		inline uint32_t count() const noexcept { return std::get<3>(m_storage); }

		inline uint32_t offset() const noexcept { return std::get<4>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	struct ML_RENDERER_API IBO final : public GraphicsBuffer<IBO>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			uint32_t, uint32_t, buffer_t, uint32_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		explicit IBO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		IBO(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		IBO(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		IBO()
			: self_type{ NULL, 0, 0, nullptr, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool create(IBO * value, uint32_t usage, uint32_t type);

		static void bind(IBO const * value);
		
		static bool destroy(IBO * value);

		static void update(IBO * value, buffer_t data, uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IBO & create(uint32_t usage, uint32_t type);

		void bind() const;

		bool destroy();

		IBO & update(buffer_t data, uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline uint32_t usage() const noexcept { return std::get<0>(m_storage); }

		inline uint32_t type() const noexcept { return std::get<1>(m_storage); }
		
		inline buffer_t data() const noexcept { return std::get<2>(m_storage); }
		
		inline uint32_t count() const noexcept { return std::get<3>(m_storage); }

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
		explicit FBO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		FBO(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		FBO(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		FBO()
			: self_type{ NULL, vec2{ 0 } }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool create(FBO * value, vec2 const & size);

		static void bind(FBO const * value);
		
		static bool destroy(FBO * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FBO & create(vec2 const & size);

		void bind() const;

		bool destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline vec2 size() const noexcept { return std::get<0>(m_storage); }

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
		explicit RBO(handle_t handle, Args && ... args) noexcept
			: GraphicsBuffer{ handle }
			, m_storage{ std::forward<Args>(args)... }
		{
		}

		RBO(self_type const & other)
			: self_type{ other.m_handle, other.m_storage }
		{
		}

		RBO(self_type && other) noexcept
			: self_type{}
		{
			swap(std::move(other));
		}

		RBO()
			: self_type{ NULL, vec2{ 0 }, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_handle, other.m_handle);

				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool create(RBO * value, vec2 const & size, uint32_t type);

		static void bind(RBO const * value);
		
		static bool destroy(RBO * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RBO & create(vec2 const & size, uint32_t type);

		void bind() const;

		bool destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline vec2 size() const noexcept { return std::get<0>(m_storage); }

		inline uint32_t type() const noexcept { return std::get<1>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BUFFERS_HPP_