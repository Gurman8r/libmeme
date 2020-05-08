#ifndef _ML_FILE_MANAGER_HPP_
#define _ML_FILE_MANAGER_HPP_

// WIP

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/FileUtility.hpp>

namespace ml
{
	struct ML_ENGINE_API file_manager final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		file_manager(json const & j, allocator_type alloc = {}) noexcept;
		
		~file_manager() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD fs::path path2(fs::path const & value = {}) const noexcept
		{
			return m_content_home.native() + value.native();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD fs::path const & program_path() const & noexcept { return m_program_path; }

		ML_NODISCARD fs::path const & content_home() const & noexcept { return m_content_home; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		fs::path m_program_path{};
		fs::path m_content_home{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_FILE_MANAGER_HPP_