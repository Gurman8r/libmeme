#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Model::Model()
		: m_storage{}
	{
	}

	Model::Model(allocator_type const & alloc)
		: m_storage{ alloc }
	{
	}

	Model::Model(initializer_type init, allocator_type const & alloc)
		: m_storage{ init, alloc }
	{
	}

	Model::Model(path_t const & path, allocator_type const & alloc)
		: m_storage{ alloc }
	{
		load_from_file(path);
	}

	Model::Model(storage_type const & storage, allocator_type const & alloc)
		: m_storage{ storage, alloc }
	{
	}

	Model::Model(storage_type && storage, allocator_type const & alloc) noexcept
		: m_storage{ std::move(storage), alloc }
	{
	}

	Model::Model(Model const & other, allocator_type const & alloc)
		: m_storage{ other.m_storage, alloc }
	{
	}

	Model::Model(Model && other, allocator_type const & alloc) noexcept
		: m_storage{ alloc }
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
		// open scene
		Assimp::Importer _ai;
		aiScene const * scene{ _ai.ReadFile(path.string().c_str(), flags) };
		if (!scene)
		{
			return debug::log_error("Failed reading aiScene from file");
		}
		
		// cleanup
		if (!m_storage.empty()) { m_storage.clear(); }

		// for each mesh
		std::for_each(&scene->mMeshes[0], &scene->mMeshes[scene->mNumMeshes], [&](aiMesh * const & mesh)
		{
			// vertices
			pmr::vector<vertex> verts;

			// for each face
			std::for_each(&mesh->mFaces[0], &mesh->mFaces[mesh->mNumFaces], [&](aiFace const & face)
			{
				// reserve space
				verts.reserve(verts.size() +
					std::distance(&face.mIndices[0], &face.mIndices[face.mNumIndices])
				);

				// for each index
				std::for_each(&face.mIndices[0], &face.mIndices[face.mNumIndices], [&](uint32_t const & i)
				{
					auto const vp{ // position
						mesh->mVertices ? &mesh->mVertices[i] : nullptr
					};
					auto const vn{ // normal
						mesh->mNormals ? &mesh->mNormals[i] : nullptr
					};
					auto const uv{ // texcoord
						mesh->HasTextureCoords(0) ? &mesh->mTextureCoords[0][i] : nullptr
					};

					// make vertex
					verts.emplace_back(make_vertex(
						vp ? vec3{ vp->x, vp->y, vp->z } : vec3{ 0 },
						vn ? vec3{ vn->x, vn->y, vn->z } : vec3{ 0 },
						uv ? vec2{ uv->x, uv->y } : vec2{ 0 }
					));
				});
			});

			// make mesh
			verts.shrink_to_fit();
			m_storage.emplace_back(make_mesh(verts));
		});

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