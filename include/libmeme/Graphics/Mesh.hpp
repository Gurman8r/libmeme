#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>
#include <libmeme/Graphics/ModelLoader.hpp>

namespace ml
{
	struct mesh final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using vertices_t	= typename pmr::vector<vertex>;
		using contiguous_t	= typename pmr::vector<float_t>;
		using indices_t		= typename pmr::vector<uint32_t>;

		static constexpr gfx::buffer_element default_layout[] =
		{
			{ vec3{}, "a_position"	},
			{ vec3{}, "a_normal"	},
			{ vec2{}, "a_texcoord"	},
		};

		mesh(uint32_t mode = gfx::primitive_triangles) noexcept
			: m_vao{ gfx::vertexarray::create(mode) }
		{
		}

		mesh(contiguous_t const & verts, gfx::buffer_layout const & layout = default_layout)
			: mesh{}
		{
			m_vao->set_layout(layout);
			add_vertices(verts);
		}

		mesh(contiguous_t const & verts, indices_t const & inds, gfx::buffer_layout const & layout = default_layout)
			: mesh{}
		{
			m_vao->set_layout(layout);
			add_vertices(verts);
			set_indices(inds);
		}

		mesh(vertices_t const & verts, gfx::buffer_layout const & layout = default_layout)
			: mesh{ util::contiguous(verts), layout }
		{
		}

		mesh(vertices_t const & verts, indices_t const & inds, gfx::buffer_layout const & layout = default_layout)
			: mesh{ util::contiguous(verts), inds, layout }
		{
		}

		mesh(fs::path const & path, gfx::buffer_layout const & layout = default_layout) noexcept
			: mesh{ model_loader::read(path), {}, layout }
		{
		}

		mesh(mesh && other) noexcept : m_vao{}
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		void add_vertices(shared<gfx::vertexbuffer> const & value) noexcept
		{
			m_vao->add_vertices(value);
		}

		void add_vertices(contiguous_t const & verts) noexcept
		{
			m_vao->add_vertices(gfx::vertexbuffer::create
			(
				verts.size(), verts.data()
			));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_layout(gfx::buffer_layout const & value) noexcept
		{
			m_vao->set_layout(value);
		}

		void set_indices(shared<gfx::indexbuffer> const & value) noexcept
		{
			m_vao->set_indices(value);
		}

		void set_indices(indices_t const & inds) noexcept
		{
			m_vao->set_indices(inds.empty() ? nullptr : gfx::indexbuffer::create
			(
				inds.size(), inds.data()
			));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared<gfx::vertexarray> const & get_vao() const & noexcept { return m_vao; }

		gfx::buffer_layout const & get_layout() const & noexcept { return m_vao->get_layout(); }

		shared<gfx::indexbuffer> const & get_indices() const & noexcept { return m_vao->get_indices(); }

		pmr::vector<shared<gfx::vertexbuffer>> const & get_vertices() const & noexcept { return m_vao->get_vertices(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shared<gfx::vertexarray> m_vao;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MESH_HPP_