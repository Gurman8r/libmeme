#include <libmeme/Renderer/Mesh.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	mesh::mesh() noexcept
		: m_layout{ make_buffer_layout() }
		, m_vao{ make_vao() }
		, m_vbo{ make_vbo() }
		, m_ibo{ make_ibo() }
	{
	}

	mesh::mesh(vertices_t const & vertices)
		: mesh{}
	{
		load_from_memory(vertices);
	}

	mesh::mesh(vertices_t const & vertices, indices_t const & indices)
		: mesh{}
	{
		load_from_memory(vertices, indices);
	}

	mesh::mesh(mesh const & other)
		: m_layout{ other.m_layout }
		, m_vao{ other.m_vao }
		, m_vbo{ other.m_vbo }
		, m_ibo{ other.m_ibo }
	{
	}

	mesh::mesh(mesh && other) noexcept
		: mesh{}
	{
		swap(std::move(other));
	}

	mesh::~mesh()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	mesh & mesh::operator=(mesh const & other)
	{
		mesh temp{ other };
		swap(temp);
		return (*this);
	}

	mesh & mesh::operator=(mesh && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void mesh::swap(mesh & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_layout.swap(other.m_layout);
			std::swap(m_vao, other.m_vao);
			std::swap(m_vbo, other.m_vbo);
			std::swap(m_ibo, other.m_ibo);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void mesh::destroy()
	{
		vao().destroy();
		vbo().destroy();
		ibo().destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool mesh::load_from_memory(vertices_t const & vertices, indices_t const & indices)
	{
		return load_from_memory(util::contiguous(vertices), indices);
	}

	bool mesh::load_from_memory(vertices_t const & vertices)
	{
		return load_from_memory(util::contiguous(vertices));
	}

	bool mesh::load_from_memory(contiguous_t const & vertices, indices_t const & indices)
	{
		// already created
		if (good())
		{
			return debug::log_error("mesh is already created");
		}

		// no vertices
		if (vertices.empty())
		{
			return debug::log_error("no verticies provided to mesh");
		}

		// no indices
		if (indices.empty())
		{
			return load_from_memory(vertices);
		}

		// create vao
		if (!m_vao.generate(GL::Triangles))
		{
			destroy();
			return debug::log_error("mesh failed creating VAO");
		}

		// create vbo
		if (!m_vbo.generate(GL::StaticDraw))
		{
			destroy();
			return debug::log_error("mesh failed creating VBO");
		}

		// create ibo
		if (!m_ibo.generate(GL::StaticDraw, GL::UnsignedInt))
		{
			destroy();
			return debug::log_error("mesh failed creating IBO");
		}
		
		// bind buffers
		ML_BIND_SCOPE(m_vao);
		ML_BIND_SCOPE(m_vbo);
		ML_BIND_SCOPE(m_ibo);
		
		// update buffers
		m_vbo.update((void *)vertices.data(), (uint32_t)vertices.size());
		m_ibo.update((void *)indices.data(), (uint32_t)indices.size());
		
		// apply layout
		layout().apply();
		
		return true;
	}

	bool mesh::load_from_memory(contiguous_t const & vertices)
	{
		// already created
		if (good())
		{
			return debug::log_error("mesh is already created");
		}

		// no vertices
		if (vertices.empty())
		{
			return debug::log_error("no vertices provided to mesh");
		}

		// create vao
		if (!m_vao.generate(GL::Triangles))
		{
			destroy();
			return debug::log_error("mesh failed creating VAO");
		}

		// create vbo
		if (!m_vbo.generate(GL::StaticDraw))
		{
			destroy();
			return debug::log_error("mesh failed creating VBO");
		}

		// bind buffers
		ML_BIND_SCOPE(m_vao);
		ML_BIND_SCOPE(m_vbo);

		// update buffers
		m_vbo.update((void *)vertices.data(), (uint32_t)vertices.size());
		
		// apply layout
		layout().apply();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void mesh::draw(render_target const & target, mesh const * value)
	{
		if (!value || !value->m_vao || !value->m_vbo)
			return;

		if (value->m_ibo)
		{
			target.draw(value->m_vao, value->m_vbo, value->m_ibo);
		}
		else
		{
			target.draw(value->m_vao, value->m_vbo);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}