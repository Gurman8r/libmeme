#include <libmeme/Renderer/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Uniform::Uniform()
		: m_storage{}
	{
	}

	Uniform::Uniform(storage_t const & storage)
		: m_storage{ storage }
	{
	}

	Uniform::Uniform(storage_t && storage) noexcept
		: m_storage{ std::move(storage) }
	{
	}

	Uniform::Uniform(Uniform const & other)
		: Uniform{ other.m_storage }
	{
	}

	Uniform::Uniform(Uniform && other) noexcept
		: Uniform{}
	{
		swap(std::move(other));
	}

	Uniform::~Uniform() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Uniform & Uniform::operator=(Uniform const & other)
	{
		Uniform temp{ other };
		swap(temp);
		return (*this);
	}

	Uniform & Uniform::operator=(Uniform && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Uniform::swap(Uniform & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_storage.swap(other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}