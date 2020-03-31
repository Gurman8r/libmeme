#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <libmeme/Renderer/Mesh.hpp>

namespace ml
{
	// collection of meshes
	struct ML_RENDERER_API model final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename mesh;
		using allocator_type			= typename pmr::polymorphic_allocator<byte_t>;
		using storage_type				= typename pmr::vector<value_type>;
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

		model();
		
		explicit model(allocator_type const & alloc);
		
		model(std::initializer_list<mesh> init, allocator_type const & alloc = {});
		
		model(filesystem::path const & path, allocator_type const & alloc = {});
		
		model(storage_type const & storage, allocator_type const & alloc = {});
		
		model(storage_type && storage, allocator_type const & alloc = {}) noexcept;
		
		model(model const & other, allocator_type const & alloc = {});
		
		model(model && other, allocator_type const & alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		model & operator=(model const & other);

		model & operator=(model && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(model & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(filesystem::path const & path);

		bool load_from_file(filesystem::path const & path, uint32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(render_target const & target, model const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto operator[](size_type const i) -> reference { return m_storage[i]; }

		ML_NODISCARD auto operator[](size_type const i) const -> const_reference { return m_storage[i]; }

		ML_NODISCARD auto back() noexcept -> reference { return m_storage.back(); }

		ML_NODISCARD auto back() const noexcept -> const_reference { return m_storage.back(); }

		ML_NODISCARD auto begin() noexcept -> iterator { return m_storage.begin(); }

		ML_NODISCARD auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		ML_NODISCARD auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		ML_NODISCARD auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		ML_NODISCARD auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		ML_NODISCARD auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		ML_NODISCARD auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		ML_NODISCARD auto data() noexcept -> pointer { return m_storage.data(); }

		ML_NODISCARD auto data() const noexcept -> const_pointer { return m_storage.data(); }

		ML_NODISCARD bool empty() const noexcept { return m_storage.empty(); }

		ML_NODISCARD auto end() noexcept -> iterator { return m_storage.end(); }

		ML_NODISCARD auto end() const noexcept -> const_iterator { return m_storage.end(); }

		ML_NODISCARD auto front() noexcept -> reference { return m_storage.front(); }

		ML_NODISCARD auto front() const noexcept -> const_reference { return m_storage.front(); }

		ML_NODISCARD auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		ML_NODISCARD auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		ML_NODISCARD auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_model(mesh && m)
	{
		return model{ std::initializer_list<mesh>{ ML_forward(m) } };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_model(Args && ... args)
	{
		return model{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_HPP_