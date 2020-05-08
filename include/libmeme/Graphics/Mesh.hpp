#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Graphics/Buffers.hpp>
#include <libmeme/Graphics/BufferLayout.hpp>
#include <libmeme/Graphics/Material.hpp>
#include <libmeme/Graphics/Vertex.hpp>

namespace ml
{
	struct ML_GRAPHICS_API mesh final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using contiguous_t		= typename pmr::vector<float_t>;
		using vertices_t		= typename pmr::vector<vertex>;
		using indices_t			= typename pmr::vector<uint32_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh(allocator_type alloc = {}) noexcept;
		
		mesh(vertices_t const & vertices, allocator_type alloc = {});
		
		mesh(vertices_t const & vertices, indices_t const & indices, allocator_type alloc = {});
		
		mesh(mesh const & value, allocator_type alloc = {});
		
		mesh(mesh && value, allocator_type alloc = {}) noexcept;
		
		~mesh();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh & operator=(mesh const & value);
		
		mesh & operator=(mesh && value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void destroy();

		void swap(mesh & value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_memory(mesh const & value);

		bool load_from_memory(vertices_t const & vertices, indices_t const & indices);
		
		bool load_from_memory(vertices_t const & vertices);
		
		bool load_from_memory(contiguous_t const & vertices, indices_t const & indices);

		bool load_from_memory(contiguous_t const & vertices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(struct render_target & target, mesh const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool good() const noexcept { return (m_ibo && m_vao && m_vbo) || (m_vao && m_vbo); }

		operator bool() const noexcept { return good(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto vao() noexcept -> VAO & { return m_vao; }

		ML_NODISCARD auto vao() const noexcept -> VAO const & { return m_vao; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto vbo() noexcept -> VBO & { return m_vbo; }

		ML_NODISCARD auto vbo() const noexcept -> VBO const & { return m_vbo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto ibo() noexcept -> IBO & { return m_ibo; }

		ML_NODISCARD auto ibo() const noexcept -> IBO const & { return m_ibo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto layout() noexcept -> buffer_layout & { return m_layout; }

		ML_NODISCARD auto layout() const noexcept -> buffer_layout const & { return m_layout; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto vertices() noexcept -> contiguous_t & { return m_vertices; }

		ML_NODISCARD auto vertices() const noexcept -> contiguous_t const & { return m_vertices; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto indices() noexcept -> indices_t & { return m_indices; }

		ML_NODISCARD auto indices() const noexcept -> indices_t const & { return m_indices; }

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
		return mesh{ ML_forward(v) };
	}

	ML_NODISCARD static inline auto make_mesh(mesh::vertices_t && v, mesh::indices_t && i)
	{
		return mesh{ ML_forward(v), ML_forward(i) };
	}

	template <class ... Args
	> ML_NODISCARD inline auto make_mesh(Args && ... args)
	{
		return mesh{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_