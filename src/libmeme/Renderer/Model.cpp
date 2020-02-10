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

	model::model()
		: m_storage{}
	{
	}

	model::model(allocator_type const & alloc)
		: m_storage{ alloc }
	{
	}

	model::model(std::initializer_list<mesh> init, allocator_type const & alloc)
		: m_storage{ init, alloc }
	{
	}

	model::model(fs::path const & path, allocator_type const & alloc)
		: m_storage{ alloc }
	{
		load_from_file(path);
	}

	model::model(storage_type const & storage, allocator_type const & alloc)
		: m_storage{ storage, alloc }
	{
	}

	model::model(storage_type && storage, allocator_type const & alloc) noexcept
		: m_storage{ std::move(storage), alloc }
	{
	}

	model::model(model const & other, allocator_type const & alloc)
		: m_storage{ other.m_storage, alloc }
	{
	}

	model::model(model && other, allocator_type const & alloc) noexcept
		: m_storage{ alloc }
	{
		swap(std::move(other));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	model & model::operator=(model const & other)
	{
		model temp{ other };
		swap(temp);
		return (*this);
	}

	model & model::operator=(model && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void model::swap(model & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_storage.swap(other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool model::load_from_file(fs::path const & path)
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

	bool model::load_from_file(fs::path const & path, uint32_t flags)
	{
		// open scene
		Assimp::Importer _ai;
		aiScene const * scene{ _ai.ReadFile(path.string().c_str(), flags) };
		if (!scene)
		{
			return debug::log_error("Failed reading aiScene from file");
		}

		// cleanup
		if (!m_storage.empty())
			m_storage.clear();

		// reserve space
		m_storage.reserve(
			std::distance(&scene->mMeshes[0], &scene->mMeshes[scene->mNumMeshes])
		);

		// for each mesh
		std::for_each(&scene->mMeshes[0], &scene->mMeshes[scene->mNumMeshes], [&](aiMesh * const & mesh)
		{
			// mesh vertices
			mesh::vertices_t verts{};

			// for each face
			std::for_each(&mesh->mFaces[0], &mesh->mFaces[mesh->mNumFaces], [&](aiFace const & face)
			{
				// reserve space
				verts.reserve(verts.capacity()
					+ std::distance(&face.mIndices[0], &face.mIndices[face.mNumIndices])
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
						vp ? vec3{ vp->x, vp->y, vp->z } : vec3::zero(),
						vn ? vec3{ vn->x, vn->y, vn->z } : vec3::one(),
						uv ? vec2{ uv->x, uv->y } : vec2::one()
					));
				});

			});

			// create the mesh
			m_storage.emplace_back(make_mesh(verts));

		});

		return !m_storage.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void model::draw(render_target const & target, model const * value)
	{
		if (!value) { return; }
		
		for (auto const & elem : (*value))
		{
			target.draw(elem);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}