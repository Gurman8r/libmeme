#include <libmeme/Engine/PluginManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_manager::plugin_manager(json const & j, allocator_type const & alloc) noexcept
		: m_files{ alloc }
		, m_libs{ alloc }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void plugin_manager::clear()
	{
		m_libs.for_each([](auto const &, plugin * p) { memory_manager::deallocate(p); });
		m_files.clear();
	}

	bool plugin_manager::free(fs::path const & path)
	{
		return false;
	}

	bool plugin_manager::load(fs::path const & path)
	{
		// path empty
		if (path.empty()) { return false; }

		// check file name already loaded
		if (auto const file{ m_files.insert(path.filename()) }; file.second)
		{
			// load library
			if (auto lib{ make_shared_library(*file.first) })
			{
				// load plugin
				if (auto const ptr{ lib.call_function<plugin *>("ml_plugin_main") })
				{
					m_libs[std::move(lib)] = ptr.value();

					return true;
				}
			}
			m_files.erase(file.first);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}