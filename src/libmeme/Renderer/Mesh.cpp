#include <libmeme/Renderer/Mesh.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Platform/FileSystem.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh() : m_storage{ std::make_tuple(
		make_buffer_layout(), make_vao(), make_vbo(), make_ibo()
	) }
	{
	}

	Mesh::Mesh(vertices_t const & vertices)
		: m_storage{}
	{
		load_from_memory(vertices);
	}

	Mesh::Mesh(vertices_t const & vertices, indices_t const & indices)
		: m_storage{}
	{
		load_from_memory(vertices, indices);
	}

	Mesh::Mesh(Mesh const & other)
		: m_storage{ other.m_storage }
	{
	}

	Mesh::Mesh(Mesh && other) noexcept
		: m_storage{}
	{
		swap(std::move(other));
	}

	Mesh::~Mesh()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh & Mesh::operator=(Mesh const & other)
	{
		Mesh temp{ other };
		swap(temp);
		return (*this);
	}

	Mesh & Mesh::operator=(Mesh && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Mesh::swap(Mesh & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_storage, other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Mesh::destroy()
	{
		vao().destroy();
		vbo().destroy();
		ibo().destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Mesh::load_from_memory(vertices_t const & vertices, indices_t const & indices)
	{
		return load_from_memory(util::contiguous(vertices), indices);
	}

	bool Mesh::load_from_memory(vertices_t const & vertices)
	{
		return load_from_memory(util::contiguous(vertices));
	}

	bool Mesh::load_from_memory(contiguous_t const & vertices, indices_t const & indices)
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
		if (!vao().generate(GL::Triangles))
		{
			destroy();
			return debug::log_error("mesh failed creating VAO");
		}

		// create vbo
		if (!vbo().generate(GL::StaticDraw))
		{
			destroy();
			return debug::log_error("mesh failed creating VBO");
		}

		// create ibo
		if (!ibo().generate(GL::StaticDraw, GL::UnsignedInt))
		{
			destroy();
			return debug::log_error("mesh failed creating IBO");
		}
		
		// bind buffers
		ML_BIND_SCOPE_M(vao());
		ML_BIND_SCOPE_M(vbo());
		ML_BIND_SCOPE_M(ibo());
		
		// update buffers
		vbo().update((void *)vertices.data(), (uint32_t)vertices.size());
		ibo().update((void *)indices.data(), (uint32_t)indices.size());
		
		// apply layout
		layout().apply();
		
		return true;
	}

	bool Mesh::load_from_memory(contiguous_t const & vertices)
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
		if (!vao().generate(GL::Triangles))
		{
			destroy();
			return debug::log_error("mesh failed creating VAO");
		}

		// create vbo
		if (!vbo().generate(GL::StaticDraw))
		{
			destroy();
			return debug::log_error("mesh failed creating VBO");
		}

		// bind buffers
		ML_BIND_SCOPE_M(vao());
		ML_BIND_SCOPE_M(vbo());

		// update buffers
		vbo().update((void *)vertices.data(), (uint32_t)vertices.size());
		
		// apply layout
		layout().apply();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Mesh::draw(RenderTarget const & target, Mesh const * value)
	{
		if (!value || !value->vao() || !value->vbo())
			return;

		if (value->ibo())
		{
			target.draw(value->vao(), value->vbo(), value->ibo());
		}
		else
		{
			target.draw(value->vao(), value->vbo());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}