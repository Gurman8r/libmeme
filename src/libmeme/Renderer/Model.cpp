#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>

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

	Model::Model()
		: m_storage{}
	{
	}

	Model::Model(initializer_type init)
		: m_storage{ init }
	{
	}

	Model::Model(path_t const & path)
		: m_storage{}
	{
		load_from_file(path);
	}

	Model::Model(storage_type const & storage)
		: m_storage{ storage }
	{
	}

	Model::Model(storage_type && storage) noexcept
		: m_storage{ std::move(storage) }
	{
	}

	Model::Model(Model const & other)
		: m_storage{ other.m_storage }
	{
	}

	Model::Model(Model && other) noexcept
		: m_storage{}
	{
		swap(std::move(other));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Model & Model::operator=(Model const & other)
	{
		Model temp{ other };
		swap(temp);
		return (*this);
	}

	Model & Model::operator=(Model && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Model::swap(Model & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_storage.swap(other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Model::load_from_file(path_t const & path)
	{
		return load_from_file(path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords
		);
	}

	bool Model::load_from_file(path_t const & path, uint32_t flags)
	{
		if (!m_storage.empty()) { m_storage.clear(); }

		// Read File
		Assimp::Importer ai;
		if (aiScene const * scene{ ai.ReadFile(path.string().c_str(), flags) })
		{
			// Iterate Meshes
			for (aiMesh ** m = &scene->mMeshes[0]; m != &scene->mMeshes[scene->mNumMeshes]; m++)
			{
				// Vertices
				std::vector<Vertex> verts;

				// Iterate Faces
				for (aiFace * f = &(*m)->mFaces[0]; f != &(*m)->mFaces[(*m)->mNumFaces]; f++)
				{
					// Iterate Indices
					for (uint32_t * i = &f->mIndices[0]; i != &f->mIndices[f->mNumIndices]; ++i)
					{
						auto const * vp{ (*m)->mVertices ? &(*m)->mVertices[*i] : nullptr };
						auto const * vn{ (*m)->mNormals ? &(*m)->mNormals[*i] : nullptr };
						auto const * uv{ (*m)->HasTextureCoords(0) ? &(*m)->mTextureCoords[0][*i] : nullptr };

						// Make Vertex
						verts.emplace_back(make_vertex(
							vp ? vec3{ vp->x, vp->y, vp->z } : vec3::zero(),
							vn ? vec3{ vn->x, vn->y, vn->z } : vec3::one(),
							uv ? vec2{ uv->x, uv->y } : vec2::one()
						));
					}
				}

				// Make Mesh
				m_storage.emplace_back(make_mesh(verts));
			}
		}
		return !m_storage.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Model::draw(RenderTarget const & target, Model const * value)
	{
		if (!value) { return; }

		for (auto const & elem : (*value))
		{
			target.draw(elem);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}