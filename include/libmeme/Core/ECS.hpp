#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

// Data-Oriented Entity Component System

// Sources:
// https://github.com/SuperV1234/cppcon2015
// https://www.youtube.com/watch?v=NTWSeQtHZ9M

#include <libmeme/Core/BitSet.hpp>
#include <libmeme/Core/Typeof.hpp>

#define _ML_ECS _ML ecs::

// ECS
namespace ml::ecs
{
	template <class
	> struct manager;
}

// UTILITY
namespace ml::ecs::util
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

		using storage_type = typename meta::tuple<meta::remap<pmr::vector, type_list>>;

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

		using type_list = typename meta::list<_ML_ECS util::template x_wrapper<Systems>...>;

		template <class M
		> using storage_type = typename meta::tuple<meta::list<Systems<M>...>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t count() noexcept
		{
			return meta::size<type_list>();
		}

		template <template <class> class X
		> static constexpr bool contains() noexcept
		{
			return meta::contains<_ML_ECS util::x_wrapper<X>, type_list>();
		}

		template <template <class> class X
		> static constexpr size_t index() noexcept
		{
			static_assert(self_type::contains<X>(), "system not found");
			return meta::index_of<_ML_ECS util::x_wrapper<X>, type_list>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// (O) OPTIONS
namespace ml::ecs::cfg
{
	template <
		size_t GrowAmt = 5,	// growth base amount
		size_t MultNum = 2,	// growth multipler numerator
		size_t MultDen = 1	// growth multipler denomenator
	> struct options final
	{
		static_assert(0 < GrowAmt);
		static_assert(0 < MultDen);
		static_assert(MultDen <= MultNum);

		static constexpr size_t grow_amt
		{
			GrowAmt
		};
		static constexpr float_t grow_mul
		{
			_ML util::ratio_cast(1.f, std::ratio<MultNum, MultDen>{})
		};
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
		using signature_storage	= typename meta::tuple<meta::repeat<signature_count, signature>>;

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
				// get the signature's signature
				auto & b{ std::get<self_type::signature_id<decltype(s)::type>()>(temp) };

				// enable component bits
				meta::for_types<components::template filter<decltype(s)::type>
				>([&b](auto c)
				{
					b.write<self_type::component_bit<decltype(c)::type>()
					>(true);
				});

				// enable tag bits
				meta::for_types<tags::template filter<decltype(s)::type>
				>([&b](auto t)
				{
					b.write<self_type::tag_bit<decltype(t)::type>()
					>(true);
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

		ML_NODISCARD inline bool valid() const noexcept
		{
			return m_manager && m_manager->is_valid_handle(*this);
		}

		ML_NODISCARD inline operator bool() const noexcept
		{
			return this->valid(); // true if valid
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool alive() const noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->is_alive(*this);
		}

		inline handle & kill() noexcept
		{
			ML_ASSERT(m_manager);
			m_manager->kill(*this);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> inline handle & add_tag() noexcept
		{
			ML_ASSERT(m_manager);
			m_manager->template add_tag<T>(*this);
			return (*this);
		}

		template <class T
		> inline handle & del_tag() noexcept
		{
			ML_ASSERT(m_manager);
			m_manager->template del_tag<T>(*this);
			return (*this);
		}

		template <class T
		> inline bool has_tag() noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->template has_tag<T>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C, class ... Args
		> inline decltype(auto) add_component(Args && ... args) noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->template add_component<C>(*this, ML_FWD(args)...);
		}

		template <class C
		> inline decltype(auto) add_component() noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->template add_component<C>(*this);
		}

		template <class C
		> inline handle & del_component() noexcept
		{
			ML_ASSERT(m_manager);
			m_manager->template del_component<C>(*this);
			return (*this);
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component() noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->template get_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component() const noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->template get_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD inline bool has_component() const noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->template has_component<C>(*this);
		}

		template <class C
		> ML_NODISCARD inline bool matches_signature() const noexcept
		{
			ML_ASSERT(m_manager);
			return m_manager->template matches_signature<C>(*this);
		}

		template <template <class> class X
		> ML_NODISCARD inline bool matches_system() const noexcept
		{
			ML_ASSERT(m_manager);
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

		using traits_type		= typename U;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using self_type			= typename manager<traits_type>;
		using signature			= typename traits_type::signature;
		using component_storage	= typename traits_type::component_storage;
		using system_storage	= typename traits_type::system_storage;
		using handle			= typename impl::handle<self_type>;
		using handle_storage	= typename pmr::vector<handle>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Alive, ID_Index, ID_Handle, ID_Signature };

		using entity_storage = typename std::tuple<
			pmr::vector<bool>,		// state of entity (alive / dead)
			pmr::vector<size_t>,	// index of real component data
			pmr::vector<size_t>,	// index of managing handle
			pmr::vector<signature>	// signature bitsets
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit manager(allocator_type const & alloc) noexcept
			: m_capacity	{ 0 }
			, m_size		{ 0 }
			, m_size_next	{ 0 }
			, m_entities	{ std::allocator_arg, alloc }
			, m_handles		{ alloc }
			, m_components	{ std::allocator_arg, alloc }
			, m_systems		{}
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

		manager() noexcept : self_type{ 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			this->deep_copy(other);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
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

		inline void clear() noexcept
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				std::get<ID_Alive>(	m_entities)[i] = false;
				std::get<ID_Index>(	m_entities)[i] = i;
				std::get<ID_Handle>(m_entities)[i] = i;
				std::get<ID_Signature>(m_entities)[i].reset();
				
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
				m_systems	= other.m_systems;
			}
		}

		inline void grow_to(size_t const cap)
		{
			if (cap <= m_capacity) return;

			meta::for_tuple([this, cap](auto & v) { v.resize(cap); }, m_entities);
			m_handles.resize(cap);
			meta::for_tuple([this, cap](auto & v) { v.resize(cap); }, m_components);

			for (size_t i = m_capacity; i < cap; ++i)
			{
				std::get<ID_Alive>(m_entities)[i] = false;
				std::get<ID_Index>(m_entities)[i] = i;
				std::get<ID_Handle>(m_entities)[i] = i;
				std::get<ID_Signature>(m_entities)[i].reset();

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
						if (!std::get<ID_Alive>(m_entities)[dead]) break;
					}

					// find alive entity from the right
					for (; true; --alive)
					{
						if (std::get<ID_Alive>(m_entities)[alive]) break;
						if (alive <= dead) return dead;
					}

					// found two entities that need to be swapped
					ML_ASSERT(std::get<ID_Alive>(m_entities)[alive]);
					ML_ASSERT(!std::get<ID_Alive>(m_entities)[dead]);

					this->swap(alive, dead);

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

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_size,		other.m_size);
				std::swap(m_capacity,	other.m_capacity);
				std::swap(m_size_next,	other.m_size_next);

				m_entities	.swap(other.m_entities);
				m_handles	.swap(other.m_handles);
				m_components.swap(other.m_components);
				m_systems	.swap(other.m_systems);
			}
		}

		inline void swap(size_t const lhs, size_t const rhs) noexcept
		{
			if (lhs != rhs)
			{
				std::swap(std::get<ID_Alive>(m_entities)[lhs], std::get<ID_Alive>(m_entities)[rhs]);
				std::swap(std::get<ID_Index>(m_entities)[lhs], std::get<ID_Index>(m_entities)[rhs]);
				std::swap(std::get<ID_Handle>(m_entities)[lhs], std::get<ID_Handle>(m_entities)[rhs]);
				std::swap(std::get<ID_Signature>(m_entities)[lhs], std::get<ID_Signature>(m_entities)[rhs]);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_t create_entity()
		{
			// grow if needed
			if (m_capacity <= m_size_next)
			{
				constexpr auto amt{ traits_type::options::grow_amt };
				constexpr auto mul{ traits_type::options::grow_mul };
				this->grow_to(static_cast<size_t>((m_capacity + amt) * mul));
			}

			size_t const i{ m_size_next++ };
			std::get<ID_Alive>(m_entities)[i] = true;
			std::get<ID_Signature>(m_entities)[i].reset();
			return i;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool is_alive(size_t const i) const
		{
			return std::get<ID_Alive>(m_entities)[i];
		}

		ML_NODISCARD inline bool is_alive(handle const & h) const
		{
			return this->is_alive(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & kill(size_t const i)
		{
			std::get<ID_Alive>(m_entities)[i] = false;
			return (*this);
		}

		inline self_type & kill(handle const & h)
		{
			return this->kill(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline handle create_handle()
		{
			size_t const i{ this->create_entity() };
			size_t const e{ std::get<ID_Handle>(m_entities)[i] };
			auto & h{ m_handles[e] };

			handle temp{};
			temp.m_manager	= h.m_manager;
			temp.m_entity	= h.m_entity = i;
			temp.m_self		= e;
			temp.m_counter	= h.m_counter;
			ML_ASSERT(temp);
			return temp;
		}

		ML_NODISCARD inline bool is_valid_handle(handle const & h) const noexcept
		{
			return (h.m_counter == this->get_handle(h).m_counter);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		template <class T
		> inline self_type & add_tag(size_t const i) noexcept
		{
			std::get<ID_Signature>(m_entities)[i].write<traits_type::template tag_bit<T>()
			>(true);
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
			std::get<ID_Signature>(m_entities)[i].write<traits_type::template tag_bit<T>()
			>(false);
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
			return std::get<ID_Signature>(m_entities)[i].read<traits_type::template tag_bit<T>()>();
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
			std::get<ID_Signature>(m_entities)[i].write<traits_type::template component_bit<C>()
			>(true);

			auto & c{ std::get<pmr::vector<C>>(m_components)[std::get<ID_Index>(m_entities)[i]] };
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
			std::get<ID_Signature>(m_entities)[i].write<traits_type::template component_bit<C>()
			>(false);
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
			return std::get<pmr::vector<C>>(m_components)[std::get<ID_Index>(m_entities)[i]];
		}

		template <class C
		> ML_NODISCARD inline decltype(auto) get_component(size_t const i) const noexcept
		{
			return std::get<pmr::vector<C>>(m_components)[std::get<ID_Index>(m_entities)[i]];
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

		ML_NODISCARD signature const & get_signature(size_t const i) const noexcept
		{
			return std::get<ID_Signature>(m_entities)[i];
		}

		ML_NODISCARD signature const & get_signature(handle const & h) const noexcept
		{
			return this->get_signature(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class C
		> ML_NODISCARD inline bool has_component(size_t const i) const noexcept
		{
			return this->get_signature(i).read<traits_type::template component_bit<C>()>();
		}
		
		template <class C
		> ML_NODISCARD inline bool has_component(handle const & h) const noexcept
		{
			return this->has_component<C>(h.m_entity);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool matches_signature(size_t const i, signature const & s) const noexcept
		{
			return (s & this->get_signature(i)) == s;
		}

		template <class S
		> ML_NODISCARD inline bool matches_signature(size_t const i) const noexcept
		{
			return this->matches_signature(i, traits_type::template signature_bitset<S>());
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
			// invoke function on every alive entity
			for (size_t i = 0; i < m_size; ++i)
			{
				std::invoke(ML_FWD(fn), i);
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_components(size_t const i, Fn && fn)
		{
			// invoke function on all of an entity's components
			meta::for_types<typename traits_type::component_list>([&](auto c)
			{
				using C = typename decltype(c)::type;
				if (this->has_component<C>(i))
				{
					std::invoke(ML_FWD(fn), this->get_component<C>(i));
				}
			});
			return (*this);
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class S, class Fn
		> inline self_type & for_matching(Fn && fn)
		{
			// invoke function on all entities matching a signature
			return this->for_entities([&](size_t const i)
			{
				if (this->matches_signature<S>(i))
				{
					this->expand_call<S>(i, ML_FWD(fn));
				}
			});
		}

		template <template <class> class X, class Fn
		> inline self_type & for_system(Fn && fn)
		{
			// invoke function on all systems matching a signature
			return this->for_matching<typename X<traits_type>::signature
			>([&fn, &x = std::get<traits_type::template system_id<X>()>(m_systems)
			](size_t, auto && ... req_comp)
			{
				std::invoke(ML_FWD(fn), x, ML_FWD(req_comp)...);
			});
		}

		template <template <class> class X, class ... Args
		> inline self_type & update_system(Args && ... args)
		{
			// invoke update on all systems matching a signature
			return this->for_system<X>([&args...](auto & x, auto && ... req_comp)
			{
				x.update(ML_FWD(args)..., ML_FWD(req_comp)...);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		template <class ... Ts
		> struct expand_call_helper;

		template <class S, class Fn
		> inline void expand_call(size_t const i, Fn && fn)
		{
			using req_comp = typename traits_type::components::template filter<S>;

			using helper = meta::rename<expand_call_helper, req_comp>;

			helper::call(i, *this, ML_FWD(fn));
		}

		template <class ... Ts
		> struct expand_call_helper
		{
			template <class Fn
			> static inline void call(size_t const i, self_type & self, Fn && fn)
			{
				auto const c{ std::get<ID_Index>(self.m_entities)[i] }; // component data index

				std::invoke(ML_FWD(fn), i, std::get<pmr::vector<Ts>>(self.m_components)[c]...);
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