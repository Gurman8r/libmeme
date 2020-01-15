#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <libmeme/Renderer/Uniform.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	// collection of uniforms
	struct ML_RENDERER_API Material final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type				= typename std::vector<Uniform>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(storage_type const & data);
		Material(storage_type && data) noexcept;
		Material(Material const & other);
		Material(Material && other) noexcept;
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material & operator=(Material const & other);

		Material & operator=(Material && other) noexcept;

		void swap(Material & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			m_storage.clear();
		}

		inline Uniform push_back(Uniform && value)
		{
			return m_storage.emplace_back(std::move(value));
		}

		inline bool insert(Uniform && value)
		{
			if (!value.name().empty() && !find_by_name(value.name()))
			{
				push_back(std::move(value));
				
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Pr
		> ML_NODISCARD inline iterator find_if(Pr && pr)
		{
			return std::find_if(begin(), end(), pr);
		}

		template <class Pr
		> ML_NODISCARD inline const_iterator find_if(Pr && pr) const
		{
			return std::find_if(cbegin(), cend(), pr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline std::optional<Uniform> find_by_name(std::string const & name)
		{
			if (name.empty()) { return std::nullopt; }
			if (auto it{ find_if([name](auto && u) { return u.name() == name; }) }; it != end())
			{
				return std::make_optional(*it);
			}
			return std::nullopt;
		}

		ML_NODISCARD inline std::optional<Uniform> find_by_name(std::string const & name) const
		{
			if (name.empty()) { return std::nullopt; }
			if (auto it{ find_if([name](auto && u) { return u.name() == name; }) }; it != cend())
			{
				return std::make_optional(*it);
			}
			return std::nullopt;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline reference operator[](size_t index) noexcept
		{
			return m_storage[index];
		}

		ML_NODISCARD inline const_reference operator[](size_t index) const noexcept
		{
			return m_storage[index];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto begin() noexcept -> iterator { return m_storage.begin(); }
		ML_NODISCARD inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }
		ML_NODISCARD inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		ML_NODISCARD inline auto end() noexcept -> iterator { return m_storage.end(); }
		ML_NODISCARD inline auto end() const noexcept -> const_iterator { return m_storage.end(); }
		ML_NODISCARD inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		ML_NODISCARD inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }
		ML_NODISCARD inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }
		ML_NODISCARD inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		ML_NODISCARD inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }
		ML_NODISCARD inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }
		ML_NODISCARD inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_material(Material::storage_type && s)
	{
		return Material{ std::move(s) };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_material(Args && ... args)
	{
		return Material{ Material::storage_type{ std::forward<Args>(args)... } };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATERIAL_HPP_