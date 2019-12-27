#include <libmeme/Renderer/Mesh.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh() : m_storage{ std::make_tuple(
		BufferLayout{}, VAO{}, VBO{}, IBO{}
	) }
	{
	}

	Mesh::Mesh(vertices_t const & vertices)
		: Mesh{}
	{
		loadFromMemory(vertices);
	}

	Mesh::Mesh(vertices_t const & vertices, indices_t const & indices)
		: Mesh{}
	{
		loadFromMemory(vertices, indices);
	}

	Mesh::Mesh(Mesh const & other)
		: Mesh{}
	{
		assign(other);
	}

	Mesh::Mesh(Mesh && other) noexcept
		: Mesh{}
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
		assign(other);
		return (*this);
	}

	Mesh & Mesh::operator=(Mesh && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Mesh::assign(Mesh const & other)
	{
		if (this != std::addressof(other))
		{
			m_storage = other.m_storage;
		}
	}

	void Mesh::swap(Mesh & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_storage.swap(other.m_storage);
		}
	}

	void Mesh::destroy()
	{
		vao().destroy();
		vbo().destroy();
		ibo().destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Mesh::loadFromMemory(vertices_t const & vertices, indices_t const & indices)
	{
		return loadFromMemory(alg::contiguous(vertices), indices);
	}

	bool Mesh::loadFromMemory(vertices_t const & vertices)
	{
		return loadFromMemory(alg::contiguous(vertices));
	}

	bool Mesh::loadFromMemory(contiguous_t const & vertices, indices_t const & indices)
	{
		if (!vertices.empty() && !indices.empty())
		{
			if (ML_BIND(VAO, vao().create(GL::Triangles)))
			{
				if (ML_BIND(VBO, vbo().create(GL::StaticDraw)))
				{
					if (ML_BIND(IBO, ibo().create(GL::StaticDraw, GL::UnsignedInt)))
					{
						vbo().update((void *)vertices.data(), (uint32_t)vertices.size());

						ibo().update((void *)indices.data(), (uint32_t)indices.size());

						layout().bind();

						return true;
					}
				}
			}
		}
		return false;
	}

	bool Mesh::loadFromMemory(contiguous_t const & vertices)
	{
		if (!vertices.empty())
		{
			if (ML_BIND(VAO, vao().create(GL::Triangles)))
			{
				if (ML_BIND(VBO, vbo().create(GL::StaticDraw)))
				{
					vbo().update((void *)vertices.data(), (uint32_t)vertices.size());

					layout().bind();

					return true;
				}
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Mesh::draw(RenderTarget const & target, Mesh const * value)
	{
		if (value)
		{
			if (value->vao() && value->vbo())
			{
				if (value->ibo())
				{
					target.draw(value->vao(), value->vbo(), value->ibo());
				}
				else
				{
					target.draw(value->vao(), value->vbo());
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}