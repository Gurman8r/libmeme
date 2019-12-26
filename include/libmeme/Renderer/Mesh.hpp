#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/BufferLayout.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_RENDERER_API Mesh final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<
			BufferLayout, VAO, VBO, IBO
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh();
		Mesh(path_t const & path);
		Mesh(Mesh const & other);
		Mesh(Mesh && other) noexcept;
		~Mesh();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh & operator=(Mesh const & other);
		Mesh & operator=(Mesh && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void assign(Mesh const & other);
		void swap(Mesh & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline decltype(auto) layout() noexcept { return std::get<0>(m_storage); }
		inline decltype(auto) layout() const noexcept { return std::get<0>(m_storage); }
		
		inline decltype(auto) vao() noexcept { return std::get<1>(m_storage); }
		inline decltype(auto) vao() const noexcept { return std::get<1>(m_storage); }
		
		inline decltype(auto) vbo() noexcept { return std::get<2>(m_storage); }
		inline decltype(auto) vbo() const noexcept { return std::get<2>(m_storage); }
		
		inline decltype(auto) ibo() noexcept { return std::get<3>(m_storage); }
		inline decltype(auto) ibo() const noexcept { return std::get<3>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MESH_HPP_