#include <libmeme/Engine/Application.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Engine/EngineEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application * application::g_app{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application::application(system_context * sys) noexcept
		: system_object	{ sys }
		, m_plugins		{ sys->mem->allocator() }
	{
		ML_assert(!g_app && (g_app = this));
	}

	application::~application() noexcept
	{
		ML_assert(g_app == this && !(g_app = nullptr));

		while (!m_plugins.get<plugin_id>().empty())
		{
			this->uninstall_plugin(m_plugins.get<plugin_id>().back());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_id application::install_plugin(fs::path const & path, void * user)
	{
		// load library
		if (plugin_id const id{ std::invoke([&, &lib = shared_library{ path }]()
		{
			return !lib ? nullptr : std::get<plugin_id &>(m_plugins.push_back
			(
				ML_handle(plugin_id, lib.hash()), lib.path(), std::move(lib), nullptr,
				plugin_api
				{
					lib.proc<plugin *, application *, void *>("ml_plugin_attach"),
					lib.proc<void, application *, plugin *>("ml_plugin_detach"),
				}
			));
		}) })
		// load plugin
		{
			if (plugin * ptr{ m_plugins.back<plugin_api>().attach(this, user) })
			{
				m_plugins.back<manual<plugin>>().reset(ptr);

				return id;
			}
		}
		// failed
		return nullptr;
	}

	bool application::uninstall_plugin(plugin_id value)
	{
		if (!value) { return false; }
		if (auto const it{ m_plugins.find<plugin_id>(value) }
		; it == m_plugins.end<plugin_id>()) { return false; }
		else
		{
			auto const i{ m_plugins.index_of<plugin_id>(it) };

			plugin * ptr{ m_plugins.at<manual<plugin>>(i).release() };

			m_plugins.at<plugin_api>(i).detach(this, ptr);

			get_memory()->deallocate_object(ptr);

			m_plugins.erase(i);

			return true;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}