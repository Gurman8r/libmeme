#include <libmeme/Client/LoopSystem.hpp>

namespace ml
{
	player_loop::~player_loop() noexcept
	{
	}
}

namespace ml
{
	loop_system::loop_system(allocator_type alloc) noexcept
		: m_loops		{ alloc }
		, m_main_loop	{}
	{
	}
}