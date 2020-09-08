#include <libmeme/Engine/PluginManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_id plugin_manager::install(fs::path const & path)
	{
		// load library
		if (auto const id{ std::invoke([&, &lib = shared_library{ path }]() -> plugin_id
		{
			return !lib ? nullptr : std::get<plugin_id &>(m_data.push_back
			(
				ML_handle(plugin_id, lib.hash()), lib.path(), std::move(lib), nullptr,
				plugin_api
				{
					lib.proc<plugin *, system_context *>("ml_plugin_attach"),
					lib.proc<void, system_context *, plugin *>("ml_plugin_detach")
				}
			));
		}) })
		// load plugin
		{
			if (plugin * ptr{ m_data.back<plugin_api>().attach(m_sys) })
			{
				m_data.back<plugin *>() = ptr;

				return id;
			}
		}
		// failed
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool plugin_manager::uninstall(plugin_id value)
	{
		if (!value) { return false; }
		if (auto const it{ m_data.find<plugin_id>(value) }; it != m_data.end<plugin_id>())
		{
			auto const i{ m_data.index_of<plugin_id>(it) };

			plugin * ptr{ m_data.at<plugin *>(i) };

			m_data.at<plugin_api>(i).detach(m_sys, ptr);

			ml_free(ptr);

			m_data.erase(i);

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}