#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

// Data Oriented ECS
// Credit to Vittorio Romeo
// https://github.com/SuperV1234/cppcon2015
// https://www.youtube.com/watch?v=NTWSeQtHZ9M

#include <libmeme/Core/BitSet.hpp>
#include <libmeme/Core/MultiVector.hpp>

// ECS
namespace ml::ecs
{
	template <class
	> struct manager;
}

// UTILITY
namespace ml::ecs::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base system helper
	template <class Signature
	> struct x_base
	{
		using signature = typename Signature;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// used to store "template template" systems in type lists
	template <template <class> class System
	> struct x_wrapper final {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// (T) TAGS
namespace ml::ecs::cfg
{
	template <class ... Tags
	> struct tags final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename tags<Tags...>;

		using type_list = typename meta::list<Tags...>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			static_assert(self_type::contains<T>(), "tag not found");
			return meta::index_of<T, type_list>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using impl_filter = std::bool_constant<self_type::contains<T>()>;

		template <class T
		> using filter = typename meta::filter<impl_filter, T>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// (C) COMPONENTS
namespace ml::ecs::cfg
{
	template <class ... Components
	> struct components final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename components<Components...>;

		using type_list = typename meta::list<Components...>;

		using storage_type = typename ds::multi_vector<Components...>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			static_assert(self_type::contains<T>(), "component not found");
			return meta::index_of<T, type_list>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using impl_filter = std::bool_constant<self_type::contains<T>()>;

		template <class T
		> using filter = typename meta::filter<impl_filter, T>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// (S) SIGNATURES
namespace ml::ecs::cfg
{
	template <class ... Signatures
	> struct signatures final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename signatures<Signatures...>;

		using type_list = typename meta::list<Signatures...>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			static_assert(self_type::contains<T>(), "signature not found");
			return meta::index_of<T, type_list>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// (X) SYSTEMS
namespace ml::ecs::cfg
{
	template <template <class> class ... Systems
	> struct systems final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename systems<Systems...>;

		using type_list = typename meta::list<detail::template x_wrapper<Systems>...>;

		template <class U
		> using storage_type = typename meta::tuple<meta::list<Systems<U>...>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <template <class> class X
		> static constexpr bool contains() noexcept
		{
			return meta::contains<detail::x_wrapper<X>, type_list>();
		}

		template <template <class> class X
		> static constexpr size_t index() noexcept
		{
			static_assert(self_type::contains<X>(), "system not found");
			return meta::index_of<detail::x_wrapper<X>, type_list>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// (O) OPTIONS
namespace ml::ecs::cfg
{
	template <
		size_t	GrowBase = 5,
		class	GrowMult = std::ratio<2, 1>
	> struct options final
	{
		// growth base amount
		static constexpr size_t grow_base{ GrowBase };
		static_assert(0 < grow_base, "growth base negative or zero");

		// growth multiplier
		static constexpr float_t grow_mult{ util::ratio_cast(1.f, GrowMult{}) };
		static_assert(1.f <= grow_mult, "expression would result in negative growth");

		static constexpr size_t calc_growth(size_t const cap) noexcept
		{
			return (size_t)((float_t)(cap + grow_base) * grow_mult);
		}
	};
}

// (U) TRAITS
namespace ml::ecs
{
	template <
		class T = cfg::tags			<>,	// Tags
		class C = cfg::components	<>,	// Components
		class S = cfg::signatures	<>,	// Signatures
		class X = cfg::systems		<>,	// Systems
		class O = cfg::options		<>	// Options
	> struct traits final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using self_type		= typename traits<T, C, S, X, O>;
		using tags			= typename T;
		using components	= typename C;
		using signatures	= typename S;
		using systems		= typename X;
		using options		= typename O;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t tag_count		{ tags::count() };
		static constexpr size_t component_count	{ components::count() };
		static constexpr size_t signature_count	{ signatures::count() };
		static constexpr size_t system_count	{ systems::count() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using component_list	= typename components::type_list;
		using tag_list			= typename tags::type_list;
		using signature_list	= typename signatures::type_list;
		using system_list		= typename systems::type_list;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using component_storage = typename components::storage_type;
		using system_storage	= typename systems::template storage_type<self_type>;
		using signature			= typename ds::bitset<component_count + tag_count>;
		using signature_storage	= typename meta::array<signature, signature_count>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> static constexpr bool has_component() noexcept
		{
			return components::template contains<C>();
		}

		template <class C
		> static constexpr size_t component_id() noexcept
		{
			return components::template index<C>();
		}

		template <class C
		> static constexpr size_t component_bit() noexcept
		{
			return self_type::component_id<C>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> static constexpr bool has_tag() noexcept
		{
			return tags::template contains<T>();
		}

		template <class T
		> static constexpr size_t tag_id() noexcept
		{
			return tags::template index<T>();
		}

		template <class T
		> static constexpr size_t tag_bit() noexcept
		{
			return component_count + self_type::tag_id<T>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class S
		> static constexpr bool has_signature() noexcept
		{
			return signatures::template contains<S>();
		}

		template <class S
		> static constexpr size_t signature_id() noexcept
		{
			return signatures::template index<S>();
		}

		template <class S
		> static constexpr signature const & signature_bitset() noexcept
		{
			return std::get<self_type::signature_id<S>()>(m_signature_bitsets);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <template <class> class X
		> static constexpr bool has_system() noexcept
		{
			return systems::template contains<X>();
		}

		template <template <class> class X
		> static constexpr size_t system_id() noexcept
		{
			return systems::template index<X>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static constexpr signature_storage gen_signature_bitsets() noexcept
		{
			// generate bitsets for each signature
			signature_storage temp{};
			meta::for_types<typename signatures::type_list
			>([&temp](auto s)
			{
				// get the signature's bitset
				auto & b{ std::get<self_type::signature_id<decltype(s)::type>()>(temp) };

				// enable component bits
				meta::for_types<components::template filter<decltype(s)::type>
				>([&b](auto c)
				{
					b.set<self_type::component_bit<decltype(c)::type>()>();
				});

				// enable tag bits
				meta::for_types<tags::template filter<decltype(s)::type>
				>([&b](auto t)
				{
					b.set<self_type::tag_bit<decltype(t)::type>()>();
				});
			});
			return temp;
		}

		static constexpr signature_storage m_signature_bitsets{ gen_signature_bitsets() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// (H) HANDLE
namespace ml::ecs::impl
{
	// abstraction over entity interface
	template <class M = manager<>
	> struct handle final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool valid() const noexcept
		{
			return m_manager && m_manager->is_valid_handle(*this);
		}

		ML_NODISCARD operator bool() const noexcept
		{
			return this->valid(); // true if valid
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool alive() const noexcept
		{
			ML_assert(m_manager);
			return m_manager->is_alive(*this);
		}

		handle & kill() noexcept
		{
			ML_assert(m_manager);
			m_manager->kill(*this);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> handle & add_tag() noexcept
		{
			ML_assert(m_manager);
			m_manager->template add_tag<T>(*this);
			return (*this);
		}

		template <class T
		> handle & del_tag() noexcept
		{
			ML_assert(m_manager);
			m_manager->template del_tag<T>(*this);
			return (*this);
		}

		template <class T
		> bool has_tag() noexcept
		{
			ML_assert(m_manager);
			return m_manager->template has_tag<T>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C, class ... Args
		> auto & add_component(Args && ... args) noexcept
		{
			ML_assert(m_manager);
			return m_manager->template add_component<C>(*this, ML_forward(args)...);
		}

		template <class C
		> auto & add_component() noexcept
		{
			ML_assert(m_manager);
			return m_manager->template add_component<C>(*this);
		}

		template <class C
		> handle & del_component() noexcept
		{
			ML_assert(m_manager);
			m_manager->template del_component<C>(*this);
			return (*this);
		}

		template <class C
		> ML_NODISCARD auto & get_component() noexcept
		{
			ML_assert(m_manager);
			return m_manager->template get_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD auto const & get_component() const noexcept
		{
			ML_assert(m_manager);
			return m_manager->template get_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD bool has_component() const noexcept
		{
			ML_assert(m_manager);
			return m_manager->template has_component<C>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> ML_NODISCARD bool matches_signature() const noexcept
		{
			ML_assert(m_manager);
			return m_manager->template matches_signature<C>(*this);
		}

		template <template <class> class X
		> ML_NODISCARD bool matches_system() const noexcept
		{
			ML_assert(m_manager);
			return m_manager->template matches_system<X>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend M;

		M *		m_manager;	// pointer to owning manager
		size_t	m_entity;	// index of real entity data
		size_t	m_self;		// index of real handle data
		int32_t	m_counter;	// validity check counter

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// (M) MANAGER
namespace ml::ecs
{
	template <class U = traits<>
	> struct manager final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using traits_type		= typename U;
		using self_type			= typename manager<traits_type>;
		using options_type		= typename traits_type::options;
		using signature			= typename traits_type::signature;
		using component_storage	= typename traits_type::component_storage;
		using system_storage	= typename traits_type::system_storage;
		using handle			= typename impl::handle<self_type>;
		using handle_storage	= typename pmr::vector<handle>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { id_alive, id_index, id_handle, id_bitset };

		using entity_storage = typename ds::multi_vector<
			bool,	  // state of entity (alive / dead)
			size_t,	  // index of real component data
			size_t,	  // index of managing handle
			signature // entity signature bitset
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		manager(allocator_type const & alloc = {}) noexcept
			: m_capacity	{}
			, m_size		{}
			, m_size_next	{}
			, m_entities	{ alloc }
			, m_components	{ alloc }
			, m_handles		{ alloc }
			, m_systems		{}
		{
		}

		manager(self_type const & value, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->deep_copy(value);
		}
		
		manager(self_type && value, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->swap(std::move(value));
		}

		manager(size_t const cap, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->grow_to(cap);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(self_type const & value)
		{
			this->deep_copy(value);
			return (*this);
		}

		self_type & operator=(self_type && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto capacity() const noexcept -> size_t { return m_capacity; }

		auto size() const noexcept -> size_t { return m_size; }

		auto size_next() const noexcept -> size_t { return m_size_next; }

		auto components() const noexcept -> component_storage const & { return m_components; }

		auto entities() const noexcept -> entity_storage const & { return m_entities; }

		auto handles() const noexcept -> handle_storage const & { return m_handles; }

		auto systems() const noexcept -> system_storage const & { return m_systems; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				m_entities.expand_all(i, [&](auto & a, auto & e, auto & h, auto & b)
				{
					a = false;	// alive
					e = i;		// index
					h = i;		// handle
					b.reset();	// bitset
				});
				
				auto & h{ m_handles[i] };
				h.m_entity = i;
				h.m_counter = 0;
			}
			m_size = m_size_next = 0;
		}

		void deep_copy(self_type const & value)
		{
			if (this != std::addressof(value))
			{
				m_capacity	= value.m_capacity;
				m_size		= value.m_size;
				m_size_next = value.m_size_next;
				m_components= value.m_components;
				m_entities	= value.m_entities;
				m_handles	= value.m_handles;
				m_systems	= value.m_systems;
			}
		}

		void grow_to(size_t const cap)
		{
			if (cap <= m_capacity) { return; }

			m_components.resize(cap);
			m_handles.resize(cap);
			m_entities.reserve(cap);

			for (size_t i = m_capacity; i < cap; ++i)
			{
				m_entities.push_back(false, i, i, signature{});

				auto & h{ m_handles[i] };
				h.m_manager = this;
				h.m_entity = i;
				h.m_counter = 0;
			}
			m_capacity = cap;
		}

		void refresh() noexcept
		{
			if (m_size_next == 0)
			{
				m_size = 0;
			}
			else m_size = m_size_next = ([&]()
			{
				// arrange all alive entities towards the left
				size_t dead{}, alive{ m_size_next - 1 };
				while (true)
				{
					// find dead entity from the left
					for (; true; ++dead)
					{
						if (dead > alive) { return dead; }
						if (!m_entities.get<id_alive>(dead)) { break; }
					}

					// find alive entity from the right
					for (; true; --alive)
					{
						if (m_entities.get<id_alive>(alive)) { break; }
						if (alive <= dead) { return dead; }
					}

					// found two entities that need to be swapped
					ML_assert(m_entities.get<id_alive>(alive));
					ML_assert(!m_entities.get<id_alive>(dead));
					
					// swap the entities
					m_entities.swap(alive, dead);

					// refresh alive entity
					auto & a{ m_handles[alive] };
					a.m_entity = alive;

					// invalidate and refresh dead entity
					auto & d{ m_handles[dead] };
					++d.m_counter;
					d.m_entity = dead;

					// move both iterator indices
					++dead; --alive;
				}
				return dead;
			})();
		}

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_capacity,	value.m_capacity);
				std::swap(m_size,		value.m_size);
				std::swap(m_size_next,	value.m_size_next);

				m_components.swap(value.m_components);
				m_entities	.swap(value.m_entities);
				m_handles	.swap(value.m_handles);
				m_systems	.swap(value.m_systems);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD size_t create_entity()
		{
			// grow if needed
			if (m_capacity <= m_size_next)
			{
				this->grow_to(traits_type::options::calc_growth(m_capacity));
			}

			size_t const i{ m_size_next++ };
			m_entities.get<id_alive>(i) = true;
			m_entities.get<id_bitset>(i).reset();
			return i;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_alive(size_t const i) const
		{
			return m_entities.get<id_alive>(i);
		}

		ML_NODISCARD bool is_alive(handle const & h) const
		{
			return this->is_alive(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & kill(size_t const i)
		{
			m_entities.get<id_alive>(i) = false;
			return (*this);
		}

		self_type & kill(handle const & h)
		{
			return this->kill(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD handle create_handle()
		{
			size_t const i{ this->create_entity() };
			size_t const e{ m_entities.get<id_handle>(i) };
			auto & h{ m_handles[e] };

			handle temp{};
			temp.m_manager	= h.m_manager;
			temp.m_entity	= h.m_entity = i;
			temp.m_self		= e;
			temp.m_counter	= h.m_counter;
			ML_assert(temp);
			return temp;
		}

		ML_NODISCARD bool is_valid_handle(handle const & h) const noexcept
		{
			return (h.m_counter == this->get_handle(h).m_counter);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD handle & get_handle(size_t const i)
		{
			return m_handles[i];
		}

		ML_NODISCARD handle const & get_handle(size_t const i) const
		{
			return m_handles[i];
		}

		ML_NODISCARD handle & get_handle(handle const & h)
		{
			return this->get_handle(h.m_self);
		}

		ML_NODISCARD handle const & get_handle(handle const & h) const
		{
			return this->get_handle(h.m_self);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> self_type & add_tag(size_t const i) noexcept
		{
			m_entities.get<id_bitset>(i).set<traits_type::template tag_bit<T>()>();
			return (*this);
		}

		template <class T
		> self_type & add_tag(handle const & h) noexcept
		{
			return this->add_tag<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> self_type & del_tag(size_t const i) noexcept
		{
			m_entities.get<id_bitset>(i).clear<traits_type::template tag_bit<T>()>();
			return (*this);
		}

		template <class T
		> self_type & del_tag(handle const & h) noexcept
		{
			return this->del_tag<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD bool has_tag(size_t const i) const noexcept
		{
			return m_entities.get<id_bitset>(i).read<traits_type::template tag_bit<T>()>();
		}

		template <class T
		> ML_NODISCARD bool has_tag(handle const & h) const noexcept
		{
			return this->has_tag(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C, class ... Args
		> auto & add_component(size_t const i, Args && ... args) noexcept
		{
			m_entities.get<id_bitset>(i).set<traits_type::template component_bit<C>()>();

			auto & c{ m_components.get<C>(m_entities.get<id_index>(i)) };
			c = C{ ML_forward(args)... };
			return c;
		}

		template <class C
		> auto & add_component(size_t const i) noexcept
		{
			return this->add_component<C>(i, C{});
		}

		template <class C, class ... Args
		> auto & add_component(handle const & h, Args && ... args) noexcept
		{
			return this->add_component<C>(h.m_entity, ML_forward(args)...);
		}

		template <class C
		> auto & add_component(handle const & h) noexcept
		{
			return this->add_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> self_type & del_component(size_t const i) noexcept
		{
			m_entities.get<id_bitset>(i).clear<traits_type::template component_bit<C>()>();
			return (*this);
		}

		template <class C
		> self_type & del_component(handle const & h) noexcept
		{
			return this->del_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> ML_NODISCARD auto & get_component(size_t const i) noexcept
		{
			return m_components.get<C>(m_entities.get<id_index>(i));
		}

		template <class C
		> ML_NODISCARD auto const & get_component(size_t const i) const noexcept
		{
			return m_components.get<C>(m_entities.get<id_index>(i));
		}

		template <class C
		> ML_NODISCARD auto & get_component(handle const & h) noexcept
		{
			return this->get_component<C>(h.m_entity);
		}

		template <class C
		> ML_NODISCARD auto const & get_component(handle const & h) const noexcept
		{
			return this->get_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> ML_NODISCARD bool has_component(size_t const i) const noexcept
		{
			return this->get_signature(i).read<traits_type::template component_bit<C>()>();
		}

		template <class C
		> ML_NODISCARD bool has_component(handle const & h) const noexcept
		{
			return this->has_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD signature const & get_signature(size_t const i) const noexcept
		{
			return m_entities.get<id_bitset>(i);
		}

		ML_NODISCARD signature const & get_signature(handle const & h) const noexcept
		{
			return this->get_signature(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool matches_signature(size_t const i, signature const & s) const noexcept
		{
			return (s & this->get_signature(i)) == s;
		}

		template <class S
		> ML_NODISCARD bool matches_signature(size_t const i) const noexcept
		{
			return this->matches_signature(i, traits_type::template signature_bitset<S>());
		}

		template <class S
		> ML_NODISCARD bool matches_signature(handle const & h) const noexcept
		{
			return this->matches_signature<S>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <template <class> class X
		> ML_NODISCARD bool matches_system(size_t const i) const noexcept
		{
			return this->matches_signature<typename X<traits_type>::signature>(i);
		}

		template <template <class> class X
		> ML_NODISCARD bool matches_system(handle const & h) const noexcept
		{
			return this->matches_system<X>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// invoke function on every alive entity
		template <class Fn
		> self_type & for_entities(Fn && fn) noexcept
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(ML_forward(fn), i);
			}
			return (*this);
		}

		// invoke function on each of an entity's components
		template <class Fn
		> self_type & for_components(size_t const i, Fn && fn) noexcept
		{
			meta::for_types<typename traits_type::component_list>([&](auto c) noexcept
			{
				using C = typename decltype(c)::type;
				if (this->has_component<C>(i))
				{
					std::invoke(ML_forward(fn), this->get_component<C>(i));
				}
			});
			return (*this);
		}

		// invoke function on all alive entities matching a signature
		template <class S, class Fn
		> self_type & for_matching(Fn && fn) noexcept
		{
			return this->for_entities([&](size_t const i) noexcept
			{
				if (this->matches_signature<S>(i))
				{
					this->expand_call<S>(i, ML_forward(fn));
				}
			});
		}

		// invoke function on all systems matching a signature
		template <template <class> class X, class Fn
		> self_type & for_system(Fn && fn) noexcept
		{
			return this->for_matching<typename X<traits_type>::signature
			>([&, &x = std::get<traits_type::template system_id<X>()>(m_systems)
			](size_t, auto && ... req_comp) noexcept
			{
				std::invoke(ML_forward(fn), x, ML_forward(req_comp)...);
			});
		}

		// invoke all systems matching a signature
		template <template <class> class X, class ... Args
		> self_type & update_system(Args && ... args) noexcept
		{
			return this->for_system<X>([&](auto & x, auto && ... req_comp) noexcept
			{
				std::invoke(x, ML_forward(req_comp)..., ML_forward(args)...);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		template <class ... Ts
		> struct expand_call_helper;

		template <class S, class Fn
		> void expand_call(size_t const i, Fn && fn) noexcept
		{
			using req_comp = typename traits_type::components::template filter<S>;

			using helper = meta::rename<expand_call_helper, req_comp>;

			helper::call(i, *this, ML_forward(fn));
		}

		template <class ... Ts
		> struct expand_call_helper
		{
			template <class Fn
			> static void call(size_t const i, self_type & self, Fn && fn) noexcept
			{
				self.m_components.expand<Ts...>(self.m_entities.get<id_index>(i), [&
				](auto && ... req_comp) noexcept
				{
					std::invoke(ML_forward(fn), i, ML_forward(req_comp)...);
				});
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t m_capacity;	// storage capacity		(total allocated slots)
		size_t m_size;		// current size			(size excluding newly created entities)
		size_t m_size_next;	// size after refresh	(size including newly created entities)
		
		entity_storage		m_entities;		// entity data
		handle_storage		m_handles;		// handle data
		component_storage	m_components;	// component data
		system_storage		m_systems;		// systems data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// TESTS
namespace ml::ecs::tests
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tags
	struct T0 {};
	struct T1 {};
	struct T2 {};

	// components
	struct C0 {};
	struct C1 {};
	struct C2 {};
	struct C3 {};
	struct C4 {};

	// signatures
	using S0 = meta::list<>;				// 00000000
	using S1 = meta::list<C0, C1>;			// 11000000
	using S2 = meta::list<C0, C4, T0>;		// 10001100
	using S3 = meta::list<C1, T0, C3, T2>;	// 01010101

	// traits
	using M = traits<
		cfg::tags		<T0, T1, T2>,
		cfg::components	<C0, C1, C2, C3, C4>,
		cfg::signatures	<S0, S1, S2, S3>,
		cfg::systems	<>
	>;

	// tests
	static_assert(M::tag_count				== 3);
	static_assert(M::component_count		== 5);
	static_assert(M::signature_count		== 4);
	static_assert(M::system_count			== 0);

	static_assert(M::component_id<C0>()		== 0);
	static_assert(M::component_id<C1>()		== 1);
	static_assert(M::component_id<C2>()		== 2);
	static_assert(M::component_id<C3>()		== 3);
	static_assert(M::component_id<C4>()		== 4);
	static_assert(M::tag_id<T0>()			== 0);
	static_assert(M::tag_id<T1>()			== 1);
	static_assert(M::tag_id<T2>()			== 2);
	static_assert(M::signature_id<S0>()		== 0);
	static_assert(M::signature_id<S1>()		== 1);
	static_assert(M::signature_id<S2>()		== 2);
	static_assert(M::signature_id<S3>()		== 3);

	static_assert(M::component_bit<C0>()	== 0);
	static_assert(M::component_bit<C1>()	== 1);
	static_assert(M::component_bit<C2>()	== 2);
	static_assert(M::component_bit<C3>()	== 3);
	static_assert(M::component_bit<C4>()	== 4);
	static_assert(M::tag_bit<T0>()			== 5);
	static_assert(M::tag_bit<T1>()			== 6);
	static_assert(M::tag_bit<T2>()			== 7);

	static_assert(M::signature_bitset<S0>() == "00000000");
	static_assert(M::signature_bitset<S1>() == "11000000");
	static_assert(M::signature_bitset<S2>() == "10001100");
	static_assert(M::signature_bitset<S3>() == "01010101");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_