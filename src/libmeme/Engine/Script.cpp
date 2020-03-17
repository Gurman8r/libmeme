#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Engine.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script::script(allocator_type const & alloc) noexcept
	{
	}

	script::script(script const & other, allocator_type const & alloc)
	{
	}

	script::script(script && other, allocator_type const & alloc) noexcept
	{
	}

	script::~script()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}