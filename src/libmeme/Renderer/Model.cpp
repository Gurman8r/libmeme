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

	model::model(allocator_type const & alloc)
		: m_meshes{ alloc }
	{
	}

	model::model(std::initializer_list<mesh> init, allocator_type const & alloc)
		: m_meshes{ init, alloc }
	{
	}

	model::model(fs::path const & path, allocator_type const & alloc)
		: m_meshes{ alloc }
	{
		load_from_file(path);
	}

	model::model(storage_type const & storage, allocator_type const & alloc)
		: m_meshes{ storage, alloc }
	{
	}

	model::model(storage_type && storage, allocator_type const & alloc) noexcept
		: m_meshes{ std::move(storage), alloc }
	{
	}

	model::model(model const & other, allocator_type const & alloc)
		: m_meshes{ other.m_meshes, alloc }
	{
	}

	model::model(model && other, allocator_type const & alloc) noexcept
		: m_meshes{ alloc }
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
			m_meshes.swap(other.m_meshes);
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

	bool model::load_from_file(fs::path const & path, int32_t flags)
	{
		// open scene
		Assimp::Importer _ai;
		aiScene const * scene{ _ai.ReadFile(path.string().c_str(), flags) };
		if (!scene)
		{
			return debug::log::error("model failed reading assimp scene");
		}

		// cleanup
		if (!m_meshes.empty())
			m_meshes.clear();

		// reserve space
		m_meshes.reserve(
			std::distance(&scene->mMeshes[0], &scene->mMeshes[scene->mNumMeshes])
		);

		// for each mesh
		std::for_each(&scene->mMeshes[0], &scene->mMeshes[scene->mNumMeshes], [&](aiMesh * const m)
		{
			// mesh vertices
			pmr::vector<vertex> verts{};

			// for each face
			std::for_each(&m->mFaces[0], &m->mFaces[m->mNumFaces], [&](aiFace const & f)
			{
				// reserve space
				verts.reserve(verts.capacity()
					+ std::distance(&f.mIndices[0], &f.mIndices[f.mNumIndices])
				);

				// for each index
				std::for_each(&f.mIndices[0], &f.mIndices[f.mNumIndices], [&](uint32_t i)
				{
					auto const vp{ // position
						m->mVertices ? &m->mVertices[i] : nullptr
					};
					auto const vn{ // normal
						m->mNormals ? &m->mNormals[i] : nullptr
					};
					auto const uv{ // texcoord
						m->HasTextureCoords(0) ? &m->mTextureCoords[0][i] : nullptr
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
			m_meshes.emplace_back(make_mesh(verts));

		});

		return !m_meshes.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void model::draw(render_target const & target, model const * value)
	{
		if (!value) { return; }
		
		for (mesh const & e : (*value))
		{
			target.draw(e);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}