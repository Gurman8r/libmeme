#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>
#include <libmeme/Graphics/ModelLoader.hpp>

namespace ml
{
	struct mesh final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using vertices_t	= typename pmr::vector<vertex>;
		using contiguous_t	= typename pmr::vector<float_t>;
		using indices_t		= typename pmr::vector<uint32_t>;

		mesh(uint32_t mode = gl::primitive_triangles) noexcept
			: m_vao{ gl::vertexarray::allocate(mode) }
		{
		}

		mesh(contiguous_t const & verts, gl::buffer_layout const & buffer_layout = {})
			: mesh{}
		{
			add_vertices(verts, buffer_layout);
		}

		mesh(contiguous_t const & verts, indices_t const & inds, gl::buffer_layout const & buffer_layout = {})
			: mesh{}
		{
			add_vertices(verts, buffer_layout);
			set_indices(inds);
		}

		mesh(vertices_t const & verts, gl::buffer_layout const & buffer_layout = {})
			: mesh{ util::contiguous(verts), buffer_layout }
		{
		}

		mesh(vertices_t const & verts, indices_t const & inds, gl::buffer_layout const & buffer_layout = {})
			: mesh{ util::contiguous(verts), inds, buffer_layout }
		{
		}

		mesh(fs::path const & path, gl::buffer_layout const & buffer_layout = {}) noexcept
			: mesh{ model_loader::read(path), {}, buffer_layout }
		{
		}

		mesh(mesh const & other) : m_vao{ other.m_vao }
		{
		}

		mesh(mesh && other) noexcept : m_vao{}
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh & operator=(mesh const & other)
		{
			mesh temp{ other };
			swap(temp);
			return (*this);
		}

		mesh & operator=(mesh && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		void swap(mesh & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_vao.swap(other.m_vao);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void add_vertices(shared<gl::vertexbuffer> const & value) noexcept
		{
			m_vao->add_vertices(value);
		}

		void add_vertices(contiguous_t const & verts) noexcept
		{
			add_vertices(gl::vertexbuffer::allocate(verts.size(), verts.data()));
		}

		void add_vertices(contiguous_t const & verts, gl::buffer_layout const & buffer_layout) noexcept
		{
			add_vertices(std::invoke([&, vb = gl::vertexbuffer::allocate(verts.size(), verts.data())
			]() noexcept
			{
				vb->set_layout(buffer_layout);
				return vb;
			}));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_indices(shared<gl::indexbuffer> const & value) noexcept
		{
			m_vao->set_indices(value);
		}

		void set_indices(indices_t const & inds) noexcept
		{
			set_indices(inds.empty() ? nullptr : gl::indexbuffer::allocate
			(
				inds.size(), inds.data()
			));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto const & get_va() const & noexcept { return m_vao; }

		auto const & get_indices() const & noexcept { return m_vao->get_indices(); }

		auto const & get_vertices() const & noexcept { return m_vao->get_vertices(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shared<gl::vertexarray> m_vao;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MESH_HPP_