#include <libmeme/Renderer/Mesh.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	mesh::mesh(allocator_type const & alloc) noexcept
		: m_vao		{}
		, m_vbo		{}
		, m_ibo		{}
		, m_layout	{}
		, m_vertices{ alloc }
		, m_indices	{ alloc }
	{
	}

	mesh::mesh(vertices_t const & vertices, allocator_type const & alloc)
		: mesh{ alloc }
	{
		load_from_memory(vertices);
	}

	mesh::mesh(vertices_t const & vertices, indices_t const & indices, allocator_type const & alloc)
		: mesh{ alloc }
	{
		load_from_memory(vertices, indices);
	}

	mesh::mesh(mesh const & other, allocator_type const & alloc)
		: mesh{ alloc }
	{
		load_from_memory(other);
	}

	mesh::mesh(mesh && other, allocator_type const & alloc) noexcept
		: mesh{ alloc }
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void mesh::destroy()
	{
		vao().destroy();
		vbo().destroy();
		ibo().destroy();
	}

	void mesh::swap(mesh & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_vao, other.m_vao);
			std::swap(m_vbo, other.m_vbo);
			std::swap(m_ibo, other.m_ibo);
			m_layout.swap(other.m_layout);
			m_vertices.swap(other.m_vertices);
			m_indices.swap(other.m_indices);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool mesh::load_from_memory(mesh const & other)
	{
		if (this == std::addressof(other))
		{
			return false;
		}
		else if (!other.m_vertices.empty())
		{
			if (!other.m_indices.empty())
			{
				return load_from_memory(other.m_vertices, other.m_indices);
			}
			else
			{
				return load_from_memory(other.m_vertices);
			}
		}
		else
		{
			return false;
		}
	}

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
			return debug::log::error("mesh is already created");
		}

		// no vertices
		if (vertices.empty())
		{
			return debug::log::error("no verticies provided to mesh");
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
			return debug::log::error("mesh failed creating VAO");
		}

		// create vbo
		if (!m_vbo.generate(GL::StaticDraw))
		{
			destroy();
			return debug::log::error("mesh failed creating VBO");
		}

		// create ibo
		if (!m_ibo.generate(GL::StaticDraw, GL::UnsignedInt))
		{
			destroy();
			return debug::log::error("mesh failed creating IBO");
		}
		
		// bind buffers
		ML_bind_scope(m_vao);
		ML_bind_scope(m_vbo);
		ML_bind_scope(m_ibo);
		
		// update buffers
		m_vbo.update((void *)vertices.data(), (uint32_t)vertices.size());
		m_ibo.update((void *)indices.data(), (uint32_t)indices.size());
		
		// apply layout
		layout().apply();
		
		// save geometry
		m_vertices = vertices;
		m_indices = indices;
		return true;
	}

	bool mesh::load_from_memory(contiguous_t const & vertices)
	{
		// already created
		if (good())
		{
			return debug::log::error("mesh is already created");
		}

		// no vertices
		if (vertices.empty())
		{
			return debug::log::error("no vertices provided to mesh");
		}

		// create vao
		if (!m_vao.generate(GL::Triangles))
		{
			destroy();
			return debug::log::error("mesh failed creating VAO");
		}

		// create vbo
		if (!m_vbo.generate(GL::StaticDraw))
		{
			destroy();
			return debug::log::error("mesh failed creating VBO");
		}

		// bind buffers
		ML_bind_scope(m_vao);
		ML_bind_scope(m_vbo);

		// update buffers
		m_vbo.update((void *)vertices.data(), (uint32_t)vertices.size());
		
		// apply layout
		layout().apply();

		// save geometry
		m_vertices = vertices;
		m_indices = {};
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void mesh::draw(render_target & target, mesh const * value)
	{
		if (!value || !value->m_vao || !value->m_vbo) { return; }

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