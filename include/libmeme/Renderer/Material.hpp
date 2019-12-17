#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Renderer/Uniform.hpp>

namespace ml
{
	struct ML_RENDERER_API Material final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename Uniform;
		using storage_t					= typename std::vector<Uniform>;
		using iterator					= typename storage_t::iterator;
		using const_iterator			= typename storage_t::const_iterator;
		using reverse_iterator			= typename storage_t::reverse_iterator;
		using const_reverse_iterator	= typename storage_t::const_reverse_iterator;
		using pointer					= typename value_type *;
		using reference					= typename value_type &;
		using const_pointer				= typename value_type const *;
		using const_reference			= typename value_type const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(storage_t const & data);
		Material(storage_t && data) noexcept;
		Material(Material const & copy);
		Material(Material && copy) noexcept;
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material & operator=(Material const & other);

		Material & operator=(Material && other) noexcept;

		void swap(Material & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(Path const & filename);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			return m_storage.clear();
		}

		inline void push_back(Uniform && value)
		{
			return m_storage.push_back(std::move(value));
		}

		inline bool insert(Uniform && value)
		{
			if (value.name().empty()) { return false; }
			if (auto const it{ find_by_name(value.name()) }; it == cend())
			{
				push_back(std::move(value));
				return true;
			}
			return false;
		}

		template <class Pr> inline void sort(Pr && pred) noexcept
		{
			return std::sort(begin(), end(), pred);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline iterator find(T const & value)
		{
			return std::find(begin(), end(), value);
		}

		template <class T> inline iterator find(T const & value) const
		{
			return std::find(cbegin(), cend(), value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Pr> inline iterator find_if(Pr && pr)
		{
			return std::find_if(begin(), end(), pr);
		}

		template <class Pr> inline const_iterator find_if(Pr && pr) const
		{
			return std::find_if(cbegin(), cend(), pr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find_by_name(std::string const & name)
		{
			return (!name.empty()
				? find_if([name](auto && u) { return (u.name() == name); })
				: end()
			);
		}

		inline const_iterator find_by_name(std::string const & name) const
		{
			return (!name.empty()
				? find_if([name](auto && u) { return (u.name() == name); })
				: cend()
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline reference operator[](size_t index) noexcept
		{
			return m_storage[index];
		}

		inline const_reference operator[](size_t index) const noexcept
		{
			return m_storage[index];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin() noexcept -> iterator { return m_storage.begin(); }
		inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }
		inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		inline auto end() noexcept -> iterator { return m_storage.end(); }
		inline auto end() const noexcept -> const_iterator { return m_storage.end(); }
		inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }
		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }
		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }
		inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }
		inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_material(Args && ... args)
	{
		return Material{ Material::storage_t{ std::forward<Args>(args)... } };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATERIAL_HPP_