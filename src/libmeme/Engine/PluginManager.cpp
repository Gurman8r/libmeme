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
		// path empty
		if (path.empty()) { return false; }

		// code
		auto const code{ util::hash(path.filename().string()) };

		// lookup
		if (auto const it{
			std::find(m_data.begin<hash_t>(), m_data.end<hash_t>(), code)
		}; it == m_data.end<hash_t>())
		{
			auto const i{ (size_t)std::distance(m_data.begin<hash_t>(), it) };

			memory_manager::deallocate(m_data.get<plugin *>(i));

			m_data.erase(i);

			return true;
		}
		return false;
	}

	hash_t plugin_manager::load(fs::path const & path)
	{
		// path empty
		if (path.empty()) { return 0; }

		// code
		auto const code{ util::hash(path.filename().string()) };

		// lookup
		if (auto const it{
			std::find(m_data.begin<hash_t>(), m_data.end<hash_t>(), code)
		}; it == m_data.end<hash_t>())
		{
			// load library
			if (auto && lib{ make_shared_library(path) })
			{
				// load plugin
				if (auto const p{ lib.call<plugin *>("ml_plugin_main") })
				{
					m_data.push_back(code, path, std::move(lib), p.value());

					return code;
				}
			}
		}

		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}