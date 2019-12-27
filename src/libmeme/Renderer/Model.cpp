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
		loadFromFile(path);
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
		: m_storage{ std::move(other.m_storage) }
	{
	}

	Model::~Model()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Model & Model::operator=(Model const & other)
	{
		assign(other);
		return (*this);
	}

	Model & Model::operator=(storage_type const & storage)
	{
		assign(storage);
		return (*this);
	}

	Model & Model::operator=(Model && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	Model & Model::operator=(storage_type && storage) noexcept
	{
		swap(std::move(storage));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Model::assign(initializer_type init)
	{
		m_storage.assign(init);
	}

	void Model::assign(Model const & other)
	{
		if (this != std::addressof(other))
		{
			m_storage = other.m_storage;
		}
	}

	void Model::assign(storage_type const & storage)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Model::swap(Model & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_storage.swap(other.m_storage);
		}
	}

	void Model::swap(storage_type & storage) noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Model::loadFromFile(path_t const & path)
	{
		return loadFromFile(path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords
		);
	}

	bool Model::loadFromFile(path_t const & path, uint32_t flags)
	{
		if (!m_storage.empty()) { m_storage.clear(); }

		Assimp::Importer ai;
		if (aiScene const * scene{ ai.ReadFile(path.string().c_str(), flags) })
		{
			// Meshes
			for (aiMesh ** m = &scene->mMeshes[0]; m != &scene->mMeshes[scene->mNumMeshes]; m++)
			{
				std::vector<Vertex> verts;

				// Faces
				for (aiFace * f = &(*m)->mFaces[0]; f != &(*m)->mFaces[(*m)->mNumFaces]; f++)
				{
					// Indices
					for (uint32_t * i = &f->mIndices[0]; i != &f->mIndices[f->mNumIndices]; i++)
					{
						auto const * vp{ (*m)->mVertices ? &(*m)->mVertices[*i] : nullptr };
						auto const * vn{ (*m)->mNormals ? &(*m)->mNormals[*i] : nullptr };
						auto const * uv{ (*m)->HasTextureCoords(0) ? &(*m)->mTextureCoords[0][*i] : nullptr };

						verts.emplace_back(make_vertex(
							vp ? vec3{ vp->x, vp->y, vp->z } : vec3::zero(),
							vn ? vec3{ vn->x, vn->y, vn->z } : vec3::one(),
							uv ? vec2{ uv->x, uv->y } : vec2::one()
						));
					}
				}

				m_storage.emplace_back(make_mesh(verts));
			}
		}
		return !m_storage.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Model::draw(RenderTarget const & target, Model const * value)
	{
		if (value)
		{
			for (auto const & elem : (*value))
			{
				target.draw(elem);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}