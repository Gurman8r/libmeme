#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

// Credit to Vittorio Romeo
// https://github.com/SuperV1234/cppcon2015
// https://www.youtube.com/watch?v=NTWSeQtHZ9M

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/TypeList.hpp>
#include <libmeme/Core/StrongTypedef.hpp>
#include <libmeme/Core/EnumFlags.hpp>

// Typedefs
namespace ml::ecs
{
	ML_STRONG_TYPEDEF(size_t,	data_index);
	ML_STRONG_TYPEDEF(size_t,	entity_index);
	ML_STRONG_TYPEDEF(size_t,	handle_data_index);
	ML_STRONG_TYPEDEF(int32_t,	counter);
}

// Component Config
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... _Components
	> struct component_config
	{
		using type_list = typename meta::type_list<_Components...>;

		template <class T> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T> static constexpr size_t index() noexcept
		{
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Signature Config
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... _Signatures
	> struct signature_config
	{
		using type_list = typename meta::type_list<_Signatures...>;

		template <class T> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T> static constexpr size_t index() noexcept
		{
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// System Config
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class ... _Systems
	> struct system_config
	{
		using type_list = typename meta::type_list<_Systems...>;

		template <class T> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T> static constexpr size_t index() noexcept
		{
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Tag Config
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... _Tags
	> struct tag_config
	{
		using type_list = typename meta::type_list<_Tags...>;

		template <class T> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T> static constexpr size_t index() noexcept
		{
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Settings
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		template <class _Settings
		> struct signature_bitsets;

		template <class _Settings
		> struct signature_bitset_storage;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class _ComponentConfig	= component_config<>,	// 
		class _SignatureConfig	= signature_config<>,	// 
		class _SystemConfig		= system_config<>,		// 
		class _TagConfig		= tag_config<>			// 
	> struct settings
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using component_config	= typename _ComponentConfig;
		using signature_config	= typename _SignatureConfig;
		using system_config		= typename _SystemConfig;
		using tag_config		= typename _TagConfig;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using bitset_type = typename std::bitset<
			component_config::count() + tag_config::count()
		>;

		template <class T> static constexpr size_t component_bit() noexcept
		{
			return component_config::index<T>();
		}

		template <class T> static constexpr size_t tag_bit() noexcept
		{
			return component_config::count() + tag_config::index<T>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Signature Bitsets
namespace ml::ecs::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Settings
	> struct signature_bitsets
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using settings_type		= _Settings;
		using signature_list	= typename settings_type::signature_list;
		using bitset_type		= typename settings_type::bitset_type;

		using bitset_repeated_list = meta::repeat<
			settings_type::signature_count(),
			bitset_type
		>;

		using bitset_storage = meta::tuple<bitset_repeated_list>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using component_filter = std::integral_constant<
			bool, settings_type::component_config::template contains<T>()
		>;

		template <class T
		> using tag_filter = std::integral_constant<
			bool, settings_type::tag_config::template contains<T>()
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Signature
		> using signature_components = meta::filter<component_filter, Signature>;

		template <class Signature
		> using signature_tags = meta::filter<tag_filter, Signature>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Entity
namespace ml::ecs
{
	template <class _Settings
	> struct manager;

	enum class entity_flags
	{
		none,
		alive = (1 << 0),
	};

	ML_ENUM_FLAGS(int32_t, entity_flags);

	template <class _Settings
	> struct entity
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using settings_type	= typename _Settings;
		using self_type		= typename entity<settings_type>;
		using manager_type	= typename manager<settings_type>;
		using bitset_type	= typename settings_type::bitset_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Flag
		> ML_NODISCARD inline bool get_flag(Flag const flag) const noexcept
		{
			return static_cast<bool>(m_flags & flag);
		}

		template <class Flag
		> inline void set_flag(Flag const flag, bool value) noexcept
		{
			m_flags = value ? (m_flags | flag) : (m_flags & ~flag);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_manager, other.m_manager);
				std::swap(m_index, other.m_index);
				std::swap(m_bitset, other.m_bitset);
				std::swap(m_flags, other.m_flags);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend manager_type;

		entity(manager_type * m, entity_index i, bitset_type b, entity_flags f) noexcept
			: m_manager	{ m }
			, m_index	{ i }
			, m_bitset	{ b }
			, m_flags	{ f }
		{
		}

		entity() noexcept : self_type{
			nullptr, entity_index{}, bitset_type{}, entity_flags::none
		}
		{
		}

		entity(self_type const & other) : self_type{
			other.m_manager, other.m_index, other.m_bitset, other.m_flags
		}
		{
		}

		entity(self_type && other) noexcept : self_type{}
		{
			swap(std::move(other));
		}

		manager_type *	m_manager;
		entity_index	m_index;
		bitset_type		m_bitset;
		entity_flags	m_flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// Handle
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Settings
	> struct handle
	{
		using settings_type = typename _Settings;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Manager
namespace ml::ecs
{
	template <class _Settings
	> struct manager
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type			= typename pmr::polymorphic_allocator<byte_t>;
		using settings_type				= typename _Settings;
		using bitset_type				= typename settings_type::bitset_type;
		using self_type					= typename manager<_Settings>;
		using entity_type				= typename entity<_Settings>;
		using entity_storage			= typename pmr::vector<std::unique_ptr<entity_type>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit manager(allocator_type const & alloc)
			: m_size		{}
			, m_capacity	{}
			, m_size_next	{}
			, m_entities	{ alloc }
		{
		}

		manager(self_type const & other, allocator_type const & alloc = {})
			: m_size		{ other.m_size }
			, m_capacity	{ other.m_capacity }
			, m_size_next	{ other.m_size_next }
			, m_entities	{ other.m_entities, alloc }
		{
		}

		manager(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			swap(std::move(other));
		}

		manager() noexcept : self_type{ allocator_type{} } {}

		~manager() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_size, other.m_size);
				std::swap(m_capacity, other.m_capacity);
				std::swap(m_size_next, other.m_size_next);
				m_entities.swap(other.m_entities);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void grow_to(size_t const cap)
		{
			m_entities.resize(cap);
			for (size_t i = 0; i < cap; ++i)
			{
				auto & e{ m_entities.at(i) };
				e->m_index = static_cast<entity_index>(i);
				e->m_bitset.reset();
				e->set_flag(entity_flags::alive, false);
			}
			m_capacity = cap;
		}

		inline void grow_if_needed()
		{
			if (m_capacity > m_size_next) return;

			grow_to((m_capacity * 10) * 2);
		}

		inline auto create_index()
		{
			grow_if_needed();

			size_t i{ m_size_next++ };

			auto & e{ m_entities[i] };

			e->set_flag(entity_flags::alive, false);

			e->m_bitset.reset();
		}

		inline void clear() noexcept
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				auto & e{ m_entities[i] };
				e->m_index = {};
				e->m_bitset.reset();
				e->set_flag(entity_flags::alive, false);
			}
			m_size = m_size_next = 0;
		}

		inline void refresh() noexcept
		{
			if (m_size_next == 0) { m_size = 0; return; }

			m_size = m_size_next = static_cast<size_t>(([&]()
			{
				entity_index dead{ 0 }, alive{ m_size_next - 1 };
				while (true)
				{
					// find first dead entity from the left
					for (; true; ++dead)
					{
						if (dead > alive) return dead;
						if (!m_entities[dead]->get_flag(entity_flags::alive)) break;
					}

					// find first alive entity from the right
					for (; true; --alive)
					{
						if (m_entities[alive]->get_flag(entity_flags::alive)) break;
						if (alive <= dead) return dead;
					}

					ML_ASSERT(m_entities[alive]->get_flag(entity_flags::alive));

					ML_ASSERT(!m_entities[dead]->get_flag(entity_flags::alive));

					// arrange all alive entities towards the left
					std::swap(m_entities[alive], m_entities[dead]);

					// move both iterator indices
					++dead;
					--alive;
				}
				return dead;
			})());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline entity_type * get_entity(entity_index i)
		{
			return m_entities.at(i).get();
		}

		inline entity_type const * get_entity(entity_index i) const
		{
			return m_entities.at(i).get();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool get_flag(entity_index i, entity_flags f) const
		{
			return get_entity(i)->get_flag(f);
		}

		inline void set_flag(entity_index i, entity_flags f, bool b)
		{
			get_entity(i)->set_flag(f, b);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool is_alive(entity_index i) const
		{
			return get_flag(i, entity_flags::alive);
		}

		inline void kill(entity_index i)
		{
			set_flag(i, entity_flags::alive, false);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool has_tag(entity_index i) const noexcept
		{
			return get_entity(i)->m_bitset[settings_type::template tag_bit<T>()];
		}

		template <class T
		> inline void add_tag(entity_index i) noexcept
		{
			get_entity(i)->m_bitset[settings_type::template tag_bit<T>()] = true;
		}

		template <class T
		> inline void del_tag(entity_index i) noexcept
		{
			get_entity(i)->m_bitset[settings_type::template tag_bit<T>()] = false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool has_component(entity_index i) const noexcept
		{
			return get_entity(i)->m_bitset[settings_type::template component_bit<T>()];
		}

		template <class T
		> inline void add_component(entity_index i) noexcept
		{
			get_entity(i)->m_bitset[settings_type::template component_bit<T>()] = true;
		}

		template <class T
		> inline void del_component(entity_index i) noexcept
		{
			get_entity(i)->m_bitset[settings_type::template component_bit<T>()] = false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto size() const noexcept -> size_t { return m_size; }

		inline auto capacity() const noexcept -> size_t { return m_capacity; }

		inline auto size_next() const noexcept -> size_t { return m_size_next; }

		inline auto entities() const noexcept -> entity_storage const & { return m_entities; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t m_size;
		size_t m_capacity;
		size_t m_size_next;
		entity_storage m_entities;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_