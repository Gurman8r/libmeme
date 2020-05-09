#ifndef _ML_PLUGIN_MANAGER_HPP_
#define _ML_PLUGIN_MANAGER_HPP_

#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/BatchVector.hpp>

namespace ml
{
	struct ML_ENGINE_API plugin_manager final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin_manager(json const & j, allocator_type alloc) noexcept;

		~plugin_manager() noexcept { clear(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear();

		bool free(fs::path const & path);

		hash_t load(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::batch_vector<
			hash_t,			// code
			fs::path,		// name
			shared_library,	// library
			plugin *		// plugin
		> m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_MANAGER_HPP_