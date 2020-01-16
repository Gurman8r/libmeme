#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <libmeme/Renderer/Mesh.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	// collection of meshes
	struct ML_RENDERER_API Model final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename Mesh;
		using storage_type				= typename std::vector<value_type>;
		using initializer_type			= typename std::initializer_list<value_type>;
		using allocator_type			= typename storage_type::allocator_type;
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

		Model();
		Model(initializer_type init);
		Model(path_t const & path);
		Model(storage_type const & storage);
		Model(storage_type && storage) noexcept;
		Model(Model const & other);
		Model(Model && other) noexcept;
		~Model();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Model & operator=(Model const & other);

		Model & operator=(Model && other) noexcept;

		void swap(Model & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		bool load_from_file(path_t const & path, uint32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw(RenderTarget const & target, Model const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto operator[](size_type const i) -> reference { return m_storage[i]; }

		ML_NODISCARD inline auto operator[](size_type const i) const -> const_reference { return m_storage[i]; }

		ML_NODISCARD inline auto back() noexcept -> reference { return m_storage.back(); }

		ML_NODISCARD inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		ML_NODISCARD inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		ML_NODISCARD inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		ML_NODISCARD inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		ML_NODISCARD inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		ML_NODISCARD inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		ML_NODISCARD inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		ML_NODISCARD inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		ML_NODISCARD inline auto data() noexcept -> pointer { return m_storage.data(); }

		ML_NODISCARD inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		ML_NODISCARD inline bool empty() const noexcept { return m_storage.empty(); }

		ML_NODISCARD inline auto end() noexcept -> iterator { return m_storage.end(); }

		ML_NODISCARD inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		ML_NODISCARD inline auto front() noexcept -> reference { return m_storage.front(); }

		ML_NODISCARD inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		ML_NODISCARD inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		ML_NODISCARD inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		ML_NODISCARD inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto make_model(Mesh && mesh)
	{
		return Model{ Model::initializer_type{ std::forward<Mesh>(mesh) } };
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_model(Args && ... args)
	{
		return Model{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_HPP_