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

		mesh(shared<gl::vertexarray> const & vao = nullptr, contiguous_t const & verts = {}, indices_t const & inds = {}) noexcept
			: m_vao{ vao }, m_verts{ verts }, m_inds{ inds }
		{
		}

		mesh(contiguous_t const & verts, indices_t const & inds = {}, gl::buffer_layout const & layout = {})
			: mesh{ gl::vertexarray::allocate(), verts, inds }
		{
			// vertices
			if (!m_verts.empty())
			{
				auto vb = gl::vertexbuffer::allocate
				(
					m_verts.data(), (uint32_t)m_verts.size()
				);
				vb->set_layout(layout);
				m_vao->add_vb(vb);
			}

			// indices
			m_vao->set_ib(m_inds.empty() ? nullptr : gl::indexbuffer::allocate
			(
				m_inds.data(), (uint32_t)m_inds.size()
			));
		}

		mesh(vertices_t const & verts, indices_t const & inds = {}, gl::buffer_layout const & layout = {})
			: mesh{ util::contiguous(verts), inds, layout }
		{
		}

		mesh(fs::path const & path, gl::buffer_layout const & layout = {}) noexcept
			: mesh{ model_loader::read(path), {}, layout }
		{
		}

		mesh(mesh const & other)
			: mesh{ other.m_verts, other.m_inds }
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
				m_verts.swap(other.m_verts);
				m_inds.swap(other.m_inds);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto vao() & noexcept -> shared<gl::vertexarray> & { return m_vao; }

		auto vao() const & noexcept -> shared<gl::vertexarray> const & { return m_vao; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shared<gl::vertexarray>	m_vao;
		contiguous_t		m_verts;
		indices_t			m_inds;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MESH_HPP_