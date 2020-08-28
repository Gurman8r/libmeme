#include <libmeme/Engine/PluginManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_manager * plugin_manager::s_instance{};

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
					plugin_iface
					{
						lib.proc<void, system_context *, plugin *>("ml_plugin_attach"),
						lib.proc<void, system_context *, plugin *>("ml_plugin_detach")
					},
					nullptr
				));
			}
			return nullptr;
		}) })
		// load plugin
		{
			plugin * ptr{};
			if (m_data.back<plugin_iface>().attach(m_sys, ptr); ptr)
			{
				m_data.back<manual<plugin>>().reset(ptr);

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

			m_data.at<plugin_iface>(i).detach
			(
				m_sys, m_data.at<manual<plugin>>(i).release()
			);

			m_data.erase(i);

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}