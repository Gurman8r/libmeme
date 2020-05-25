#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>
#include <libmeme/Graphics/ModelLoader.hpp>

namespace ml
{
	struct mesh final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		mesh(shared<gl::vertex_array> const & vao = gl::make_vao()) noexcept
			: m_vao{ vao }, m_verts{}, m_inds{}
		{
		}

		mesh(pmr::vector<float_t> const & vertices, pmr::vector<uint32_t> const & indices = {})
			: m_vao{ gl::make_vao() }, m_verts{ vertices }, m_inds{ indices }
		{
			auto vb = gl::make_vbo(m_verts);

			vb->set_layout({
				{ meta::tag_v<vec3f>, "a_position"	},
				{ meta::tag_v<vec3f>, "a_normal"	},
				{ meta::tag_v<vec2f>, "a_texcoord"	},
			});

			m_vao->add_vbo(vb);

			m_vao->set_ibo(!m_inds.empty() ? gl::make_ibo(m_inds) : nullptr);
		}

		mesh(pmr::vector<vertex> const & vertices, pmr::vector<uint32_t> const & indices = {})
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

		auto vao() & noexcept -> shared<gl::vertex_array> & { return m_vao; }

		auto vao() const & noexcept -> shared<gl::vertex_array> const & { return m_vao; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shared<gl::vertex_array> m_vao;
		pmr::vector<uint32_t> m_inds{};
		pmr::vector<float_t> m_verts{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MESH_HPP_