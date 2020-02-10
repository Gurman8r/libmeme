#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

// Credit to Vittorio Romeo
// https://github.com/SuperV1234/cppcon2015
// https://www.youtube.com/watch?v=NTWSeQtHZ9M

#include <libmeme/Core/BitSet.hpp>
#include <libmeme/Core/Meta.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

/* ecs
	manager<
		traits<
			components	<Cpt...>,
			tags		<Tag...>,
			signatures	<Sig...>,
			systems		<Sys<T>...>
		>
	>
*/

namespace ml::ecs
{
	template <class Traits
	> struct manager;
}

// CONFIG
namespace ml::ecs::cfg
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// COMPONENTS
	template <class ... Components
	> struct components final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename components<Components...>;
		
		using type_list = typename meta::list<Components...>;

		using storage = typename meta::tuple<meta::remap<pmr::vector, type_list>>;

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TAGS
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// SIGNATURES
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		// allows "template template" types to play nice with meta::list
		template <template <class> class X
		> struct system final {};
	}

	// SYSTEMS
	template <template <class> class ... Systems
	> struct systems final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename systems<Systems...>;
		
		using type_list = typename meta::list<detail::template system<Systems>...>;

		template <class Traits
		> using storage = meta::tuple<meta::list<Systems<Traits>...>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <template <class> class X
		> static constexpr bool contains() noexcept
		{
			return meta::contains<detail::system<X>, type_list>();
		}

		template <template <class> class X
		> static constexpr size_t index() noexcept
		{
			static_assert(self_type::contains<X>(), "system not found");
			return meta::index_of<detail::system<X>, type_list>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// OPTIONS
	template <
		size_t StartCap,	// starting capacity
		size_t GrowAmt,	// grow amount
		size_t MultNum,	// grow multipler numerator
		size_t MultDen		// grow multipler denomenator
	> struct options final
	{
		static constexpr size_t start_capacity
		{
			StartCap
		};
		static constexpr size_t grow_amount
		{
			GrowAmt
		};
		static constexpr float_t grow_multiplier
		{
			util::ratio_cast(1.f, std::ratio<MultNum, MultDen>{})
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ENTITY
namespace ml::ecs::impl
{
	// entity class
	template <class Manager> class entity final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend Manager;

		using traits_type = typename Manager::traits_type;

		using bitset_type = typename traits_type::bitset_type;

		bool		m_alive;	// state of entity (alive / dead)
		size_t		m_data;		// index of real component data
		size_t		m_handle;	// index of managing handle
		bitset_type	m_bitset;	// signature bitset

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// HANDLE
namespace ml::ecs::impl
{
	// abstraction over entity/manager interface
	template <class Manager> class handle final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend Manager;

		Manager *	m_manager;	// pointer to owning manager
		size_t		m_entity;	// index of real entity data
		size_t		m_self;		// index of real handle data
		int32_t		m_counter;	// validity check counter

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline handle & invalidate() noexcept
		{
			++m_counter;
			return (*this);
		}

		inline handle & refresh(size_t const i) noexcept
		{
			m_entity = i;
			return (*this);
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept
		{
			return this->valid(); // true if valid
		}

		ML_NODISCARD inline bool valid() const noexcept
		{
			return m_manager && m_manager->is_valid_handle(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool alive() const noexcept
		{
			return m_manager->is_alive(*this);
		}

		inline handle & kill() noexcept
		{
			m_manager->kill(*this);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline handle & add_tag() noexcept
		{
			m_manager->add_tag<T>(*this);
			return (*this);
		}

		template <class T
		> inline handle & del_tag() noexcept
		{
			m_manager->del_tag<T>(*this);
			return (*this);
		}

		template <class T
		> inline bool has_tag() noexcept
		{
			return m_manager->has_tag<T>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C, class ... Args
		> inline decltype(auto) add_component(Args && ... args) noexcept
		{
			return m_manager->add_component<C>(*this, ML_FWD(args)...);
		}

		template <class C
		> inline decltype(auto) add_component() noexcept
		{
			return m_manager->add_component<C>(*this);
		}

		template <class C
		> inline handle & del_component() noexcept
		{
			m_manager->del_component<C>(*this);
			return (*this);
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component() noexcept
		{
			return m_manager->get_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component() const noexcept
		{
			return m_manager->get_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD inline bool has_component() const noexcept
		{
			return m_manager->has_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD inline bool matches_signature() const noexcept
		{
			return m_manager->matches_signature<C>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// TRAITS
namespace ml::ecs
{
	template <
		class Tags			= cfg::tags			<>,
		class Components	= cfg::components	<>,
		class Signatures	= cfg::signatures	<>,
		class Systems		= cfg::systems		<>,
		class Options		= cfg::options		<42, 5, 2, 1>
	> struct traits final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using self_type = typename traits<
			Tags,
			Components,
			Signatures,
			Systems,
			Options
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using components	= typename Components;
		using tags			= typename Tags;
		using signatures	= typename Signatures;
		using systems		= typename Systems;
		using options		= typename Options;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t		component_count	{ components::count() };
		static constexpr size_t		tag_count		{ tags::count() };
		static constexpr size_t		signature_count	{ signatures::count() };
		static constexpr size_t		system_count	{ systems::count() };
		static constexpr size_t		bit_count		{ component_count + tag_count };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using bitset_type = typename ds::bitset<bit_count>;

		using bitset_storage = typename meta::tuple<meta::repeat<signature_count, bitset_type>>;

		using component_storage = typename components::storage;

		using system_storage = typename systems::template storage<self_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> static constexpr bool has_component() noexcept
		{
			return components::template contains<C>();
		}

		template <class T
		> static constexpr bool has_tag() noexcept
		{
			return tags::template contains<T>();
		}

		template <class S
		> static constexpr bool has_signature() noexcept
		{
			return signatures::template contains<S>();
		}

		template <template <class> class X
		> static constexpr bool has_system() noexcept
		{
			return systems::template contains<X>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> static constexpr size_t component_index() noexcept
		{
			return components::template index<C>();
		}

		template <class T
		> static constexpr size_t tag_index() noexcept
		{
			return tags::template index<T>();
		}

		template <class S
		> static constexpr size_t signature_index() noexcept
		{
			return signatures::template index<S>();
		}

		template <template <class> class X
		> static constexpr size_t system_index() noexcept
		{
			return systems::template index<X>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> static constexpr size_t component_bit() noexcept
		{
			return self_type::component_index<C>();
		}

		template <class T
		> static constexpr size_t tag_bit() noexcept
		{
			return component_count + self_type::tag_index<T>();
		}

		template <class S
		> static constexpr bitset_type const & signature_bitset() noexcept
		{
			return std::get<self_type::signature_index<S>()>(m_bitset_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static constexpr bitset_storage generate_bitset_storage() noexcept
		{
			// generate bitsets for each signature
			bitset_storage temp{};
			meta::for_types<typename signatures::type_list
			>([&temp](auto sig)
			{
				// get the signature's bitset_type
				auto & b{ std::get<
					self_type::signature_index<decltype(sig)::type>()
				>(temp) };

				// enable component bits
				meta::for_types<components::template filter<decltype(sig)::type>
				>([&b](auto cpt)
				{
					b.set<self_type::component_bit<decltype(cpt)::type>()
					>();
				});

				// enable tag bits
				meta::for_types<tags::template filter<decltype(sig)::type>
				>([&b](auto tag)
				{
					b.set<self_type::tag_bit<decltype(tag)::type>()
					>();
				});
			});
			return temp;
		}
		
		static constexpr auto m_bitset_storage{ generate_bitset_storage() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// MANAGER
namespace ml::ecs
{
	template <class Traits
	> struct manager final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type		= typename Traits;
		using self_type			= typename manager<traits_type>;
		using entity			= typename impl::entity<self_type>;
		using handle			= typename impl::handle<self_type>;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using entity_storage	= typename pmr::vector<entity>;
		using handle_storage	= typename pmr::vector<handle>;
		using component_storage	= typename traits_type::component_storage;
		using system_storage	= typename traits_type::system_storage;

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
			this->deep_copy(other);
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

		manager() noexcept
			: self_type{ traits_type::options::start_capacity }
		{
		}

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
				e.m_bitset.reset();
				e.m_alive = false;
				
				auto & h{ m_handles[i] };
				h.m_entity = i;
				h.m_counter = 0;
			}
			m_size = m_size_next = 0;
		}

		inline void deep_copy(self_type const & other)
		{
			if (this != std::addressof(other))
			{
				m_size		= other.m_size;
				m_capacity	= other.m_capacity;
				m_size_next = other.m_size_next;
				m_entities	= other.m_entities;
				m_handles	= other.m_handles;
				m_components= other.m_components;
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
				auto & e{ m_entities[i] };
				e.m_data = i;
				e.m_handle = i;
				e.m_bitset.reset();
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
			else m_size = m_size_next = ([&]()
			{
				// arrange all alive entities towards the left
				size_t dead{ 0 }, alive{ m_size_next - 1 };
				while (true)
				{
					// find dead entity from the left
					for (; true; ++dead)
					{
						if (dead > alive) return dead;
						if (!m_entities[dead].m_alive) break;
					}

					// find alive entity from the right
					for (; true; --alive)
					{
						if (m_entities[alive].m_alive) break;
						if (alive <= dead) return dead;
					}

					// found two entities that need to be swapped
					ML_ASSERT(m_entities[alive].m_alive);
					ML_ASSERT(!m_entities[dead].m_alive);
					std::swap(m_entities[alive], m_entities[dead]);

					// refresh alive entity
					m_handles[alive].refresh(alive);

					// invalidate and refresh dead entity
					m_handles[dead].invalidate().refresh(dead);

					// move both iterator indices
					++dead; --alive;
				}
				return dead;
			})();
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_size,		other.m_size);
				std::swap(m_capacity,	other.m_capacity);
				std::swap(m_size_next,	other.m_size_next);
				std::swap(m_entities,	other.m_entities);
				std::swap(m_handles,	other.m_handles);
				std::swap(m_components, other.m_components);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto capacity() const noexcept -> size_t { return m_capacity; }

		inline auto size() const noexcept -> size_t { return m_size; }

		inline auto size_next() const noexcept -> size_t { return m_size_next; }

		inline auto entities() const noexcept -> entity_storage const & { return m_entities; }

		inline auto handles() const noexcept -> handle_storage const & { return m_handles; }

		inline auto components() const noexcept -> component_storage const & { return m_components; }

		inline auto systems() const noexcept -> system_storage const & { return m_systems; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_t create_entity()
		{
			// grow if needed
			if (m_capacity <= m_size_next)
			{
				static constexpr auto amt{ traits_type::options::grow_amount };
				static constexpr auto mul{ traits_type::options::grow_multiplier };
				this->grow_to(static_cast<size_t>((m_capacity * amt) * mul));
			}

			size_t i{ m_size_next++ };
			auto & e{ m_entities[i] };
			e.m_alive = true;
			e.m_bitset.reset();
			return i;
		}

		ML_NODISCARD inline entity & get_entity(size_t const i)
		{
			return m_entities[i];
		}

		ML_NODISCARD inline entity & get_entity(handle const & h)
		{
			return this->get_entity(h.m_entity);
		}

		ML_NODISCARD inline entity const & get_entity(size_t const i) const
		{
			return m_entities[i];
		}
		
		ML_NODISCARD inline entity const & get_entity(handle const & h) const
		{
			return this->get_entity(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline handle create_handle()
		{
			size_t i{ this->create_entity() };
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
			return m_handles[i];
		}

		ML_NODISCARD inline handle const & get_handle(size_t const i) const
		{
			return m_handles[i];
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
			return m_entities[i].m_alive;
		}

		ML_NODISCARD inline bool is_alive(handle const & h) const
		{
			return this->is_alive(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & kill(size_t const i)
		{
			m_entities[i].m_alive = false;
			return (*this);
		}

		inline self_type & kill(handle const & h)
		{
			return this->kill(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline self_type & add_tag(size_t const i) noexcept
		{
			m_entities[i].m_bitset.set<traits_type::template tag_bit<T>()>();
			return (*this);
		}

		template <class T
		> inline self_type & add_tag(handle const & h) noexcept
		{
			return this->add_tag<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline self_type & del_tag(size_t const i) noexcept
		{
			m_entities[i].m_bitset.clear<traits_type::template tag_bit<T>()>();
			return (*this);
		}

		template <class T
		> inline self_type & del_tag(handle const & h) noexcept
		{
			return this->del_tag<T>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool has_tag(size_t const i) const noexcept
		{
			return m_entities[i].m_bitset.read<traits_type::template tag_bit<T>()>();
		}

		template <class T
		> ML_NODISCARD inline bool has_tag(handle const & h) const noexcept
		{
			return this->has_tag(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C, class ... Args
		> inline decltype(auto) add_component(size_t const i, Args && ... args) noexcept
		{
			auto & e{ m_entities[i] };
			e.m_bitset.set<traits_type::template component_bit<C>()>();

			auto & c{ std::get<pmr::vector<C>>(m_components)[e.m_data] };
			c = C{ ML_FWD(args)... };
			return c;
		}

		template <class C
		> inline decltype(auto) add_component(size_t const i) noexcept
		{
			return this->add_component<C>(i, C{});
		}

		template <class C, class ... Args
		> inline decltype(auto) add_component(handle const & h, Args && ... args) noexcept
		{
			return this->add_component<C>(h.m_entity, ML_FWD(args)...);
		}

		template <class C
		> inline decltype(auto) add_component(handle const & h) noexcept
		{
			return this->add_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> inline self_type & del_component(size_t const i) noexcept
		{
			m_entities[i].m_bitset.clear<traits_type::template component_bit<C>()>();
			return (*this);
		}

		template <class C
		> inline self_type & del_component(handle const & h) noexcept
		{
			return this->del_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component(size_t const i) noexcept
		{
			return std::get<pmr::vector<C>>(m_components)[m_entities[i].m_data];
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component(size_t const i) const noexcept
		{
			return std::get<pmr::vector<C>>(m_components)[m_entities[i].m_data];
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component(handle const & h) noexcept
		{
			return this->get_component<C>(h.m_entity);
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component(handle const & h) const noexcept
		{
			return this->get_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> ML_NODISCARD inline bool has_component(size_t const i) const noexcept
		{
			return m_entities[i].m_bitset.read(traits_type::template component_bit<C>());
		}
		
		template <class C
		> ML_NODISCARD inline bool has_component(handle const & h) const noexcept
		{
			return this->has_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class S
		> ML_NODISCARD inline bool matches_signature(size_t const i) const noexcept
		{
			auto const & e{ m_entities[i].m_bitset };
			auto const & s{ traits_type::template signature_bitset<S>() };
			return (s & e) == s;
		}

		template <class S
		> ML_NODISCARD inline bool matches_signature(handle const & h) const noexcept
		{
			return this->matches_signature<S>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <template <class> class X
		> ML_NODISCARD inline bool matches_system(size_t const i) const noexcept
		{
			return this->matches_signature<typename X<traits_type>::signature>(i);
		}

		template <template <class> class X
		> ML_NODISCARD inline bool matches_system(handle const & h) const noexcept
		{
			return this->matches_system<X>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type & for_entities(Fn && fn)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(ML_FWD(fn), i);
			}
			return (*this);
		}

		template <class S, class Fn
		> inline self_type & for_matching(Fn && fn)
		{
			return this->for_entities([&](size_t const i)
			{
				if (this->matches_signature<S>(i))
				{
					this->expand_invoke<S>(i, ML_FWD(fn));
				}
			});
		}

		template <template <class> class X, class ... Args
		> inline self_type & update_system(Args && ... args)
		{
			auto & sys{ std::get<traits_type::template system_index<X>()>(m_systems) };

			return this->for_matching<typename X<traits_type>::signature
			>([&](auto, auto && ... req_comp)
			{
				sys.update(ML_FWD(args)..., ML_FWD(req_comp)...);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		template <class ... Ls> struct invoke_helper;

		template <class S, class Fn
		> inline void expand_invoke(size_t const i, Fn && fn)
		{
			using req_comp = typename traits_type::components::template filter<S>;

			using helper = meta::rename<invoke_helper, req_comp>;

			helper::invoke(i, *this, ML_FWD(fn));
		}

		template <class ... Ls> struct invoke_helper
		{
			template <class Fn
			> static inline void invoke(size_t const i, self_type & self, Fn && fn)
			{
				auto const idx{ self.get_entity(i).m_data };

				std::invoke(ML_FWD(fn), i, std::get<pmr::vector<Ls>>(self.m_components)[idx]...);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t m_capacity;	// storage capacity		(allocated memory)
		size_t m_size;		// current size			(excludes newly created entities)
		size_t m_size_next;	// size after refresh	(includes newly created entities)
		
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
	using MT = traits<
		cfg::tags		<T0, T1, T2>,
		cfg::components	<C0, C1, C2, C3, C4>,
		cfg::signatures	<S0, S1, S2, S3>,
		cfg::systems	<>
	>;

	// tests
	static_assert(MT::tag_count			== 3);
	static_assert(MT::component_count	== 5);
	static_assert(MT::signature_count	== 4);
	static_assert(MT::system_count		== 0);

	static_assert(MT::component_index<C0>() == 0);
	static_assert(MT::component_index<C1>() == 1);
	static_assert(MT::component_index<C2>() == 2);
	static_assert(MT::component_index<C3>() == 3);
	static_assert(MT::component_index<C4>() == 4);
	static_assert(MT::tag_index<T0>()		== 0);
	static_assert(MT::tag_index<T1>()		== 1);
	static_assert(MT::tag_index<T2>()		== 2);
	static_assert(MT::signature_index<S0>() == 0);
	static_assert(MT::signature_index<S1>() == 1);
	static_assert(MT::signature_index<S2>() == 2);
	static_assert(MT::signature_index<S3>() == 3);

	static_assert(MT::component_bit<C0>()	== 0);
	static_assert(MT::component_bit<C1>()	== 1);
	static_assert(MT::component_bit<C2>()	== 2);
	static_assert(MT::component_bit<C3>()	== 3);
	static_assert(MT::component_bit<C4>()	== 4);
	static_assert(MT::tag_bit<T0>()			== 5);
	static_assert(MT::tag_bit<T1>()			== 6);
	static_assert(MT::tag_bit<T2>()			== 7);

	static_assert(MT::signature_bitset<S0>() == "00000000");
	static_assert(MT::signature_bitset<S1>() == "11000000");
	static_assert(MT::signature_bitset<S2>() == "10001100");
	static_assert(MT::signature_bitset<S3>() == "01010101");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_