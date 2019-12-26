#include <libmeme/Renderer/Mesh.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/FileSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh() : m_storage{ std::make_tuple(
		BufferLayout{ {
			{ 0, 3, GL::Float, false, Vertex::Size, 0, sizeof(float_t) },
			{ 1, 3, GL::Float, false, Vertex::Size, 3, sizeof(float_t) },
			{ 2, 2, GL::Float, false, Vertex::Size, 6, sizeof(float_t) } }
		}, VAO{}, VBO{}, IBO{}
	) }
	{
	}

	Mesh::Mesh(path_t const & path)
		: Mesh{}
	{
	}

	Mesh::Mesh(Mesh const & other)
		: Mesh{}
	{
	}

	Mesh::Mesh(Mesh && other) noexcept
		: Mesh{}
	{
	}

	Mesh::~Mesh()
	{
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

		}
	}

	void Mesh::swap(Mesh & other) noexcept
	{
		if (this != std::addressof(other))
		{

		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Mesh::loadFromFile(path_t const & path)
	{
		if (std::ifstream in{ path })
		{
			in.close();

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}