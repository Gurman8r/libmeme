#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Material::Material()
		: m_storage{}
	{
	}

	Material::Material(storage_t const & data)
		: m_storage{ data }
	{
	}

	Material::Material(storage_t && data) noexcept
		: m_storage{ std::move(data) }
	{
	}

	Material::Material(Material const & other)
		: m_storage{ other.m_storage }
	{
	}

	Material::Material(Material && other) noexcept
		: m_storage{}
	{
		swap(std::move(other));
	}

	Material::~Material() {}

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
			m_storage.swap(other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::loadFromFile(path_t const & filename)
	{
		if (std::ifstream in{ filename })
		{
			in.close();

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}