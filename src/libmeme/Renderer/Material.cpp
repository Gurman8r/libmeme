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

	Material::Material(Material const & copy)
		: Material{ copy.m_storage }
	{
	}

	Material::Material(Material && copy) noexcept
		: Material{}
	{
		swap(std::move(copy));
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

	void Material::swap(Material & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_storage.swap(other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Material::loadFromFile(Path const & filename)
	{
		if (std::ifstream in{ filename })
		{
			in.close();
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}