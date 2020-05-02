#ifndef _ML_SCRIPT_MANAGER_HPP_
#define _ML_SCRIPT_MANAGER_HPP_

// WIP

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_ENGINE_API script_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script_manager(json const & j, allocator_type const & alloc = {}) noexcept;

		~script_manager() noexcept { (void)shutdown(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_initialized() const noexcept;

		ML_NODISCARD bool startup();

		ML_NODISCARD bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t do_file(fs::path const & value);

		int32_t do_string(pmr::string const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		fs::path m_library_home{};
		fs::path m_setup_script{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_MANAGER_HPP_