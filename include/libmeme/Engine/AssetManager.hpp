#ifndef _ML_ASSET_MANAGER_HPP_
#define _ML_ASSET_MANAGER_HPP_

// WIP

#include <libmeme/Engine/Asset.hpp>

namespace ml
{
	struct ML_ENGINE_API asset_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using storage_type		= typename ds::flat_map<hash_t, ds::flat_map<hash_t, asset>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		asset_manager(json const & j, allocator_type const & alloc = {}) noexcept;

		~asset_manager() noexcept { clear(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			m_data.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> auto get(hash_t const name)
		{
			if (auto const cat{ m_data.find(hashof_v<T>) })
			{
				if (auto const it{ cat->second->find(name) })
				{
				}
			}
			return std::nullopt;
		}

		template <class T
		> auto get(pmr::string const & name)
		{
			return get<T>(util::hash(name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args
		> bool set(hash_t const name, Args && ... args)
		{
			if (auto const cat{ m_data.find(hashof_v<T>) })
			{
				if (auto const it{ cat->second->find(name) })
				{
				}
			}
			return false;
		}

		template <class T, class ... Args
		> bool set(pmr::string const & name, Args && ... args)
		{
			return set<T>(util::hash(name), ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ASSET_MANAGER_HPP_