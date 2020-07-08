#ifndef _ML_PLUGIN_MANAGER_HPP_
#define _ML_PLUGIN_MANAGER_HPP_

#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/BatchVector.hpp>

namespace ml
{
	struct ML_ENGINE_API plugin_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		explicit plugin_manager(json const & j, allocator_type alloc) noexcept;

		~plugin_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept { m_data.clear(); }

		bool free(fs::path const & path);

		hash_t load(fs::path const & path, void * user_data = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::batch_vector<
			hash_t,			// file hash
			fs::path,		// file path
			shared_library,	// library instance
			unique<plugin>	// plugin instance
		> m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_MANAGER_HPP_