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

		enum : size_t { ID_Layout, ID_VAO, ID_VBO, ID_IBO };

		using storage_type	= typename std::tuple<BufferLayout, VAO, VBO, IBO>;
		using contiguous_t	= typename std::vector<float_t>;
		using vertices_t	= typename std::vector<Vertex>;
		using indices_t		= typename std::vector<uint32_t>;

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

		bool load_from_memory(vertices_t const & vertices, indices_t const & indices);
		
		bool load_from_memory(vertices_t const & vertices);
		
		bool load_from_memory(contiguous_t const & vertices, indices_t const & indices);

		bool load_from_memory(contiguous_t const & vertices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(RenderTarget const & target, Mesh const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) layout() noexcept { return std::get<ID_Layout>(m_storage); }
		
		ML_NODISCARD inline decltype(auto) layout() const noexcept { return std::get<ID_Layout>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ML_NODISCARD inline decltype(auto) vao() noexcept { return std::get<ID_VAO>(m_storage); }
		
		ML_NODISCARD inline decltype(auto) vao() const noexcept { return std::get<ID_VAO>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) vbo() noexcept { return std::get<ID_VBO>(m_storage); }
		
		ML_NODISCARD inline decltype(auto) vbo() const noexcept { return std::get<ID_VBO>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) ibo() noexcept { return std::get<ID_IBO>(m_storage); }
		
		ML_NODISCARD inline decltype(auto) ibo() const noexcept { return std::get<ID_IBO>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept
		{
			return (vao() && vbo()) || (vao() && vbo() && ibo());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_mesh(Mesh::vertices_t && v)
	{
		return Mesh{ std::move(v) };
	}

	ML_NODISCARD static inline auto make_mesh(Mesh::vertices_t && v, Mesh::indices_t && i)
	{
		return Mesh{ std::move(v), std::move(i) };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_mesh(Args && ... args)
	{
		return Mesh{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_