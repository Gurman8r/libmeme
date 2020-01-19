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

		enum : size_t { ID_Layout, ID_VAO, ID_VBO, ID_IBO };

		using storage_type	= typename std::tuple<buffer_layout, VAO, VBO, IBO>;
		using contiguous_t	= typename pmr::vector<float_t>;
		using vertices_t	= typename pmr::vector<vertex>;
		using indices_t		= typename pmr::vector<uint32_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh();
		
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

		ML_NODISCARD inline decltype(auto) layout() noexcept
		{
			return std::get<ID_Layout>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) layout() const noexcept
		{
			return std::get<ID_Layout>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) vao() noexcept
		{
			return std::get<ID_VAO>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) vao() const noexcept
		{
			return std::get<ID_VAO>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) vbo() noexcept
		{
			return std::get<ID_VBO>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) vbo() const noexcept
		{
			return std::get<ID_VBO>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) ibo() noexcept
		{
			return std::get<ID_IBO>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) ibo() const noexcept
		{
			return std::get<ID_IBO>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool good() const noexcept
		{
			return (ibo() && vao() && vbo()) || (vao() && vbo());
		}

		inline operator bool() const noexcept
		{
			return good();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

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