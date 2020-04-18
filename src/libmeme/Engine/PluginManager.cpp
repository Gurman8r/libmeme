#include <libmeme/Engine/PluginManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_manager::plugin_manager(json const & j, allocator_type const & alloc) noexcept
		: m_data{ alloc }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void plugin_manager::clear()
	{
		for (auto & p : m_data.get<plugin *>())
		{
			memory_manager::deallocate(p);
		}

		m_data.clear();
	}

	bool plugin_manager::free(fs::path const & path)
	{
		return false;
	}

	bool plugin_manager::load(fs::path const & path)
	{
		// filename
		auto const fname{ path.filename() };

		// path empty
		if (fname.empty()) { return false; }

		// check already loaded
		if (auto it{ std::find(m_data.begin<fs::path>(), m_data.end<fs::path>(), fname) }
		; it == m_data.end<fs::path>())
		{
			// load library
			if (auto && lib{ make_shared_library(path) })
			{
				// load plugin
				if (auto const ptr{ lib.call<plugin *>("ml_plugin_main") })
				{
					m_data.push_back(fname, std::move(lib), ptr.value());

					return true;
				}
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}