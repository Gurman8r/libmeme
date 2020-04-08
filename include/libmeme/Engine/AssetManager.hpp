#ifndef _ML_ASSET_MANAGER_HPP_
#define _ML_ASSET_MANAGER_HPP_

// WIP

#include <libmeme/Engine/Asset.hpp>

namespace ml
{
	struct ML_ENGINE_API asset_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using categories_t = ds::flat_map<hash_t, ds::flat_map<hash_t, std::any>>;

		using ref_t = std::optional<std::reference_wrapper<std::any>>;

		using cref_t = std::optional<std::reference_wrapper<std::any const>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		asset_manager() noexcept;

		~asset_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept { m_data.clear(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> std::optional<T> get(hash_t const name)
		{
			if (auto const cat{ m_data.find(hashof_v<T>) })
			{
				if (auto const it{ cat->second->find(name) })
				{
					std::any & a{ *it->second };

					return std::make_optional<T>(std::any_cast<T>(a));
				}
			}
			return std::nullopt;
		}

		template <class T
		> std::optional<T> get(pmr::string const & name)
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
					std::any & a{ *it->second };
					a.emplace<T>(ML_forward(args)...);
					return true;
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

		template <class T
		> ref_t ref(hash_t const name)
		{
			if (auto const cat{ m_data.find(hashof_v<T>) })
			{
				if (auto const it{ cat->second->find(name) })
				{
					std::any & a{ *it->second };

					return std::make_optional(std::ref(a));
				}
			}
			return std::nullopt;
		}

		template <class T
		> ref_t ref(pmr::string const & name)
		{
			return ref<T>(util::hash(name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> cref_t cref(hash_t const name) const
		{
			if (auto const cat{ m_data.find(hashof_v<T>) })
			{
				if (auto const it{ cat->second->find(name) })
				{
					std::any const & a{ *it->second };

					return std::make_optional(std::cref(a));
				}
			}
			return std::nullopt;
		}

		template <class T
		> cref_t cref(pmr::string const & name) const
		{
			return cref<T>(util::hash(name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		categories_t m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ASSET_MANAGER_HPP_