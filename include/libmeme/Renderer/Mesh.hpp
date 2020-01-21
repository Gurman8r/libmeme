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

		using contiguous_t	= typename pmr::vector<float_t>;
		using vertices_t	= typename pmr::vector<vertex>;
		using indices_t		= typename pmr::vector<uint32_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh() noexcept;
		
		mesh(vertices_t const & vertices);
		
		mesh(vertices_t const & vertices, indices_t const & indices);
		
		mesh(mesh const & other);
		
		mesh(mesh && other) noexcept;
		
		~mesh();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh & operator=(mesh const & other);
		
		mesh & operator=(mesh && other) noexcept;

		void swap(mesh & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_memory(vertices_t const & vertices, indices_t const & indices);
		
		bool load_from_memory(vertices_t const & vertices);
		
		bool load_from_memory(contiguous_t const & vertices, indices_t const & indices);

		bool load_from_memory(contiguous_t const & vertices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(render_target const & target, mesh const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto layout() noexcept -> buffer_layout & { return m_layout; }

		ML_NODISCARD inline auto layout() const noexcept -> buffer_layout const & { return m_layout; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto vao() noexcept -> vertex_array & { return m_vao; }

		ML_NODISCARD inline auto vao() const noexcept -> vertex_array const & { return m_vao; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto vbo() noexcept -> vertex_buffer & { return m_vbo; }

		ML_NODISCARD inline auto vbo() const noexcept -> vertex_buffer const & { return m_vbo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto ibo() noexcept -> index_buffer & { return m_ibo; }

		ML_NODISCARD inline auto ibo() const noexcept -> index_buffer const & { return m_ibo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool good() const noexcept
		{
			return (m_ibo && m_vao && m_vbo) || (m_vao && m_vbo);
		}

		inline operator bool() const noexcept
		{
			return good();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		buffer_layout	m_layout;
		vertex_array	m_vao;
		vertex_buffer	m_vbo;
		index_buffer	m_ibo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_mesh(mesh::vertices_t && v)
	{
		return mesh{ std::move(v) };
	}

	ML_NODISCARD static inline auto make_mesh(mesh::vertices_t && v, mesh::indices_t && i)
	{
		return mesh{ std::move(v), std::move(i) };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_mesh(Args && ... args)
	{
		return mesh{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_