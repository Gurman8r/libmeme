#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

// should be optimized (data oriented?)

#include <libmeme/Graphics/Mesh.hpp>

namespace ml
{
	struct ML_GRAPHICS_API model final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type			= typename pmr::polymorphic_allocator<byte_t>;
		using storage_type				= typename pmr::vector<mesh>;
		using difference_type			= typename storage_type::difference_type;
		using size_type					= typename storage_type::size_type;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		model(allocator_type alloc = {}) noexcept;
		
		model(std::initializer_list<mesh> init, allocator_type alloc = {});
		
		model(fs::path const & path, allocator_type alloc = {});
		
		model(storage_type const & storage, allocator_type alloc = {});
		
		model(storage_type && storage, allocator_type alloc = {}) noexcept;
		
		model(model const & value, allocator_type alloc = {});
		
		model(model && value, allocator_type alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		model & operator=(model const & value);

		model & operator=(model && value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(model & value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path);

		bool load_from_file(fs::path const & path, int32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(model const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto operator[](size_type const i) -> reference { return m_meshes[i]; }

		ML_NODISCARD auto operator[](size_type const i) const -> const_reference { return m_meshes[i]; }

		ML_NODISCARD auto back() noexcept -> reference { return m_meshes.back(); }

		ML_NODISCARD auto back() const noexcept -> const_reference { return m_meshes.back(); }

		ML_NODISCARD auto begin() noexcept -> iterator { return m_meshes.begin(); }

		ML_NODISCARD auto begin() const noexcept -> const_iterator { return m_meshes.begin(); }

		ML_NODISCARD auto capacity() const noexcept -> size_type { return m_meshes.capacity(); }

		ML_NODISCARD auto cbegin() const noexcept -> const_iterator { return m_meshes.cbegin(); }

		ML_NODISCARD auto cend() const noexcept -> const_iterator { return m_meshes.cend(); }

		ML_NODISCARD auto crbegin() const noexcept -> const_reverse_iterator { return m_meshes.crbegin(); }

		ML_NODISCARD auto crend() const noexcept -> const_reverse_iterator { return m_meshes.crend(); }

		ML_NODISCARD auto data() noexcept -> pointer { return m_meshes.data(); }

		ML_NODISCARD auto data() const noexcept -> const_pointer { return m_meshes.data(); }

		ML_NODISCARD bool empty() const noexcept { return m_meshes.empty(); }

		ML_NODISCARD auto end() noexcept -> iterator { return m_meshes.end(); }

		ML_NODISCARD auto end() const noexcept -> const_iterator { return m_meshes.end(); }

		ML_NODISCARD auto front() noexcept -> reference { return m_meshes.front(); }

		ML_NODISCARD auto front() const noexcept -> const_reference { return m_meshes.front(); }

		ML_NODISCARD auto get_allocator() const noexcept -> allocator_type { return m_meshes.get_allocator(); }

		ML_NODISCARD auto max_size() const noexcept -> size_type { return m_meshes.max_size(); }

		ML_NODISCARD auto rbegin() noexcept -> reverse_iterator { return m_meshes.rbegin(); }

		ML_NODISCARD auto rbegin() const noexcept -> const_reverse_iterator { return m_meshes.rbegin(); }

		ML_NODISCARD auto rend() noexcept -> reverse_iterator { return m_meshes.rend(); }

		ML_NODISCARD auto rend() const noexcept -> const_reverse_iterator { return m_meshes.rend(); }

		ML_NODISCARD auto size() const noexcept -> size_type { return m_meshes.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static pmr::vector<vertex> load_model(fs::path const & path);

		static pmr::vector<vertex> load_model(fs::path const & path, int32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_meshes;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MODEL_HPP_