#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/BufferLayout.hpp>
#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Vertex.hpp>

namespace ml
{
	struct RenderTarget;

	struct ML_RENDERER_API Mesh final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Layout, VAO, VBO, IBO };

		using storage_t = typename std::tuple<
			BufferLayout,
			VertexArrayObject,
			VertexBufferObject,
			IndexBufferObject
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

		void swap(Mesh & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromMemory(vertices_t const & vertices, indices_t const & indices);
		
		bool loadFromMemory(vertices_t const & vertices);
		
		bool loadFromMemory(contiguous_t const & vertices, indices_t const & indices);

		bool loadFromMemory(contiguous_t const & vertices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(RenderTarget const & target, Mesh const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto layout() noexcept		-> BufferLayout & { return std::get<Layout>(m_storage); }
		inline auto layout() const noexcept -> BufferLayout const & { return std::get<Layout>(m_storage); }

		inline auto vao() noexcept			-> VertexArrayObject & { return std::get<VAO>(m_storage); }
		inline auto vao() const noexcept	-> VertexArrayObject const & { return std::get<VAO>(m_storage); }

		inline auto vbo() noexcept			-> VertexBufferObject & { return std::get<VBO>(m_storage); }
		inline auto vbo() const noexcept	-> VertexBufferObject const & { return std::get<VBO>(m_storage); }

		inline auto ibo() noexcept			-> IndexBufferObject & { return std::get<IBO>(m_storage); }
		inline auto ibo() const noexcept	-> IndexBufferObject const & { return std::get<IBO>(m_storage); }

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

	static inline auto make_mesh(Mesh::vertices_t && v)
	{
		return Mesh{ std::move(v) };
	}

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