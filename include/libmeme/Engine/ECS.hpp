#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

// Credit to Vittorio Romeo
// https://github.com/SuperV1234/cppcon2015
// https://www.youtube.com/watch?v=NTWSeQtHZ9M

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/BitSet.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Meta.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

// CONFIG
namespace ml::ecs::config
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// COMPONENTS
	template <class ... _Components
	> struct components final
	{
		using list = typename meta::list<_Components...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<list>();
		}

		template <class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, list>();
		}

		template <class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>(), "invalid component index");
			return meta::index_of<T, list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TAGS
	template <class ... _Tags
	> struct tags final
	{
		using list = typename meta::list<_Tags...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<list>();
		}

		template <class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, list>();
		}

		template <class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>(), "invalid tag index");
			return meta::index_of<T, list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// SIGNATURES
	template <class ... _Signatures
	> struct signatures final
	{
		using list = typename meta::list<_Signatures...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<list>();
		}

		template <class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, list>();
		}

		template <class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>(), "invalid signature index");
			return meta::index_of<T, list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// SYSTEM WRAPPER
	template <template <class> class _System
	> struct system_wrapper {};

	// SYSTEMS (WIP)
	template <template <class> class ... _Systems
	> struct systems final
	{
		using list = typename meta::list<system_wrapper<_Systems>...>;

		static constexpr size_t count() noexcept
		{
			return meta::size<list>();
		}

		template <template <class> class T
		> static constexpr bool contains() noexcept
		{
			return meta::contains<T, list>();
		}

		template <template <class> class T
		> static constexpr size_t index() noexcept
		{
			static_assert(contains<T>(), "invalid system index");
			return meta::index_of<T, list>();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// SETTINGS
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class _Components	= config::components<>,
		class _Tags			= config::tags<>,
		class _Signatures	= config::signatures<>,
		class _Systems		= config::systems<>
	> struct settings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using components		= typename _Components;
		using tags				= typename _Tags;
		using signatures		= typename _Signatures;
		using systems			= typename _Systems;
		using bitset			= typename ds::bitset<components::count() + tags::count()>;
		using bitset_storage	= typename meta::tuple<meta::repeat<signatures::count(), bitset>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using impl_component_filter = std::bool_constant<components::template contains<T>()>;

		template <class T
		> using component_filter = typename meta::filter<impl_component_filter, T>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using impl_tag_filter = std::bool_constant<tags::template contains<T>()>;

		template <class T
		> using tag_filter = typename meta::filter<impl_tag_filter, T>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr size_t component_bit() noexcept
		{
			return components::index<T>();
		}

		template <class T> static constexpr size_t tag_bit() noexcept
		{
			return components::count() + tags::index<T>();
		}

		template <class T> static constexpr bitset const & signature_mask() noexcept
		{
			return std::get<signatures::template index<T>()>(m_bitsets);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static constexpr bitset_storage generate_signature_bitset_storage() noexcept
		{
			// generate masks for each signature
			bitset_storage temp{};
			meta::for_types<typename signatures::list
			>([&temp](auto sig)
			{
				// get signature bitset
				auto & b{ std::get<signatures::template index<decltype(sig)::type>()>(temp) };

				// set component bits
				meta::for_types<component_filter<decltype(sig)::type>
				>([&b](auto cpt)
				{
					b.set<component_bit<decltype(cpt)::type>()>();
				});

				// set tag bits
				meta::for_types<tag_filter<decltype(sig)::type>
				>([&b](auto tag)
				{
					b.set<tag_bit<decltype(tag)::type>()>();
				});
			});
			return temp;
		}
		
		static constexpr auto m_bitsets{ generate_signature_bitset_storage() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// MANAGER
namespace ml::ecs
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Settings
	> struct manager : public trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct entity; struct handle;

		using settings_type		= typename _Settings;
		using self_type			= typename manager<settings_type>;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using entity_storage	= typename pmr::vector<entity>;
		using handle_storage	= typename pmr::vector<handle>;
		using component_storage = typename meta::tuple<
			meta::remap<pmr::vector, typename settings_type::components::list>
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// serves as a key for accessing real component/tag data
		struct entity final
		{
			using bitset = typename settings_type::bitset;

			bool	m_alive;	// state of entity (alive / dead)
			size_t	m_data;		// index of real component data
			size_t	m_handle;	// index of managing handle
			bitset	m_mask;		// component/tag signature bitsets
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// abstraction over entity interface
		struct handle final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD inline operator bool() const noexcept
			{
				return this->is_valid();
			}

			ML_NODISCARD inline bool is_valid() const noexcept
			{
				return m_manager && m_manager->is_valid_handle(*this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD inline bool is_alive() const noexcept
			{
				return m_manager->is_alive(*this);
			}

			inline void kill() noexcept
			{
				m_manager->kill(*this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class T> inline void add_tag() noexcept
			{
				m_manager->add_tag<T>(*this);
			}

			template <class T> inline void del_tag() noexcept
			{
				m_manager->del_tag<T>(*this);
			}

			template <class T> inline bool has_tag() noexcept
			{
				return m_manager->has_tag<T>(*this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class T, class ... Args
			> inline decltype(auto) add_component(Args && ... args) noexcept
			{
				return m_manager->add_component<T>(*this, ML_FWD(args)...);
			}

			template <class T> inline decltype(auto) add_component() noexcept
			{
				return m_manager->add_component<T>(*this);
			}

			template <class T> inline void del_component() noexcept
			{
				m_manager->del_component<T>(*this);
			}

			template <class T> ML_NODISCARD inline auto & get_component() noexcept
			{
				return m_manager->get_component<T>(*this);
			}

			template <class T> ML_NODISCARD inline auto const & get_component() const noexcept
			{
				return m_manager->get_component<T>(*this);
			}

			template <class T> ML_NODISCARD inline bool has_component() const noexcept
			{
				return m_manager->has_component<T>(*this);
			}

			template <class T> ML_NODISCARD inline bool matches_signature() const noexcept
			{
				return m_manager->matches_signature<T>(*this);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			
		private:
			friend self_type;

			self_type * m_manager;	// pointer to owning manager
			size_t		m_entity;	// index of real entity data
			size_t		m_self;		// index of real handle data
			int32_t		m_counter;	// validity check counter

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit manager(allocator_type const & alloc) noexcept
			: m_capacity	{ 0 }
			, m_size		{ 0 }
			, m_size_next	{ 0 }
			, m_entities	{ alloc }
			, m_handles		{ alloc }
			, m_components	{ std::allocator_arg, alloc }
		{
		}

		manager(self_type const & other, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->copy(other);
		}
		
		manager(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->swap(std::move(other));
		}

		manager(size_t const cap, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->grow_to(cap);
		}

		manager() noexcept : self_type{ 100, {} } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			this->swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
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
				e.m_mask.reset();
				e.m_alive = false;
				
				auto & h{ m_handles[i] };
				h.m_entity = i;
				h.m_counter = 0;
			}
			m_size = m_size_next = 0;
		}

		inline void copy(self_type const & other)
		{
			if (this != std::addressof(other))
			{
				m_size = other.m_size;
				m_capacity = other.m_capacity;
				m_size_next = other.m_size_next;
				m_entities = other.m_entities;
				m_handles = other.m_handles;
				m_components = other.m_components;
			}
		}

		inline void grow_to(size_t const cap)
		{
			if (cap <= m_capacity) return;

			m_entities.resize(cap);
			m_handles.resize(cap);
			meta::for_tuple([this, cap](auto & vec) { vec.resize(cap); }, m_components);
			
			for (size_t i = m_capacity; i < cap; ++i)
			{
				auto & e{ m_entities.at(i) };
				e.m_data = i;
				e.m_handle = i;
				e.m_mask.reset();
				e.m_alive = false;
				
				auto & h{ m_handles[i] };
				h.m_manager = this;
				h.m_entity = i;
				h.m_counter = 0;
			}
			m_capacity = cap;
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
						if (!(m_entities[dead].m_alive)) break;
					}

					// find first alive entity from the right
					for (; true; --alive)
					{
						if (m_entities[alive].m_alive) break;
						if (alive <= dead) return dead;
					}

					// found two entities that need to be swapped
					ML_ASSERT(m_entities[alive].m_alive);
					ML_ASSERT(!(m_entities[dead].m_alive));
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

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_size, other.m_size);
				std::swap(m_capacity, other.m_capacity);
				std::swap(m_size_next, other.m_size_next);
				m_entities.swap(other.m_entities);
				m_handles.swap(other.m_handles);
				m_components.swap(other.m_components);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto capacity() const noexcept -> size_t { return m_capacity; }

		inline auto size() const noexcept -> size_t { return m_size; }

		inline auto size_next() const noexcept -> size_t { return m_size_next; }

		inline auto entities() const noexcept -> entity_storage const & { return m_entities; }

		inline auto components() const noexcept -> component_storage const & { return m_components; }

		inline auto handles() const noexcept -> handle_storage const & { return m_handles; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_t create_entity()
		{
			// grow if needed
			if (m_capacity <= m_size_next)
			{
				this->grow_to((m_capacity * 10) * 2);
			}

			size_t i{ m_size_next++ };
			auto & e{ m_entities[i] };
			e.m_alive = true;
			e.m_mask.reset();
			return i;
		}

		ML_NODISCARD inline entity & get_entity(size_t const i)
		{
			return m_entities.at(i);
		}

		ML_NODISCARD inline entity & get_entity(handle const & h)
		{
			return this->get_entity(h.m_entity);
		}

		ML_NODISCARD inline entity const & get_entity(size_t const i) const
		{
			return m_entities.at(i);
		}
		
		ML_NODISCARD inline entity const & get_entity(handle const & h) const
		{
			return this->get_entity(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline handle create_handle()
		{
			size_t i{ create_entity() };
			auto & e{ m_entities[i] };
			auto & h{ m_handles[e.m_handle] };

			handle temp{};
			temp.m_manager	= h.m_manager;
			temp.m_entity	= h.m_entity = i;
			temp.m_self		= e.m_handle;
			temp.m_counter	= h.m_counter;

			ML_ASSERT(temp);
			return temp;
		}

		ML_NODISCARD inline bool is_valid_handle(handle const & h) const noexcept
		{
			return (h.m_counter == this->get_handle(h).m_counter);
		}

		ML_NODISCARD inline handle & get_handle(size_t const i)
		{
			return m_handles.at(i);
		}

		ML_NODISCARD inline handle const & get_handle(size_t const i) const
		{
			return m_handles.at(i);
		}

		ML_NODISCARD inline handle & get_handle(handle const & h)
		{
			return this->get_handle(h.m_self);
		}

		ML_NODISCARD inline handle const & get_handle(handle const & h) const
		{
			return this->get_handle(h.m_self);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool is_alive(size_t const i) const
		{
			return this->get_entity(i).m_alive;
		}

		ML_NODISCARD inline bool is_alive(handle const & h) const
		{
			return this->is_alive(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void kill(size_t const i)
		{
			return this->get_entity(i).m_alive = false;
		}

		inline void kill(handle const & h)
		{
			return this->kill(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline void add_tag(size_t const i) noexcept
		{
			this->get_entity(i).m_mask.set<settings_type::template tag_bit<T>()>();
		}

		template <class T
		> inline void add_tag(handle const & h) noexcept
		{
			return this->add_tag<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline void del_tag(size_t const i) noexcept
		{
			this->get_entity(i).m_mask.clear<settings_type::template tag_bit<T>()>();
		}

		template <class T
		> inline void del_tag(handle const & h) noexcept
		{
			return this->del_tag<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool has_tag(size_t const i) const noexcept
		{
			return this->get_entity(i).m_mask.read<settings_type::template tag_bit<T>()>();
		}

		template <class T
		> ML_NODISCARD inline bool has_tag(handle const & h) const noexcept
		{
			return this->has_tag(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args
		> inline decltype(auto) add_component(size_t const i, Args && ... args) noexcept
		{
			auto & e{ this->get_entity(i) };
			e.m_mask.set<settings_type::template component_bit<T>()>();

			auto & c{ std::get<pmr::vector<T>>(m_components)[e.m_data] };
			c = T{ ML_FWD(args)... };
			return c;
		}

		template <class T
		> inline decltype(auto) add_component(size_t const i) noexcept
		{
			return this->add_component<T>(i, T{});
		}

		template <class T, class ... Args
		> inline decltype(auto) add_component(handle const & h, Args && ... args) noexcept
		{
			return this->add_component<T>(h.m_entity, ML_FWD(args)...);
		}

		template <class T
		> inline decltype(auto) add_component(handle const & h) noexcept
		{
			return this->add_component<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline void del_component(size_t const i) noexcept
		{
			this->get_entity(i).m_mask.clear<settings_type::template component_bit<T>()>();
		}

		template <class T
		> inline void del_component(handle const & h) noexcept
		{
			this->del_component<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline auto & get_component(size_t const i) noexcept
		{
			return std::get<pmr::vector<T>>(m_components)[this->get_entity(i).m_data];
		}

		template <class T
		> ML_NODISCARD inline auto const & get_component(size_t const i) const noexcept
		{
			return std::get<pmr::vector<T>>(m_components)[this->get_entity(i).m_data];
		}

		template <class T
		> ML_NODISCARD inline auto & get_component(handle const & h) noexcept
		{
			return this->get_component<T>(h.m_entity);
		}

		template <class T
		> ML_NODISCARD inline auto const & get_component(handle const & h) const noexcept
		{
			return this->get_component<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool has_component(size_t const i) const noexcept
		{
			return this->get_entity(i).m_mask.read(settings_type::template component_bit<T>());
		}
		
		template <class T
		> ML_NODISCARD inline bool has_component(handle const & h) const noexcept
		{
			return this->has_component<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool matches_signature(size_t const i) const noexcept
		{
			auto const & e{ this->get_entity(i).m_mask };
			auto const & s{ settings_type::template signature_mask<T>() };
			return (s & e) == s;
		}

		template <class T
		> ML_NODISCARD inline bool matches_signature(handle const & h) const noexcept
		{
			return this->matches_signature<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type & for_entities(Fn && fn)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(fn, i);
			}
			return (*this);
		}

		template <class T, class Fn
		> inline self_type & for_matching(Fn && fn)
		{
			return this->for_entities([this, &fn](size_t const i)
			{
				if (this->matches_signature<T>(i))
				{
					this->expand_invoke<T>(i, fn);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		template <class ... Ts> struct invoke_helper;

		template <class T, class Fn
		> inline void expand_invoke(size_t const i, Fn && fn)
		{
			using req_comp = typename settings_type::template component_filter<T>;

			using helper = meta::rename<invoke_helper, req_comp>;

			helper::invoke(i, *this, fn);
		}

		template <class ... Ts> struct invoke_helper
		{
			template <class Fn
			> static inline void invoke(size_t const i, self_type & self, Fn && fn)
			{
				size_t const index{ self.get_entity(i).m_data };

				std::invoke(fn, i, std::get<pmr::vector<Ts>>(self.m_components)[index]...);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t m_capacity;	// storage capacity		(allocated memory)
		size_t m_size;		// current size			(excludes newly created entities)
		size_t m_size_next;	// size after refresh	(includes newly created entities)
		
		entity_storage		m_entities;		// entity storage
		handle_storage		m_handles;		// handle storage
		component_storage	m_components;	// component storage

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// TESTS
namespace ml::ecs::tests
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// components
	struct C0 {};
	struct C1 {};
	struct C2 {};
	struct C3 {};
	struct C4 {};

	// tags
	struct T0 {};
	struct T1 {};
	struct T2 {};

	// signatures
	using S0 = meta::list<>;				// 00000000
	using S1 = meta::list<C0, C1>;			// 11000000
	using S2 = meta::list<C0, C4, T0>;		// 10001100
	using S3 = meta::list<C1, T0, C3, T2>;	// 01010101

	// settings
	using ES = settings<
		config::components	<C0, C1, C2, C3, C4>,
		config::tags			<T0, T1, T2>,
		config::signatures	<S0, S1, S2, S3>
	>;

	// tests
	static_assert(ES::components::count() == 5);
	static_assert(ES::signatures::count() == 4);
	static_assert(ES::tags		::count() == 3);

	static_assert(ES::components::index<C0>() == 0);
	static_assert(ES::components::index<C1>() == 1);
	static_assert(ES::components::index<C2>() == 2);
	static_assert(ES::components::index<C3>() == 3);
	static_assert(ES::components::index<C4>() == 4);
	static_assert(ES::tags		::index<T0>() == 0);
	static_assert(ES::tags		::index<T1>() == 1);
	static_assert(ES::tags		::index<T2>() == 2);
	static_assert(ES::signatures::index<S0>() == 0);
	static_assert(ES::signatures::index<S1>() == 1);
	static_assert(ES::signatures::index<S2>() == 2);
	static_assert(ES::signatures::index<S3>() == 3);

	static_assert(ES::component_bit<C0>() == 0);
	static_assert(ES::component_bit<C1>() == 1);
	static_assert(ES::component_bit<C2>() == 2);
	static_assert(ES::component_bit<C3>() == 3);
	static_assert(ES::component_bit<C4>() == 4);
	static_assert(ES::		tag_bit<T0>() == 5);
	static_assert(ES::		tag_bit<T1>() == 6);
	static_assert(ES::		tag_bit<T2>() == 7);

	static_assert(ES::signature_mask<S0>() == "00000000");
	static_assert(ES::signature_mask<S1>() == "11000000");
	static_assert(ES::signature_mask<S2>() == "10001100");
	static_assert(ES::signature_mask<S3>() == "01010101");

	static_assert(std::is_same_v<ES::component_filter<S0>,
		meta::list<>
	>);
	static_assert(std::is_same_v<ES::component_filter<S3>,
		meta::list<C1, C3>
	>);
	static_assert(std::is_same_v<ES::tag_filter<S3>,
		meta::list<T0, T2>
	>);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_