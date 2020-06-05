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

		explicit mesh(shared<gl::vertexarray> const & vao = nullptr) noexcept
			: m_vao{ vao }
		{
		}

		mesh(contiguous_t const & verts, gl::layout const & layout = {})
			: mesh{ gl::make_vertexarray() }
		{
			add_vb(verts, layout);
		}

		mesh(contiguous_t const & verts, indices_t const & inds, gl::layout const & layout = {})
			: mesh{ gl::make_vertexarray() }
		{
			add_vb(verts, layout);
			set_ib(inds);
		}

		mesh(vertices_t const & verts, gl::layout const & layout = {})
			: mesh{ util::contiguous(verts), layout }
		{
		}

		mesh(vertices_t const & verts, indices_t const & inds, gl::layout const & layout = {})
			: mesh{ util::contiguous(verts), inds, layout }
		{
		}

		mesh(fs::path const & path, gl::layout const & layout = {}) noexcept
			: mesh{ model_loader::read(path), {}, layout }
		{
		}

		mesh(mesh const & other)
			: mesh{ other.m_vao }
		{
		}

		mesh(mesh && other) noexcept
			: mesh{}
		{
			this->swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh & operator=(mesh const & other)
		{
			mesh temp{ other };
			this->swap(temp);
			return (*this);
		}

		mesh & operator=(mesh && other) noexcept
		{
			this->swap(std::move(other));
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

		void add_vb(shared<gl::vertexbuffer> const & value)
		{
			m_vao->add_vb(value);
		}

		void add_vb(contiguous_t const & verts)
		{
			m_vao->add_vb(gl::make_vertexbuffer(verts.data(), verts.size()));
		}

		void add_vb(contiguous_t const & verts, gl::layout const & layout)
		{
			m_vao->add_vb(std::invoke([&, vb = gl::make_vertexbuffer(verts.data(), verts.size())
			]() noexcept
			{
				vb->set_layout(layout);
				return vb;
			}));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_ib(shared<gl::indexbuffer> const & value)
		{
			m_vao->set_ib(value);
		}

		void set_ib(indices_t const & inds)
		{
			m_vao->set_ib(inds.empty() ? nullptr : gl::make_indexbuffer
			(
				inds.data(), inds.size()
			));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gl::handle_t get_handle() const noexcept { return m_vao->get_handle(); }

		auto const & get_va() const & noexcept { return m_vao; }

		auto const & get_ib() const & noexcept { return m_vao->get_ib(); }

		auto const & get_vbs() const & noexcept { return m_vao->get_vbs(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shared<gl::vertexarray> m_vao;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MESH_HPP_