#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

// Sources:
// https://www.youtube.com/watch?v=NTWSeQtHZ9M
// https://github.com/SuperV1234/cppcon2015

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/TypeList.hpp>

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
		class _Components,
		class _Tags,
		class _Signatures
	> struct settings
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using component_list			= typename _Components;
		using tag_list					= typename _Tags;
		using signature_list			= typename _Signatures;
		using self_type					= typename settings<_Components, _Tags, _Signatures>;
		using signature_bitsets			= typename impl::signature_bitsets<self_type>;
		using signature_bitset_storage	= typename impl::signature_bitset_storage<self_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr bool is_component() noexcept
		{
			return meta::contains<T, component_list>{};
		}

		template <class T> static constexpr bool is_tag() noexcept
		{
			return meta::contains<T, tag_list>{};
		}

		template <class T> static constexpr bool is_signature() noexcept
		{
			return meta::contains<T, signature_list>{};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t component_count() noexcept
		{
			return meta::size<component_list>();
		}

		static constexpr size_t tag_count() noexcept
		{
			return meta::size<tag_list>();
		}

		static constexpr size_t signature_count() noexcept
		{
			return meta::size<signature_list>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr size_t component_id() noexcept
		{
			return meta::index_of<T, component_list>::value;
		}

		template <class T> static constexpr size_t tag_id() noexcept
		{
			return meta::index_of<T, tag_list>::value;
		}

		template <class T> static constexpr size_t signature_id() noexcept
		{
			return meta::index_of<T, signature_list>::value;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using bit_set = typename std::bitset<component_count() + tag_count()>;

		template <class T> static constexpr size_t component_bit() noexcept
		{
			return component_id<T>();
		}

		template <class T> static constexpr size_t tag_bit() noexcept
		{
			return component_count() + tag_id<T>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Signature
namespace ml::ecs::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Settings
	> struct signature_bitsets
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using settings_type		= _Settings;
		using signature_list	= typename settings_type::signature_list;
		using bit_set			= typename settings_type::bit_set;

		using bitset_repeated_list = meta::repeat<
			settings_type::signature_count(),
			bit_set
		>;

		using bitset_storage = meta::tuple<bitset_repeated_list>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> using is_component_filter = std::integral_constant<
			bool, settings_type::template is_component<T>()
		>;

		template <class T
		> using is_tag_filter = std::integral_constant<
			bool, settings_type::template is_tag<T>()
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Signature
		> using signature_components = meta::filter<
			is_component_filter, Signature
		>;

		template <class Signature
		> using signature_tags = meta::filter<
			is_tag_filter, Signature
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Settings
	> struct signature_bitset_storage
	{
		using settings_type		= _Settings;
		using signature_bitsets = typename settings_type::signature_bitsets;
		using signature_list	= typename signature_bitsets::signature_list;
		using bitset_storage	= typename signature_bitsets::bitset_storage;

		signature_bitset_storage() noexcept
		{
			meta::for_types<signature_list>([this](auto t)
			{
				this->initialize_bitset<ECS_TYPE(t)>();
			});
		}

		bitset_storage const & get_storage() const noexcept
		{
			return m_storage;
		}

		template <class T
		> auto & get_signature_bitset() noexcept
		{
			static_assert(settings_type::template is_signature<T>(), "?");
			return std::get<
				settings_type::template signature_id<T>()
			>(m_storage);
		}

	private:
		template <class T
		> void initialize_bitset() noexcept
		{
			auto & b(this->get_signature_bitset<T>());

			meta::for_types<signature_bitsets::template signature_components<T>
			>([&, this, &b](auto t)
			{
				b[settings_type::template component_bit<ECS_TYPE(t)>()] = true;
			});

			meta::for_tuple<signature_bitsets::template signature_tags<T>
			>([&, this, &b](auto t)
			{
				b[settings_type::template tag_bit<ECS_TYPE(t)>()] = true;
			});
		}

		bitset_storage m_storage;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Entity
namespace ml::ecs
{
	enum EntityFlags_
	{
		EntityFlags_None,
		EntityFlags_Alive = (1 << 0),
	};

	template <class _Settings
	> struct entity
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using settings_type = typename _Settings;

		using bit_set = typename settings_type::bit_set;

		size_t index{};

		bit_set bitset{};

		int32_t flags{ EntityFlags_None };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// Manager
namespace ml::ecs
{
	template <class _Settings
	> struct manager
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using settings_type		= typename _Settings;
		using bit_set			= typename settings_type::bit_set;
		using self_type			= typename manager<_Settings>;
		using entity_type		= typename entity<_Settings>;
		using entity_storage	= typename pmr::vector<entity_type>;
		using bitset_storage	= typename impl::signature_bitset_storage<_Settings>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		manager() noexcept : self_type{ allocator_type{} }
		{
		}

		~manager() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit manager(allocator_type const & alloc)
			: m_size		{}
			, m_capacity	{}
			, m_size_next	{}
			, m_entities	{ alloc }
			, m_bitsets		{}
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t			m_size;
		size_t			m_capacity;
		size_t			m_size_next;
		entity_storage	m_entities;
		bitset_storage	m_bitsets;


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_