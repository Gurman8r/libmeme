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
		using storage_type				= typename pmr::vector<uniform>;
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

		template <class ... Args
		> inline uniform & emplace_back(Args && ... args)
		{
			return m_storage.emplace_back(ML_FWD(args)...);
		}

		inline void push_back(uniform const & value)
		{
			this->emplace_back(value);
		}

		inline void push_back(uniform && value)
		{
			this->emplace_back(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool insert(uniform const & value)
		{
			if (!value.name().empty() && !this->find(value.name()))
			{
				push_back(value);

				return true;
			}
			return false;
		}

		inline bool insert(uniform && value)
		{
			if (!value.name().empty() && !this->find(value.name()))
			{
				push_back(std::move(value));
				
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline uniform * find(pmr::string const & name)
		{
			if (name.empty()) { return nullptr; }

			if (auto it{ std::find_if(begin(), end(), [name](auto const & u) {
				return u.name() == name;
			}) }; it != end())
			{
				return &(*it);
			}
			else
			{
				return nullptr;
			}
		}

		ML_NODISCARD inline uniform const * find(pmr::string const & name) const
		{
			if (name.empty()) { return nullptr; }
			if (auto it{ std::find_if(begin(), end(), [name](auto const & u) {
				return u.name() == name;
			}) }; it != end())
			{
				return &(*it);
			}
			else
			{
				return nullptr;
			}
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
		return material{ std::move(s) };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_material(Args && ... args)
	{
		return material{ material::storage_type{ ML_FWD(args)... } };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATERIAL_HPP_