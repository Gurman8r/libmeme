#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Engine.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script::script(allocator_type const & alloc) noexcept
		: m_hooks{ alloc }
	{
	}

	script::script(script const & other, allocator_type const & alloc)
		: m_hooks{ alloc }
	{
	}

	script::script(script && other, allocator_type const & alloc) noexcept
		: m_hooks{ alloc }
	{
	}

	script::~script()
	{
		m_hooks.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void script::swap(script & other) noexcept
	{
		if (this != std::addressof(other))
		{
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}