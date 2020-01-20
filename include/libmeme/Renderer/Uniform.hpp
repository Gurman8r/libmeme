#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	struct ML_RENDERER_API uniform final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using variable_type = typename std::variant<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, color,
			mat2, mat3, mat4,
			struct texture const *
		>;

		using function_type = typename std::function<
			variable_type()
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type_t = typename typeof<>;

		using name_t = typename pmr::string;

		using data_t = typename std::variant<variable_type, function_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Variable, ID_Function };

		enum : size_t { ID_Type, ID_Name, ID_Data };

		using storage_type = typename std::tuple<type_t, name_t, data_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform() noexcept : m_storage{} {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit uniform(allocator_type const & alloc) : m_storage{ std::make_tuple(
			type_t{},
			name_t{ alloc },
			data_t{}
		) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit uniform(storage_type const & value, allocator_type const & alloc = {}) : m_storage{ std::make_tuple(
			type_t{ std::get<ID_Type>(value) },
			name_t{ std::get<ID_Name>(value), alloc },
			data_t{ std::get<ID_Data>(value) }
		) }
		{
		}

		explicit uniform(storage_type && value, allocator_type const & alloc = {}) noexcept : m_storage{ std::make_tuple(
			type_t{ std::move(std::get<ID_Type>(value)) },
			name_t{ std::move(std::get<ID_Name>(value)), alloc },
			data_t{ std::move(std::get<ID_Data>(value)) }
		) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform(uniform const & other, allocator_type const & alloc = {}) : m_storage{ std::make_tuple(
			type_t{ std::get<ID_Type>(other.m_storage) },
			name_t{ std::get<ID_Name>(other.m_storage), alloc },
			data_t{ std::get<ID_Data>(other.m_storage) }
		) }
		{
		}

		uniform(uniform && other, allocator_type const & alloc = {}) noexcept : m_storage{ std::make_tuple(
			type_t{ std::move(std::get<ID_Type>(other.m_storage)) },
			name_t{ std::move(std::get<ID_Name>(other.m_storage)), alloc },
			data_t{ std::move(std::get<ID_Data>(other.m_storage)) }
		) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline uniform & operator=(uniform const & other)
		{
			uniform temp{ other };
			swap(temp);
			return (*this);
		}

		inline uniform & operator=(uniform && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(uniform & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline type_t const & type() const noexcept
		{
			return std::get<ID_Type>(m_storage);
		}

		ML_NODISCARD inline name_t const & name() const noexcept
		{
			return std::get<ID_Name>(m_storage);
		}

		ML_NODISCARD inline data_t const & data() const noexcept
		{
			return std::get<ID_Data>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline std::optional<T> get() const
		{
			if (auto const v{ ([&, this]()
			{
				switch (data().index())
				{
				case ID_Variable: return std::get<variable_type>(data());
				case ID_Function: return std::invoke(std::get<function_type>(data()));
				default			: return variable_type{};
				}
			})() }; std::holds_alternative<T>(v))
			{
				return std::make_optional<T>(std::get<T>(v));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool operator==(uniform const & other)
		{
			return (type() == other.type())
				&& (name() == other.name());
		}

		ML_NODISCARD inline bool operator!=(uniform const & other)
		{
			return !(*this == other);
		}

		ML_NODISCARD inline bool operator<(uniform const & other)
		{
			return (type() < other.type())
				|| (name() < other.name());
		}

		ML_NODISCARD inline bool operator>(uniform const & other)
		{
			return !(*this < other);
		}

		ML_NODISCARD inline bool operator<=(uniform const & other)
		{
			return (*this < other) || (*this == other);
		}

		ML_NODISCARD inline bool operator>=(uniform const & other)
		{
			return (*this > other) || (*this == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, uniform const & value)
	{
		return out
			<< value.type().name() << " "
			<< value.name() << " ";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type, class Name, class ... Args
	> ML_NODISCARD static inline auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return uniform{ std::make_tuple(
			typeof<Type>{}, std::move(name), std::forward<Args>(args)...
		) };
	}

	ML_NODISCARD static inline auto make_uniform(uniform const & value)
	{
		return uniform{ value };
	}

	ML_NODISCARD static inline auto make_uniform(uniform && value) noexcept
	{
		return uniform{ std::move(value) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_