#include <libmeme/Engine/FileManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	file_manager::file_manager(json const & j, allocator_type alloc) noexcept
	{
		m_program_path = fs::current_path();

		j["content_home"].get_to(m_content_home);
	}

	file_manager::~file_manager() noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}