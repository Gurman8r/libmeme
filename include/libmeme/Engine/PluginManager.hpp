#ifndef _ML_PLUGIN_MANAGER_HPP_
#define _ML_PLUGIN_MANAGER_HPP_

#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/Core/SharedLibrary.hpp>
#include <libmeme/Engine/Plugin.hpp>

namespace ml
{
	struct ML_ENGINE_API plugin_manager final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct plugin_abi final
		{
			std::function<void(system_context *, plugin *)> attach, detach;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin_manager(system_context * sys) noexcept : m_sys{ sys }, m_data{}
		{
		}

		~plugin_manager() noexcept override
		{
			for (auto const id : pmr::vector<plugin_id>{ m_data.get<plugin_id>() })
			{
				this->uninstall(id);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_id install(fs::path path);

		bool uninstall(plugin_id value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;
		ds::batch_vector<
			plugin_id		,	// id
			fs::path		,	// path
			shared_library	,	// library
			plugin_abi		,	// interface
			manual<plugin>		// instance
		> m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_MANAGER_HPP_