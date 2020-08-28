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
			ML_assert(!s_instance);
			s_instance = this;
		}

		~plugin_manager() noexcept override
		{
			ML_assert(s_instance == this);
			for (auto const id : util::dup(m_data.get<plugin_id>()))
			{
				this->uninstall(id);
			}
			s_instance = nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_id install(fs::path path);

		bool uninstall(plugin_id value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static plugin_manager * const get() noexcept { return s_instance; }

		ML_NODISCARD auto sys() const noexcept -> system_context * const { return m_sys; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static plugin_manager * s_instance; // singleton

		system_context * const m_sys; // system

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