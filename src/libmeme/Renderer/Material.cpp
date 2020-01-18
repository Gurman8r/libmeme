#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material::Material()
		: m_storage{}
	{
	}

	Material::Material(allocator_type const & alloc)
		: m_storage{ alloc }
	{
	}

	Material::Material(storage_type const & data, allocator_type const & alloc)
		: m_storage{ data, alloc }
	{
	}

	Material::Material(storage_type && data, allocator_type const & alloc) noexcept
		: m_storage{ std::move(data), alloc }
	{
	}

	Material::Material(Material const & other, allocator_type const & alloc)
		: m_storage{ other.m_storage, alloc }
	{
	}

	Material::Material(Material && other, allocator_type const & alloc) noexcept
		: m_storage{ alloc }
	{
		swap(std::move(other));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material & Material::operator=(Material const & other)
	{
		Material temp{ other };
		swap(temp);
		return (*this);
	}

	Material & Material::operator=(Material && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Material::swap(Material & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_storage, other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::load_from_file(path_t const & path)
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