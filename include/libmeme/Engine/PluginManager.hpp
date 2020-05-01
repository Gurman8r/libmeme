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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_manager(json const & j, allocator_type const & alloc = {}) noexcept;

		~plugin_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear();

		bool free(fs::path const & path);

		hash_t load(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::batch_vector<
			hash_t,			// file hash
			fs::path,		// file name
			shared_library,	// library
			plugin *		// instance
		> m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_MANAGER_HPP_