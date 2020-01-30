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
#include <libmeme/Core/MemoryTracker.hpp>

// CONFIGURATION
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// COMPONENT CONFIG
	template <class ... _Components
	> struct component_config
	{
		using type_list = typename meta::type_list<_Components...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		template <class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TAG CONFIG
	template <class ... _Tags
	> struct tag_config
	{
		using type_list = typename meta::type_list<_Tags...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		template <class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// SIGNATURE CONFIG
	template <class ... _Signatures
	> struct signature_config
	{
		using type_list = typename meta::type_list<_Signatures...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		template <class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// SYSTEM WRAPPER
	template <template <class> class _System
	> struct system_wrapper {};

	// SYSTEM CONFIG
	template <template <class> class ... _Systems
	> struct system_config
	{
		using type_list = typename meta::type_list<system_wrapper<_Systems>...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <template <class> class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, type_list>();
		}

		template <template <class> class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>());
			return meta::index_of<T, type_list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ENTITY SETTINGS
namespace ml::ecs
{
	template <
		class _ComponentConfig	= component_config<>,
		class _TagConfig		= tag_config<>,
		class _SignatureConfig	= signature_config<>,
		class _SystemConfig		= system_config<>
	> struct settings
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using components		= typename _ComponentConfig;
		using tags				= typename _TagConfig;
		using signatures		= typename _SignatureConfig;
		using systems			= typename _SystemConfig;
		using bitmask			= typename ds::bitset<components::count() + tags::count()>;
		using bitmask_storage	= typename meta::tuple<meta::repeat<signatures::count(), bitmask>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using component_filter = std::bool_constant<components::template contains<T>()>;

		template <class T
		> using component_signatures = typename meta::filter<component_filter, T>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using tag_filter = std::bool_constant<tags::template contains<T>()>;

		template <class T
		> using tag_signatures = typename meta::filter<tag_filter, T>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr size_t component_bit() noexcept
		{
			return components::index<T>();
		}

		template <class T> static constexpr size_t tag_bit() noexcept
		{
			return components::count() + tags::index<T>();
		}

		template <class T> static constexpr bitmask const & get_bitmask() noexcept
		{
			return std::get<signatures::template index<T>()>(m_bitmasks);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static constexpr bitmask_storage generate_bitmasks() noexcept
		{
			bitmask_storage temp{};
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
		
		static constexpr bitmask_storage m_bitmasks{ generate_bitmasks() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// ENTITY MANAGER
namespace ml::ecs
{
	template <class _Settings
	> struct manager : public trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using settings			= typename _Settings;
		using self_type			= typename manager<settings>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct entity final
		{
			using bitmask = typename settings::bitmask;

			enum : int32_t { none, alive = 1, };

		private:
			friend self_type;
			friend struct handle;

			size_t	m_data;		// index of component data
			size_t	m_handle;	// index of handle data
			bitmask	m_bitmask;	// signature bitsets
			int32_t	m_flags;	// entity flags
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct handle final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD inline operator bool() const noexcept
			{
				return (m_counter == m_owner->get_handle(m_handle).m_counter);
			}

			ML_NODISCARD inline entity & operator*() noexcept
			{
				return m_owner->get_entity(*this);
			}

			ML_NODISCARD inline entity const & operator*() const noexcept
			{
				return m_owner->get_entity(*this);
			}

			ML_NODISCARD inline entity * operator->() noexcept
			{
				return &(**this);
			}

			ML_NODISCARD inline entity const * operator->() const noexcept
			{
				return &(**this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD inline size_t data() const noexcept
			{
				return (**this).m_data;
			}

			ML_NODISCARD inline auto const & bitmask() const noexcept
			{
				return (**this).m_bitmask;
			}

			ML_NODISCARD inline int32_t flags() const noexcept
			{
				return (**this).m_flags;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD inline bool get_flag(int32_t const f) const noexcept
			{
				return m_owner->get_flag(*this, f);
			}

			inline void set_flag(int32_t const f, bool const b) noexcept
			{
				m_owner->set_flag(*this, f, b);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD inline bool is_alive() const noexcept
			{
				return m_owner->is_alive(*this);
			}

			inline void kill() noexcept
			{
				m_owner->kill(*this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class T> inline void add_tag() noexcept
			{
				m_owner->add_tag<T>(*this);
			}

			template <class T> inline void del_tag() noexcept
			{
				m_owner->del_tag<T>(*this);
			}

			template <class T> inline bool has_tag() noexcept
			{
				return m_owner->has_tag<T>(*this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class T, class ... Args> inline decltype(auto) add_component(Args && ... args) noexcept
			{
				return m_owner->add_component<T>(*this, ML_FWD(args)...);
			}

			template <class T> inline decltype(auto) add_component() noexcept
			{
				return m_owner->add_component<T>(*this);
			}

			template <class T> inline void del_component() noexcept
			{
				m_owner->del_component<T>(*this);
			}

			template <class T> ML_NODISCARD inline auto & get_component() noexcept
			{
				return m_owner->get_component<T>(*this);
			}

			template <class T> ML_NODISCARD inline auto const & get_component() const noexcept
			{
				return m_owner->get_component<T>(*this);
			}

			template <class T> ML_NODISCARD inline bool has_component() const noexcept
			{
				return m_owner->has_component<T>(*this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			
		private:
			friend self_type;

			self_type * m_owner;	// owning manager
			size_t		m_entity;	// index of entity stored in owner
			size_t		m_handle;	// index of handle stored in owner
			int32_t		m_counter;	// validity check counter

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using entity_storage	= typename pmr::vector<entity>;
		using handle_storage	= typename pmr::vector<handle>;
		using component_storage = typename meta::tuple<
			meta::remap<std::vector, typename settings::components::type_list>
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		manager() noexcept
			: self_type{ allocator_type{} }
		{
		}

		explicit manager(allocator_type const & alloc)
			: m_size		{}
			, m_capacity	{}
			, m_size_next	{}
			, m_entities	{ alloc }
			, m_components	{}
			, m_handles		{ alloc }
		{
		}

		manager(self_type const & other, allocator_type const & alloc = {})
			: m_size		{ other.m_size }
			, m_capacity	{ other.m_capacity }
			, m_size_next	{ other.m_size_next }
			, m_entities	{ other.m_entities, alloc }
			, m_components	{ other.m_components }
			, m_handles		{ other.m_handles, alloc }
		{
		}

		manager(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			swap(std::move(other));
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				auto & e{ m_entities[i] };
				e.m_data = i;
				e.m_handle = i;
				e.m_bitmask.reset();
				e.m_flags = entity::none;
				
				auto & h{ m_handles[i] };
				h.m_entity = i;
				h.m_counter = 0;
			}
			m_size = m_size_next = 0;
		}

		inline void refresh() noexcept
		{
			if (m_size_next == 0)
			{
				m_size = 0;
				return;
			}
			else m_size = m_size_next = static_cast<size_t>(([&]()
			{
				// arrange all alive entities towards the left
				size_t dead{ 0 }, alive{ m_size_next - 1 };
				while (true)
				{
					// find first dead entity from the left
					for (; true; ++dead)
					{
						if (dead > alive) return dead;
						if (!(m_entities[dead].m_flags & entity::alive)) break;
					}

					// find first alive entity from the right
					for (; true; --alive)
					{
						if (m_entities[alive].m_flags & entity::alive) break;
						if (alive <= dead) return dead;
					}

					// found two entities that need to be swapped
					ML_ASSERT(m_entities[alive].m_flags & entity::alive);
					ML_ASSERT(!(m_entities[dead].m_flags & entity::alive));
					std::swap(m_entities[alive], m_entities[dead]);

					auto invalidate_handle = [this](size_t const i) noexcept
					{
						auto & h{ m_handles[m_entities[i].m_handle] };
						++h.m_counter;
					};

					auto refresh_handle = [this](size_t const i) noexcept
					{
						auto & h{ m_handles[m_entities[i].m_handle] };
						h.m_entity = i;
					};

					refresh_handle(dead);

					invalidate_handle(alive);
					refresh_handle(alive);

					// move both iterator indices
					++dead; --alive;
				}
				return dead;
			})());
		}

		inline void resize(size_t const cap)
		{
			m_entities.resize(cap);
			meta::for_tuple([this, cap](auto & vec) { vec.resize(cap); }, m_components);
			m_handles.resize(cap);
			for (size_t i = m_capacity; i < cap; ++i)
			{
				auto & e{ m_entities.at(i) };
				e.m_data = i;
				e.m_handle = i;
				e.m_bitmask.reset();
				e.m_flags = entity::none;
				
				auto & h{ m_handles[i] };
				h.m_owner = this;
				h.m_entity = i;
				h.m_counter = 0;
			}
			m_capacity = cap;
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
				m_handles.swap(other.m_handles);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto size() const noexcept -> size_t { return m_size; }

		inline auto capacity() const noexcept -> size_t { return m_capacity; }

		inline auto size_next() const noexcept -> size_t { return m_size_next; }

		inline auto entities() const noexcept -> entity_storage const & { return m_entities; }

		inline auto components() const noexcept -> component_storage const & { return m_components; }

		inline auto handles() const noexcept -> handle_storage const & { return m_handles; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_t create_entity()
		{
			if (m_capacity <= m_size_next) { resize((m_capacity * 10) * 2); }
			size_t i{ m_size_next++ };
			auto & e{ m_entities[i] };
			e.m_flags |= entity::alive;
			e.m_bitmask.reset();
			return i;
		}

		ML_NODISCARD inline entity & get_entity(size_t const i)
		{
			return m_entities.at(i);
		}

		ML_NODISCARD inline entity & get_entity(handle const & h)
		{
			return get_entity(h.m_entity);
		}

		ML_NODISCARD inline entity const & get_entity(size_t const i) const
		{
			return m_entities.at(i);
		}
		
		ML_NODISCARD inline entity const & get_entity(handle const & h) const
		{
			return get_entity(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline handle create_handle()
		{
			size_t i{ create_entity() };
			auto & e{ m_entities[i] };
			auto & h{ m_handles[e.m_handle] };

			handle temp{};
			temp.m_owner		= h.m_owner;
			temp.m_entity	= h.m_entity = i;
			temp.m_handle	= e.m_handle;
			temp.m_counter	= h.m_counter;
			return temp;
		}

		ML_NODISCARD inline handle & get_handle(size_t const i)
		{
			return m_handles.at(i);
		}

		ML_NODISCARD inline handle const & get_handle(size_t const i) const
		{
			return m_handles.at(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool get_flag(size_t const i, int32_t const f) const
		{
			return get_entity(i).m_flags & f;
		}

		ML_NODISCARD inline bool get_flag(handle const & h, int32_t const f) const
		{
			return get_flag(h.m_entity, f);
		}

		inline void set_flag(size_t const i, int32_t const f, bool const b)
		{
			auto & e{ get_entity(i) };
			e.m_flags = b ? (e.m_flags | f) : (e.m_flags & ~f);
		}

		inline void set_flag(handle const & h, int32_t const f, bool const b)
		{
			return set_flag(h.m_entity, f, b);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool is_alive(size_t const i) const
		{
			return get_flag(i, entity::alive);
		}

		ML_NODISCARD inline bool is_alive(handle const & h) const
		{
			return is_alive(h.m_entity);
		}

		inline void kill(size_t const i)
		{
			return set_flag(i, entity::alive, false);
		}

		inline void kill(handle const & h)
		{
			return kill(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline void add_tag(size_t const i) noexcept
		{
			get_entity(i).m_bitmask.set(settings::template tag_bit<T>());
		}

		template <class T
		> inline void add_tag(handle const & h) noexcept
		{
			return add_tag<T>(h.m_entity);
		}

		template <class T
		> inline void del_tag(size_t const i) noexcept
		{
			get_entity(i).m_bitmask.clear(settings::template tag_bit<T>());
		}

		template <class T
		> inline void del_tag(handle const & h) noexcept
		{
			return del_tag<T>(h.m_entity);
		}

		template <class T
		> ML_NODISCARD inline bool has_tag(size_t const i) const noexcept
		{
			return get_entity(i).m_bitmask.read(settings::template tag_bit<T>());
		}

		template <class T
		> ML_NODISCARD inline bool has_tag(handle const & h) const noexcept
		{
			return has_tag(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args
		> inline decltype(auto) add_component(size_t const i, Args && ... args) noexcept
		{
			auto & e{ get_entity(i) };
			e.m_bitmask.set(settings::template component_bit<T>());

			auto & c{ std::get<std::vector<T>>(m_components)[e.m_data] };
			c = T{ ML_FWD(args)... };
			return c;
		}

		template <class T
		> inline decltype(auto) add_component(size_t const i) noexcept
		{
			return add_component<T>(i, T{});
		}

		template <class T, class ... Args
		> inline decltype(auto) add_component(handle const & h, Args && ... args) noexcept
		{
			return add_component<T>(h.m_entity, ML_FWD(args)...);
		}

		template <class T
		> inline decltype(auto) add_component(handle const & h) noexcept
		{
			return add_component<T>(h.m_entity);
		}

		template <class T
		> inline void del_component(size_t const i) noexcept
		{
			get_entity(i).m_bitmask.clear(settings::template component_bit<T>());
		}

		template <class T
		> inline void del_component(handle const & h) noexcept
		{
			del_component<T>(h.m_entity);
		}

		template <class T
		> ML_NODISCARD inline auto & get_component(size_t const i) noexcept
		{
			return std::get<std::vector<T>>(m_components)[get_entity(i).m_data];
		}

		template <class T
		> ML_NODISCARD inline auto const & get_component(size_t const i) const noexcept
		{
			return std::get<std::vector<T>>(m_components)[get_entity(i).m_data];
		}

		template <class T
		> ML_NODISCARD inline auto & get_component(handle const & h) noexcept
		{
			return get_component<T>(h.m_entity);
		}

		template <class T
		> ML_NODISCARD inline auto const & get_component(handle const & h) const noexcept
		{
			return get_component<T>(h.m_entity);
		}

		template <class T
		> ML_NODISCARD inline bool has_component(size_t const i) const noexcept
		{
			return get_entity(i).m_bitmask.read(settings::template component_bit<T>());
		}
		
		template <class T
		> ML_NODISCARD inline bool has_component(handle const & h) const noexcept
		{
			return has_component<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool matches_signature(size_t const i) const noexcept
		{
			auto const & e{ get_entity(i).m_bitmask };
			auto const & s{ settings::template get_bitmask<T>() };
			return (s & e) == s;
		}

		template <class Fn
		> inline self_type & for_i(Fn && fn)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(fn, i);
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_entities(Fn && fn)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(fn, i, m_entities[i]);
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_handles(Fn && fn)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(fn, i, m_handles[i]);
			}
			return (*this);
		}

		template <class T, class Fn
		> inline self_type & for_matching(Fn && fn)
		{
			return for_i([this, &fn](size_t const i)
			{
				if (matches_signature<T>(i))
				{
					expand_invoke<T>(i, fn);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		template <class ... Ts> struct invoke_helper;

		template <class T, class Fn
		> inline void expand_invoke(size_t const i, Fn && fn)
		{
			using req_comp = typename settings::template component_signatures<T>;

			using helper = meta::rename<invoke_helper, req_comp>;

			helper::invoke(i, *this, fn);
		}

		template <class ... Ts> struct invoke_helper
		{
			template <class Fn
			> static inline void invoke(size_t const i, self_type & self, Fn && fn)
			{
				size_t di{ self.get_entity(i).m_data };

				std::invoke(fn, i, std::get<std::vector<Ts>>(self.m_components)[di]...);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t m_size, m_capacity, m_size_next;
		
		entity_storage		m_entities;
		component_storage	m_components;
		handle_storage		m_handles;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ECS_HPP_