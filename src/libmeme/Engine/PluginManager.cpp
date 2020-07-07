#include <libmeme/Engine/PluginManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_manager::plugin_manager(json const & j, allocator_type alloc) noexcept
		: m_data{ alloc }
	{
	}

	plugin_manager::~plugin_manager() noexcept
	{
		clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool plugin_manager::free(fs::path const & path)
	{
		// path empty
		if (path.empty()) { return false; }

		// lookup file
		if (auto const it{ m_data.find<hash_t>(util::hash(path.filename().string())) }
		; it != m_data.end<hash_t>())
		{
			m_data.erase(m_data.index_of<hash_t>(it));

			return true;
		}
		else
		{
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	hash_t plugin_manager::load(fs::path const & path, void * user_data)
	{
		// path empty
		if (path.empty()) { return 0; }

		// lookup file
		if (auto const code{ util::hash(path.filename().string()) }
		; m_data.find<hash_t>(code) == m_data.end<hash_t>())
		{
			// load library
			if (shared_library lib{ path })
			{
				// load plugin
				if (auto const optl{ lib.call<plugin *>(ML_PLUGIN_MAIN, user_data) })
				{
					m_data.push_back(code, path, std::move(lib), optl.value());

					return code;
				}
			}
		}

		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}