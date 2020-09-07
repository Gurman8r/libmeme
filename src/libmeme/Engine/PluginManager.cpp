#include <libmeme/Engine/PluginManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_id plugin_manager::install(fs::path path)
	{
		// load library
		if (auto const id{ std::invoke([&]() -> plugin_id
		{
			if (path.empty()) { return nullptr; }
			if (path.extension().empty() && !fs::exists(path))
			{
				path += shared_library::default_extension;
			}
			if (shared_library lib{ path })
			{
				return std::get<0>(m_data.push_back
				(
					ML_handle(plugin_id, util::hash(path.string())),
					path,
					std::move(lib),
					plugin_api
					{
						lib.proc<plugin *, system_context *>("ml_plugin_attach"),
						lib.proc<void, system_context *, plugin *>("ml_plugin_detach")
					},
					nullptr
				));
			}
			return nullptr;
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
		if (auto const it{ m_data.find<plugin_id>(value) }
		; it != m_data.end<plugin_id>())
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