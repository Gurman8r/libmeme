#include <libmeme/Renderer/Mesh.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh() : m_storage{ std::make_tuple(
		make_bufferlayout(), make_vao(), make_vbo(), make_ibo()
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
		return load_from_memory(alg::contiguous(vertices), indices);
	}

	bool Mesh::load_from_memory(vertices_t const & vertices)
	{
		return load_from_memory(alg::contiguous(vertices));
	}

	bool Mesh::load_from_memory(contiguous_t const & vertices, indices_t const & indices)
	{
		if (!vertices.empty() && !indices.empty())
		{
			if (ML_BIND(VertexArrayObject, vao().generate(GL::Triangles)))
			{
				if (ML_BIND(VertexBufferObject, vbo().generate(GL::StaticDraw)))
				{
					if (ML_BIND(IndexBufferObject, ibo().generate(GL::StaticDraw, GL::UnsignedInt)))
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

	bool Mesh::load_from_memory(contiguous_t const & vertices)
	{
		if (!vertices.empty())
		{
			if (ML_BIND(VertexArrayObject, vao().generate(GL::Triangles)))
			{
				if (ML_BIND(VertexBufferObject, vbo().generate(GL::StaticDraw)))
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
		if (!value) { return; }

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}