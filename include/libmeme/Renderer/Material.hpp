#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <libmeme/Renderer/Uniform.hpp>

namespace ml
{
	// collection of uniforms
	struct ML_RENDERER_API material final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type			= typename pmr::polymorphic_allocator<byte_t>;
		using storage_type				= typename ds::flat_set<uniform>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		material() noexcept : m_storage{} {}

		explicit material(allocator_type const & alloc)
			: m_storage{ alloc }
		{
		}

		explicit material(storage_type const & data, allocator_type const & alloc = {})
			: m_storage{ data, alloc }
		{
		}

		explicit material(storage_type && data, allocator_type const & alloc = {}) noexcept
			: m_storage{ std::move(data), alloc }
		{
		}

		material(std::initializer_list<uniform> init, allocator_type const & alloc = {})
			: m_storage{ init, alloc }
		{
		}

		template <class It
		> material(It first, It last, allocator_type const & alloc = {})
			: m_storage{ first, last, alloc }
		{
		}

		material(material const & other, allocator_type const & alloc = {})
			: m_storage{ other.m_storage, alloc }
		{
		}

		material(material && other, allocator_type const & alloc = {}) noexcept
			: m_storage{ alloc }
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline material & operator=(material const & other)
		{
			material temp{ other };
			swap(temp);
			return (*this);
		}

		inline material & operator=(material && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			m_storage.clear();
		}

		inline void swap(material & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator erase(iterator it)
		{
			return m_storage.erase(it);
		}

		inline iterator erase(iterator first, iterator last)
		{
			return m_storage.erase(first, last);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator find(pmr::string const & name)
		{
			if (!name.empty())
			{
				return std::find_if(begin(), end(), [name](auto const & u)
				{
					return u.name() == name;
				});
			}
			return end();
		}

		ML_NODISCARD inline const_iterator find(pmr::string const & name) const
		{
			if (!name.empty())
			{
				return std::find_if(cbegin(), cend(), [name](auto const & u)
				{
					return u.name() == name;
				});
			}
			return cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline iterator find(pmr::string const & name)
		{
			if (auto const it{ find(name) }; it != end() && it->holds<T>())
			{
				return it;
			}
			return end();
		}

		template <class T
		> ML_NODISCARD inline const_iterator find(pmr::string const & name) const
		{
			if (auto const it{ find(name) }; it != cend() && it->holds<T>())
			{
				return it;
			}
			return cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline uniform * get(pmr::string const & name)
		{
			if (auto const it{ find(name) }; it != end())
			{
				return &(*it);
			}
			return nullptr;
		}

		ML_NODISCARD inline uniform const * get(pmr::string const & name) const
		{
			if (auto const it{ find(name) }; it != end())
			{
				return &(*it);
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline uniform * get(pmr::string const & name)
		{
			if (auto const it{ find<T>(name) }; it != end())
			{
				return &(*it);
			}
			return nullptr;
		}

		template <class T
		> ML_NODISCARD inline uniform const * get(pmr::string const & name) const
		{
			if (auto const it{ find<T>(name) }; it != cend())
			{
				return &(*it);
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline material & set(pmr::string const & name, T const & data)
		{
			if (auto u{ get<T>(name) })
			{
				u->set(data);
			}
			return (*this);
		}

		template <class T
		> inline material & set(pmr::string const & name, T && data)
		{
			if (auto u{ get<T>(name) })
			{
				u->set(std::move(data));
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool insert(uniform const & value)
		{
			return value && m_storage.insert(value).second;
		}

		inline bool insert(uniform && value)
		{
			return value && m_storage.insert(std::move(value)).second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto operator[](size_t index) -> reference { return m_storage[index]; }

		ML_NODISCARD inline auto operator[](size_t index) const -> const_reference { return m_storage[index]; }

		ML_NODISCARD inline auto begin() noexcept -> iterator { return m_storage.begin(); }
		
		ML_NODISCARD inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		ML_NODISCARD inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }
		
		ML_NODISCARD inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }
		
		ML_NODISCARD inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }
		
		ML_NODISCARD inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }
		
		ML_NODISCARD inline auto end() noexcept -> iterator { return m_storage.end(); }
		
		ML_NODISCARD inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		ML_NODISCARD inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }
		
		ML_NODISCARD inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }
		
		ML_NODISCARD inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_material(material::storage_type && s)
	{
		return material{ ML_FWD(s) };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_material(Args && ... args)
	{
		return material{ material::storage_type{ ML_FWD(args)... } };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATERIAL_HPP_