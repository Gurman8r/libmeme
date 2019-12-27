#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/BufferLayout.hpp>
#include <libmeme/Renderer/Drawable.hpp>
#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Vertex.hpp>

namespace ml
{
	struct ML_RENDERER_API Mesh final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			BufferLayout, VAO, VBO, IBO
		>;
		
		using contiguous_t = typename std::vector<float_t>;
		using vertices_t = typename std::vector<Vertex>;
		using indices_t = typename std::vector<uint32_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh();
		Mesh(vertices_t const & vertices);
		Mesh(vertices_t const & vertices, indices_t const & indices);
		Mesh(Mesh const & other);
		Mesh(Mesh && other) noexcept;
		~Mesh();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh & operator=(Mesh const & other);
		Mesh & operator=(Mesh && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void assign(Mesh const & other);
		void swap(Mesh & other) noexcept;
		void destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromMemory(vertices_t const & vertices, indices_t const & indices);
		bool loadFromMemory(vertices_t const & vertices);
		bool loadFromMemory(contiguous_t const & vertices, indices_t const & indices);
		bool loadFromMemory(contiguous_t const & vertices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(RenderTarget const & target, Mesh const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline BufferLayout & layout() noexcept { return std::get<0>(m_storage); }
		inline BufferLayout const & layout() const noexcept { return std::get<0>(m_storage); }
		inline BufferLayout & layout(BufferLayout const & value) { return (std::get<0>(m_storage) = value); }
		inline BufferLayout & layout(BufferLayout && value) noexcept { return (std::get<0>(m_storage) = std::move(value)); }

		inline VAO & vao() noexcept { return std::get<1>(m_storage); }
		inline VAO const & vao() const noexcept { return std::get<1>(m_storage); }
		inline VAO & vao(VAO const & value) { return (std::get<1>(m_storage) = value); }
		inline VAO & vao(VAO && value) noexcept { return (std::get<1>(m_storage) = std::move(value)); }

		inline VBO & vbo() noexcept { return std::get<2>(m_storage); }
		inline VBO const & vbo() const noexcept { return std::get<2>(m_storage); }
		inline VBO & vbo(VBO const & value) { return (std::get<2>(m_storage) = value); }
		inline VBO & vbo(VBO && value) noexcept { return (std::get<2>(m_storage) = std::move(value)); }

		inline IBO & ibo() noexcept { return std::get<3>(m_storage); }
		inline IBO const & ibo() const noexcept { return std::get<3>(m_storage); }
		inline IBO & ibo(IBO const & value) { return (std::get<3>(m_storage) = value); }
		inline IBO & ibo(IBO && value) noexcept { return (std::get<3>(m_storage) = std::move(value)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const noexcept
		{
			return (vao() && vbo()) || (vao() && vbo() && ibo());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline auto make_mesh(Mesh::vertices_t && v, Mesh::indices_t && i)
	{
		return Mesh{ std::move(v), std::move(i) };
	}

	template <class ... Args> static inline auto make_mesh(Args && ... args)
	{
		return Mesh{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_