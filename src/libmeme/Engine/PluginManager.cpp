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

	void plugin_manager::clear()
	{
		//for (auto & p : m_data.get<plugin *>())
		//{
		//	memory::deallocate(p);
		//}
		m_data.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool plugin_manager::free(fs::path const & path)
	{
		// path empty
		if (path.empty()) { return false; }

		// hash code
		auto const code{ util::hash(path.filename().string()) };

		// lookup code
		if (auto const it{
			std::find(m_data.begin<hash_t>(), m_data.end<hash_t>(), code)
		}; it == m_data.end<hash_t>())
		{
			m_data.erase((size_t)std::distance(m_data.begin<hash_t>(), it));

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	hash_t plugin_manager::load(fs::path const & path, void * user_data)
	{
		// path empty
		if (path.empty()) { return 0; }

		// hash code
		auto const code{ util::hash(path.filename().string()) };

		// lookup code
		if (auto const it{
			std::find(m_data.begin<hash_t>(), m_data.end<hash_t>(), code)
		}; it == m_data.end<hash_t>())
		{
			// load library
			if (shared_library lib{ path })
			{
				// load plugin
				if (auto const p{ lib.call<plugin *>(ML_PLUGIN_MAIN, user_data) })
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