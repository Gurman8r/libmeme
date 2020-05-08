#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

// should be optimized (data oriented?)

#include <libmeme/Graphics/Shader.hpp>

namespace ml
{
	// collection of uniforms
	struct ML_GRAPHICS_API material final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type			= typename pmr::polymorphic_allocator<byte_t>;
		using storage_type				= typename ds::set<uniform>;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		material(allocator_type alloc = {})
			: m_storage{ alloc }
		{
		}

		explicit material(storage_type const & data, allocator_type alloc = {})
			: m_storage{ data, alloc }
		{
		}

		explicit material(storage_type && data, allocator_type alloc = {}) noexcept
			: m_storage{ std::move(data), alloc }
		{
		}

		material(std::initializer_list<uniform> init, allocator_type alloc = {})
			: m_storage{ init, alloc }
		{
		}

		template <class It
		> material(It first, It last, allocator_type alloc = {})
			: m_storage{ first, last, alloc }
		{
		}

		material(material const & value, allocator_type alloc = {})
			: m_storage{ value.m_storage, alloc }
		{
		}

		material(material && value, allocator_type alloc = {}) noexcept
			: m_storage{ alloc }
		{
			swap(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		material & operator=(material const & value)
		{
			material temp{ value };
			swap(temp);
			return (*this);
		}

		material & operator=(material && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path) { return false; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			m_storage.clear();
		}

		void swap(material & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_storage, value.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD iterator find(pmr::string const & name)
		{
			if (name.empty()) return end();

			return std::find_if(begin(), end(), [&name](auto const & u)
			{
				return u.name() == name;
			});
		}

		ML_NODISCARD const_iterator find(pmr::string const & name) const
		{
			if (name.empty()) return cend();
			
			return std::find_if(cbegin(), cend(), [&name](auto const & u)
			{
				return u.name() == name;
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD iterator find(pmr::string const & name)
		{
			if (auto const it{ find(name) }; it != end() && it->holds<T>())
			{
				return it;
			}
			else
			{
				return end();
			}
		}

		template <class T
		> ML_NODISCARD const_iterator find(pmr::string const & name) const
		{
			if (auto const it{ find(name) }; it != cend() && it->holds<T>())
			{
				return it;
			}
			else
			{
				return cend();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD uniform * get(pmr::string const & name)
		{
			if (auto const it{ find(name) }; it != end())
			{
				return &(*it);
			}
			else
			{
				return nullptr;
			}
		}

		ML_NODISCARD uniform const * get(pmr::string const & name) const
		{
			if (auto const it{ find(name) }; it != end())
			{
				return &(*it);
			}
			else
			{
				return nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD uniform * get(pmr::string const & name)
		{
			if (auto const it{ find<T>(name) }; it != end())
			{
				return &(*it);
			}
			else
			{
				return nullptr;
			}
		}

		template <class T
		> ML_NODISCARD uniform const * get(pmr::string const & name) const
		{
			if (auto const it{ find<T>(name) }; it != cend())
			{
				return &(*it);
			}
			else
			{
				return nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class Data
		> material & set(pmr::string const & name, Data const & data)
		{
			if (uniform * u{ get(name) })
			{
				u->set<T>(data);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool insert(uniform const & value)
		{
			return !value.name().empty() && m_storage.insert(value).second;
		}

		bool insert(uniform && value)
		{
			return !value.name().empty() && m_storage.insert(std::move(value)).second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		iterator erase(iterator it)
		{
			return m_storage.erase(it);
		}

		iterator erase(iterator first, iterator last)
		{
			return m_storage.erase(first, last);
		}

		iterator erase(pmr::string const & name)
		{
			return m_storage.erase(this->find(name));
		}

		template <class T
		> iterator erase(pmr::string const & name)
		{
			return m_storage.erase(this->find<T>(name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		material & concat(material const & value)
		{
			for (auto const & u : value)
			{
				this->insert(u);
			}
			return (*this);
		}

		material & concat(material && value)
		{
			for (auto && u : value)
			{
				this->insert(std::move(u));
			}
			return (*this);
		}

		material concat(material const & value) const
		{
			material temp{ *this };
			return temp.concat(value);
		}

		material concat(material && value) const
		{
			material temp{ *this };
			return temp.concat(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		material & operator+=(material const & value)
		{
			return this->concat(value);
		}

		material & operator+=(material && value)
		{
			return this->concat(std::move(value));
		}

		material operator+(material const & value) const
		{
			return this->concat(value);
		}

		material operator+(material && value) const
		{
			return this->concat(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto operator[](size_t const i) noexcept -> uniform & { return m_storage[i]; }

		ML_NODISCARD auto operator[](size_t const i) const noexcept -> uniform const & { return m_storage[i]; }

		ML_NODISCARD auto at(size_t const i) -> uniform & { return m_storage[i]; }

		ML_NODISCARD auto at(size_t const i) const -> uniform const & { return m_storage[i]; }

		ML_NODISCARD auto begin() noexcept -> iterator { return m_storage.begin(); }
		
		ML_NODISCARD auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		ML_NODISCARD auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }
		
		ML_NODISCARD auto cend() const noexcept -> const_iterator { return m_storage.cend(); }
		
		ML_NODISCARD auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }
		
		ML_NODISCARD auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }
		
		ML_NODISCARD auto end() noexcept -> iterator { return m_storage.end(); }
		
		ML_NODISCARD auto end() const noexcept -> const_iterator { return m_storage.end(); }

		ML_NODISCARD auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }
		
		ML_NODISCARD auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }
		
		ML_NODISCARD auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_material(material::storage_type && s)
	{
		return material{ ML_forward(s) };
	}

	template <class ... Args
	> ML_NODISCARD inline auto make_material(Args && ... args)
	{
		return material{ material::storage_type{ ML_forward(args)... } };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATERIAL_HPP_