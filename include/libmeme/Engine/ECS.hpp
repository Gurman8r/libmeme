#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

// Credit to Vittorio Romeo
// https://github.com/SuperV1234/cppcon2015
// https://www.youtube.com/watch?v=NTWSeQtHZ9M

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/BitSet.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/TypeList.hpp>
#include <libmeme/Core/StrongTypedef.hpp>
#include <libmeme/Core/EnumFlags.hpp>

namespace ml::ecs
{
	template <class T> struct entity_settings;
	template <class T> struct entity_manager;
	template <class T> struct signature_bitsets;
}

// Configuration
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Component Config
	template <class ... _Components
	> struct component_config
	{
		using type_list = typename meta::type_list<_Components...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		template <class T> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Signature Config
	template <class ... _Signatures
	> struct signature_config
	{
		using type_list = typename meta::type_list<_Signatures...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		template <class T> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// System Config
	template <template <class> class ... _Systems
	> struct system_config
	{
		using type_list = typename meta::type_list<_Systems...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <template <class> class S, class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<S<T>, type_list>();
		}

		template <template <class> class S, class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<S<T>, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Tag Config
	template <class ... _Tags
	> struct tag_config
	{
		using type_list = typename meta::type_list<_Tags...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		template <class T> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Entity Config
	template <
		class _ComponentConfig	= component_config<>,	// 
		class _SignatureConfig	= signature_config<>,	// 
		class _SystemConfig		= system_config<>,		// 
		class _TagConfig		= tag_config<>			// 
	> struct entity_config
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using self_type = typename entity_config<
			_ComponentConfig,
			_SignatureConfig,
			_SystemConfig,
			_TagConfig
		>;

		using components		= typename _ComponentConfig;
		using signatures		= typename _SignatureConfig;
		using systems			= typename _SystemConfig;
		using tags				= typename _TagConfig;
		using bitset			= typename ds::bitset<components::count() + tags::count()>;
		using bitset_storage	= typename meta::tuple<meta::repeat<signatures::count(), bitset>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr size_t component_bit() noexcept
		{
			static_assert(components::contains<T>());
			return components::index<T>();
		}

		template <class T> static constexpr size_t tag_bit() noexcept
		{
			static_assert(tags::contains<T>());
			return components::count() + tags::index<T>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using component_filter = std::bool_constant<components::template contains<T>()>;

		template <class T
		> using tag_filter = std::bool_constant<tags::template contains<T>()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using component_signatures = typename meta::filter<component_filter, T>;

		template <class T
		> using tag_signatures = typename meta::filter<tag_filter, T>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr bitset const & get_bitset() noexcept
		{
			return std::get<signatures::template index<T>()>(m_bitset_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static constexpr auto make_bitset_storage() noexcept
		{
			bitset_storage temp{};
			meta::for_types<typename signatures::type_list>([&temp](auto s)
			{
				auto & b{ std::get<signatures::template index<ML_TYPE(s)>()>(temp) };

				meta::for_types<component_signatures<ML_TYPE(s)>>([&b](auto c)
				{
					b.set(component_bit<ML_TYPE(c)>());
				});

				meta::for_types<tag_signatures<ML_TYPE(s)>>([&b](auto t)
				{
					b.set(tag_bit<ML_TYPE(t)>());
				});
			});
			return temp;
		}
		
		static constexpr bitset_storage m_bitset_storage
		{
			make_bitset_storage()
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Entity
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Config
	> struct entity
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using config = typename _Config;
		using bitset = typename config::bitset;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t	index;
		bitset	mask;
		int32_t	flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Component Storage
namespace ml::ecs
{
	template <class _Config
	> struct component_storage
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using config			= typename _Config;
		using self_type			= typename component_storage<_Config>;
		using component_list	= typename config::components::type_list;

		template <class ... Ts
		> using TupleOfVectors = std::tuple<std::vector<Ts>...>;

		using storage_type = typename meta::rename<TupleOfVectors, component_list>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		component_storage() noexcept
			: m_storage{}
		{
		}

		component_storage(self_type const & other)
			: m_storage{ other.m_storage }
		{
		}

		component_storage(self_type && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

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
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void grow(size_t const cap)
		{
			meta::for_tuple([this, cap](auto & v) { v.resize(cap); }, m_storage);
		}

		template <class Component
		> inline decltype(auto) get_component(size_t const i) noexcept
		{
			return std::get<std::vector<Component>>(m_storage)[i];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;
	};
}

// Handle
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Config
	> struct entity_handle
	{
		using config = typename _Config;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Entity Manager
namespace ml::ecs
{
	enum class entity_flags { none, alive = 1, };

	ML_ENUM_FLAGS(int32_t, entity_flags);

	template <class _Config
	> struct entity_manager
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using config			= typename _Config;
		using self_type			= typename entity_manager<_Config>;
		using entity_type		= typename entity<_Config>;
		using handle_type		= typename entity_handle<_Config>;
		using entity_storage	= typename pmr::vector<entity_type>;
		using component_storage = typename component_storage<_Config>;
		using handle_storage	= typename pmr::vector<handle_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit entity_manager(allocator_type const & alloc)
			: m_size		{}
			, m_capacity	{}
			, m_size_next	{}
			, m_entities	{ alloc }
			, m_components	{}
		{
		}

		entity_manager(self_type const & other, allocator_type const & alloc = {})
			: m_size		{ other.m_size }
			, m_capacity	{ other.m_capacity }
			, m_size_next	{ other.m_size_next }
			, m_entities	{ other.m_entities, alloc }
			, m_components	{ other.m_components }
		{
		}

		entity_manager(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			swap(std::move(other));
		}

		entity_manager() noexcept : self_type{ allocator_type{} } {}

		~entity_manager() noexcept {}

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
				m_components.swap(other.m_components);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void grow_to(size_t const cap)
		{
			m_entities.resize(cap);
			m_components.grow(cap);
			for (size_t i = 0; i < cap; ++i)
			{
				auto & e{ m_entities.at(i) };
				e.index = static_cast<size_t>(i);
				e.mask.reset();
				e.flags &= ~entity_flags::alive;
			}
			m_capacity = cap;
		}

		inline void grow_if_needed()
		{
			if (m_capacity > m_size_next) return;

			grow_to((m_capacity * 10) * 2);
		}

		inline size_t create_index()
		{
			grow_if_needed();
			size_t i{ m_size_next++ };
			auto & e{ m_entities[i] };
			e.flags |= entity_flags::alive;
			e.mask.reset();
			return i;
		}

		inline void clear() noexcept
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				auto & e{ m_entities[i] };
				e.index = {};
				e.mask.reset();
				e.flags &= ~entity_flags::alive;
			}
			m_size = m_size_next = 0;
		}

		inline void refresh() noexcept
		{
			if (m_size_next == 0)
			{
				m_size = 0; return;
			}
			else m_size = m_size_next = static_cast<size_t>(([&]()
			{
				// arrange all alive entities towards the left
				size_t dead{ 0 }, alive{ m_size_next - 1 };
				while (true)
				{
					// find first dead entity_type from the left
					for (; true; ++dead)
					{
						if (dead > alive) return dead;
						if (!m_entities[dead].flags & entity_flags::alive) break;
					}

					// find first alive entity_type from the right
					for (; true; --alive)
					{
						if (m_entities[alive].flags & entity_flags::alive) break;
						if (alive <= dead) return dead;
					}

					// found two entities that need to be swapped
					ML_ASSERT(m_entities[alive].flags & entity_flags::alive);
					ML_ASSERT(!m_entities[dead].flags & entity_flags::alive);
					std::swap(m_entities[alive], m_entities[dead]);

					// move both iterator indices
					++dead; --alive;
				}
				return dead;
			})());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline bool matches_signature(size_t i) const noexcept
		{
			auto const & e{ get_entity(i).mask };
			auto const & s{ config::template get_bitset<T>() };
			return (s & e) == s;
		}

		template <class Fn
		> inline void for_entities(Fn && fn)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(fn, i);
			}
		}

		template <class T, class Fn
		> inline void for_matching(Fn && fn)
		{
			for_entities([this, &fn](size_t i)
			{
				if (matches_signature<T>(i))
				{
					expand_invoke<T>(i, fn);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Ts
		> struct invoke_helper;

		template <class T, class Fn
		> inline void expand_invoke(size_t i, Fn && fn)
		{
			using req_comp = typename config::template component_signatures<T>;

			using helper = meta::rename<invoke_helper, req_comp>;

			helper::invoke(i, *this, fn);
		}

		template <class ... Ts
		> struct invoke_helper
		{
			template <class Fn
			> static inline void invoke(size_t const i, self_type & self, Fn && fn)
			{
				auto di{ self.get_entity(i).index };

				std::invoke(fn, i, self.m_components.template get_component<Ts>(di)...);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline entity_type & get_entity(size_t i)
		{
			return m_entities.at(i);
		}

		inline entity_type const & get_entity(size_t i) const
		{
			return m_entities.at(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool get_flag(size_t i, entity_flags f) const
		{
			return get_entity(i).flags & f;
		}

		inline void set_flag(size_t i, entity_flags f, bool b)
		{
			auto & e{ get_entity(i) };
			e.flags = b ? (e.flags | f) : (e.flags & ~f);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool is_alive(size_t i) const
		{
			return get_entity(i).flags & entity_flags::alive;
		}

		inline void kill(size_t i)
		{
			get_entity(i).flags &= ~entity_flags::alive;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args
		> inline auto add_component(size_t i, Args && ... args) noexcept
		{
			auto & e{ get_entity(i) };
			e.mask.set(config::template component_bit<T>());

			auto & c{ m_components.template get_component<T>(e.index) };
			c = T{ ML_FWD(args)... };
			return c;
		}

		template <class T
		> inline void del_component(size_t i) noexcept
		{
			get_entity(i).mask.clear(config::template component_bit<T>());
		}

		template <class T
		> ML_NODISCARD inline bool has_component(size_t i) const noexcept
		{
			return get_entity(i).mask.read(config::template component_bit<T>());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline void add_tag(size_t i) noexcept
		{
			get_entity(i).mask.set(config::template tag_bit<T>());
		}

		template <class T
		> inline void del_tag(size_t i) noexcept
		{
			get_entity(i).mask.clear(config::template tag_bit<T>());
		}

		template <class T
		> ML_NODISCARD inline bool has_tag(size_t i) const noexcept
		{
			return get_entity(i).mask.read(config::template tag_bit<T>());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto size() const noexcept -> size_t { return m_size; }

		inline auto capacity() const noexcept -> size_t { return m_capacity; }

		inline auto size_next() const noexcept -> size_t { return m_size_next; }

		inline auto entities() const noexcept -> entity_storage const & { return m_entities; }

		inline auto components() const noexcept -> component_storage const & { return m_components; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t m_size;
		size_t m_capacity;
		size_t m_size_next;
		entity_storage m_entities;
		component_storage m_components;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_