#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	material::material()
		: m_storage{}
	{
	}

	material::material(allocator_type const & alloc)
		: m_storage{ alloc }
	{
	}

	material::material(storage_type const & data, allocator_type const & alloc)
		: m_storage{ data, alloc }
	{
	}

	material::material(storage_type && data, allocator_type const & alloc) noexcept
		: m_storage{ std::move(data), alloc }
	{
	}

	material::material(material const & other, allocator_type const & alloc)
		: m_storage{ other.m_storage, alloc }
	{
	}

	material::material(material && other, allocator_type const & alloc) noexcept
		: m_storage{ alloc }
	{
		swap(std::move(other));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	material & material::operator=(material const & other)
	{
		material temp{ other };
		swap(temp);
		return (*this);
	}

	material & material::operator=(material && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void material::swap(material & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_storage, other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool material::load_from_file(fs::path const & path)
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