#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/BufferLayout.hpp>
#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Vertex.hpp>

namespace ml
{
	struct render_target;

	struct ML_RENDERER_API mesh final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using contiguous_t		= typename pmr::vector<float_t>;
		using vertices_t		= typename pmr::vector<vertex>;
		using indices_t			= typename pmr::vector<uint32_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh(allocator_type const & alloc = {}) noexcept;
		
		mesh(vertices_t const & vertices, allocator_type const & alloc = {});
		
		mesh(vertices_t const & vertices, indices_t const & indices, allocator_type const & alloc = {});
		
		mesh(mesh const & other, allocator_type const & alloc = {});
		
		mesh(mesh && other, allocator_type const & alloc = {}) noexcept;
		
		~mesh();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh & operator=(mesh const & other);
		
		mesh & operator=(mesh && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void destroy();

		void swap(mesh & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_memory(mesh const & other);

		bool load_from_memory(vertices_t const & vertices, indices_t const & indices);
		
		bool load_from_memory(vertices_t const & vertices);
		
		bool load_from_memory(contiguous_t const & vertices, indices_t const & indices);

		bool load_from_memory(contiguous_t const & vertices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(render_target const & target, mesh const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool good() const noexcept { return (m_ibo && m_vao && m_vbo) || (m_vao && m_vbo); }

		inline operator bool() const noexcept { return good(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto vao() noexcept -> VAO & { return m_vao; }

		ML_NODISCARD inline auto vao() const noexcept -> VAO const & { return m_vao; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto vbo() noexcept -> VBO & { return m_vbo; }

		ML_NODISCARD inline auto vbo() const noexcept -> VBO const & { return m_vbo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto ibo() noexcept -> IBO & { return m_ibo; }

		ML_NODISCARD inline auto ibo() const noexcept -> IBO const & { return m_ibo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto layout() noexcept -> buffer_layout & { return m_layout; }

		ML_NODISCARD inline auto layout() const noexcept -> buffer_layout const & { return m_layout; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto vertices() noexcept -> contiguous_t & { return m_vertices; }

		ML_NODISCARD inline auto vertices() const noexcept -> contiguous_t const & { return m_vertices; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto indices() noexcept -> indices_t & { return m_indices; }

		ML_NODISCARD inline auto indices() const noexcept -> indices_t const & { return m_indices; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		VAO				m_vao;
		VBO				m_vbo;
		IBO				m_ibo;
		buffer_layout	m_layout;
		contiguous_t	m_vertices;
		indices_t		m_indices;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_mesh(mesh::vertices_t && v)
	{
		return mesh{ ML_FWD(v) };
	}

	ML_NODISCARD static inline auto make_mesh(mesh::vertices_t && v, mesh::indices_t && i)
	{
		return mesh{ ML_FWD(v), ML_FWD(i) };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_mesh(Args && ... args)
	{
		return mesh{ ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_