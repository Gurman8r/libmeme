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

		mesh(shared<vertexarray> const & vao = vertexarray::create()) noexcept
			: m_vao{ vao }, m_verts{}, m_inds{}
		{
		}

		mesh(contiguous_t const & vertices, indices_t const & indices = {}, buffer_layout const & layout = {})
			: m_vao{ vertexarray::create() }, m_verts{ vertices }, m_inds{ indices }
		{
			auto vb = vertexbuffer::create(m_verts);

			vb->set_layout(layout);

			m_vao->add_vbo(vb);

			m_vao->set_ibo(!m_inds.empty() ? indexbuffer::create(m_inds) : nullptr);
		}

		mesh(vertices_t const & vertices, indices_t const & indices = {})
			: mesh{ util::contiguous(vertices), indices }
		{
		}

		mesh(fs::path const & path) noexcept
			: mesh{ model_loader::read(path), {} }
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

		auto vao() & noexcept -> shared<vertexarray> & { return m_vao; }

		auto vao() const & noexcept -> shared<vertexarray> const & { return m_vao; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shared<vertexarray>	m_vao;
		contiguous_t		m_verts;
		indices_t			m_inds;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MESH_HPP_