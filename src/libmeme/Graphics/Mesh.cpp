#include <libmeme/Graphics/Mesh.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	mesh::mesh(allocator_type alloc) noexcept
		: m_vao		{}
		, m_vbo		{}
		, m_ibo		{}
		, m_layout	{}
		, m_vertices{ alloc }
		, m_indices	{ alloc }
	{
	}

	mesh::mesh(vertices_t const & vertices, allocator_type alloc)
		: mesh{ alloc }
	{
		load_from_memory(vertices);
	}

	mesh::mesh(vertices_t const & vertices, indices_t const & indices, allocator_type alloc)
		: mesh{ alloc }
	{
		load_from_memory(vertices, indices);
	}

	mesh::mesh(mesh const & value, allocator_type alloc)
		: mesh{ alloc }
	{
		load_from_memory(value);
	}

	mesh::mesh(mesh && value, allocator_type alloc) noexcept
		: mesh{ alloc }
	{
		swap(std::move(value));
	}

	mesh::~mesh()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	mesh & mesh::operator=(mesh const & value)
	{
		mesh temp{ value };
		swap(temp);
		return (*this);
	}

	mesh & mesh::operator=(mesh && value) noexcept
	{
		swap(std::move(value));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void mesh::destroy()
	{
		vao().destroy();
		vbo().destroy();
		ibo().destroy();
	}

	void mesh::swap(mesh & value) noexcept
	{
		if (this != std::addressof(value))
		{
			std::swap(m_vao, value.m_vao);
			std::swap(m_vbo, value.m_vbo);
			std::swap(m_ibo, value.m_ibo);
			m_layout.swap(value.m_layout);
			m_vertices.swap(value.m_vertices);
			m_indices.swap(value.m_indices);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool mesh::load_from_memory(mesh const & value)
	{
		if (this == std::addressof(value))
		{
			return false;
		}
		else if (!value.m_vertices.empty())
		{
			if (!value.m_indices.empty())
			{
				return load_from_memory(value.m_vertices, value.m_indices);
			}
			else
			{
				return load_from_memory(value.m_vertices);
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
			return debug::error("mesh is already created");
		}

		// no vertices
		if (vertices.empty())
		{
			return debug::error("no verticies provided to mesh");
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
			return debug::error("mesh failed creating VAO");
		}

		// create vbo
		if (!m_vbo.generate(GL::StaticDraw))
		{
			destroy();
			return debug::error("mesh failed creating VBO");
		}

		// create ibo
		if (!m_ibo.generate(GL::StaticDraw, GL::UnsignedInt))
		{
			destroy();
			return debug::error("mesh failed creating IBO");
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
			return debug::error("mesh is already created");
		}

		// no vertices
		if (vertices.empty())
		{
			return debug::error("no vertices provided to mesh");
		}

		// create vao
		if (!m_vao.generate(GL::Triangles))
		{
			destroy();
			return debug::error("mesh failed creating VAO");
		}

		// create vbo
		if (!m_vbo.generate(GL::StaticDraw))
		{
			destroy();
			return debug::error("mesh failed creating VBO");
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

	void mesh::draw(mesh const * value)
	{
		if (!value || !value->m_vao || !value->m_vbo)
		{
			return;
		}
		else if (value->m_ibo)
		{
			ML_bind_scope(value->m_vao);
			ML_bind_scope(value->m_vbo);
			ML_bind_scope(value->m_ibo);
			gl::render_command::draw_indexed(value->m_ibo.m_count)();
		}
		else
		{
			ML_bind_scope(value->m_vao);
			ML_bind_scope(value->m_vbo);
			gl::render_command::draw_arrays(0, value->m_vbo.m_size)();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}